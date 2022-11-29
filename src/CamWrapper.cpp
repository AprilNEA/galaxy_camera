// Copyright (c) 2022 GMaster All rights reserved.
// License
// Created by whoismz on 2022-02-01.
// Refactored by AprilNEA on 2022-10-31.
// This is the package code for DaHeng camera
//
#include "CamWrapper.h"
#include "DxImageProc.h"
//#include <glog/logging.h>
#include<chrono>
#include<mutex>
#include<thread>

void update_bool(GX_STATUS status, bool &flag, const std::string &w_str = "") {
    if (status != GX_STATUS_SUCCESS) {
        flag = true;
//        LOG(INFO) << w_str << " set failed!";
    }
}

#define UPDB(x, wstr) (update_bool(x, set_failed, wstr))

void ProcessData(void *pImageBuf, void *pImageRaw8Buf, void *pImageRGBBuf,
                 int nImageWidth, int nImageHeight, int nPixelFormat,
                 int nPixelColorFilter) {
    /*！
     *
     */
    switch (nPixelFormat) {

        case GX_PIXEL_FORMAT_BAYER_GR12:
        case GX_PIXEL_FORMAT_BAYER_RG12:
        case GX_PIXEL_FORMAT_BAYER_GB12:
        case GX_PIXEL_FORMAT_BAYER_BG12:

            // Convert RGB48 to RGB24
            DxRaw16toRaw8(pImageBuf, pImageRaw8Buf, nImageWidth, nImageHeight,
                          DX_BIT_4_11);
            // Convert Raw8 to Rgb24 with choosen RGB channel order
            DxRaw8toRGB24(pImageRaw8Buf, pImageRGBBuf, nImageWidth,
                          nImageHeight, RAW2RGB_NEIGHBOUR,
                          DX_PIXEL_COLOR_FILTER(nPixelColorFilter), false);
            break;


        case GX_PIXEL_FORMAT_BAYER_GR10:
        case GX_PIXEL_FORMAT_BAYER_RG10:
        case GX_PIXEL_FORMAT_BAYER_GB10:
        case GX_PIXEL_FORMAT_BAYER_BG10:

            DxRaw16toRaw8(pImageBuf, pImageRaw8Buf, nImageWidth, nImageHeight,
                          DX_BIT_2_9);

            DxRaw8toRGB24(pImageRaw8Buf, pImageRGBBuf, nImageWidth,
                          nImageHeight, RAW2RGB_NEIGHBOUR,
                          DX_PIXEL_COLOR_FILTER(nPixelColorFilter), false);
            break;

        case GX_PIXEL_FORMAT_BAYER_GR8:
        case GX_PIXEL_FORMAT_BAYER_RG8:
        case GX_PIXEL_FORMAT_BAYER_GB8:
        case GX_PIXEL_FORMAT_BAYER_BG8:


            DxRaw8toRGB24(pImageBuf, pImageRGBBuf, nImageWidth, nImageHeight,
                          RAW2RGB_NEIGHBOUR,
                          DX_PIXEL_COLOR_FILTER(nPixelColorFilter),
                          false); // RAW2RGB_ADAPTIVE
            break;

        case GX_PIXEL_FORMAT_MONO12:

            DxRaw16toRaw8(pImageBuf, pImageRaw8Buf, nImageWidth, nImageHeight,
                          DX_BIT_4_11);

            DxRaw8toRGB24(pImageRaw8Buf, pImageRGBBuf, nImageWidth,
                          nImageHeight, RAW2RGB_NEIGHBOUR,
                          DX_PIXEL_COLOR_FILTER(NONE), false);
            break;

        case GX_PIXEL_FORMAT_MONO10:

            DxRaw16toRaw8(pImageBuf, pImageRaw8Buf, nImageWidth, nImageHeight,
                          DX_BIT_4_11);

            DxRaw8toRGB24(pImageRaw8Buf, pImageRGBBuf, nImageWidth,
                          nImageHeight, RAW2RGB_NEIGHBOUR,
                          DX_PIXEL_COLOR_FILTER(NONE), false);
            break;

        case GX_PIXEL_FORMAT_MONO8:

            DxRaw8toRGB24(pImageBuf, pImageRGBBuf, nImageWidth, nImageHeight,
                          RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(NONE),
                          false);
            break;

        default:
            break;
    }
}

void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame) {
    /*!
     *
     */
    if (pFrame->status == GX_FRAME_STATUS_SUCCESS) {
        DHCamera *cam = (DHCamera *) pFrame->pUserParam;
        auto start = std::chrono::steady_clock::now();
        ProcessData((void *) pFrame->pImgBuf, cam->g_pRaw8Buffer,
                    cam->g_pRGBframeData, pFrame->nWidth, pFrame->nHeight,
                    pFrame->nPixelFormat, cam->g_nColorFilter);

        // cam->full_gpu.upload(cam->full);

        // cv::cuda::resize(cam->full_gpu,cam->resize_gpu,cv::Size(640,640));
        // cv::cuda::cvtColor(cam->resize_gpu,cam->resize_gpu,cv::COLOR_RGB2BGR);
        if (cam->is_energy) {
            memcpy(cam->p_energy.data, cam->g_pRGBframeData,
                   3 * (cam->nPayLoadSize));
            cam->pimg_lock.lock();
            cv::resize(cam->p_energy, cam->p_img, cv::Size(1280, 1024),
                       cv::INTER_NEAREST);
            cv::cvtColor(cam->p_img, cam->p_img, cv::COLOR_RGB2BGR);
            cam->pimg_lock.unlock();
        } else {
            cam->pimg_lock.lock();
            // cv::resize(cam->full,cam->p_img,cv::Size(640,640),cv::INTER_NEAREST);
            // cv::cvtColor(cam->p_img,cam->p_img,cv::COLOR_RGB2BGR);
            // cam->resize_gpu.download(cam->p_img);
            memcpy(cam->p_img.data, cam->g_pRGBframeData,
                   3 * (cam->nPayLoadSize));
            cv::cvtColor(cam->p_img, cam->p_img, cv::COLOR_RGB2BGR);
            cam->pimg_lock.unlock();
        }
        auto end = std::chrono::steady_clock::now();
        cam->frame_cnt++;
        cam->frame_get_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (cam->frame_cnt == 500) {
            double fps_time_interval =
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - cam->fps_time_point).count();
//            LOG(INFO) << "average hkcamera delay(ms):"
//                      << cam->frame_get_time / cam->frame_cnt
//                      << " acq fps:" << 1000.0 / (fps_time_interval / 500.0);
            cam->frame_get_time = cam->frame_cnt = 0;
            cam->fps_time_point = end;
        }
    }
    return;
}

void getRGBImage(DHCamera *cam) {
    while (1) {
        if (!cam->thread_running) {
            return;
        }
        GX_STATUS status;
        status = GXDQBuf(cam->g_hDevice, &cam->g_frameBuffer, 1000);
        auto start = std::chrono::steady_clock::now();
        // cam->g_frameBuffer.

        ProcessData(cam->g_frameBuffer->pImgBuf, cam->g_pRaw8Buffer,
                    cam->g_pRGBframeData, cam->g_frameBuffer->nWidth,
                    cam->g_frameBuffer->nHeight,
                    cam->g_frameBuffer->nPixelFormat, cam->g_nColorFilter);
        if (cam->is_energy) {
            // LOG(INFO) << cam->g_frameBuffer->nWidth << " " <<
            // cam->g_frameBuffer->nHeight << " " << cam->nPayLoadSize;
            memcpy(cam->p_energy.data, cam->g_pRGBframeData,
                   3 * (cam->nPayLoadSize));
            cam->pimg_lock.lock();
            cv::resize(cam->p_energy, cam->p_img, cv::Size(1280, 1024),
                       cv::INTER_NEAREST);
            cv::cvtColor(cam->p_img, cam->p_img, cv::COLOR_RGB2BGR);
            cam->pimg_lock.unlock();
        } else {
            cam->pimg_lock.lock();
            memcpy(cam->p_img.data, cam->g_pRGBframeData,
                   3 * (cam->nPayLoadSize));
            cv::cvtColor(cam->p_img, cam->p_img, cv::COLOR_RGB2BGR);
            cam->pimg_lock.unlock();
        }
        GXQBuf(cam->g_hDevice, cam->g_frameBuffer);
        auto end = std::chrono::steady_clock::now();
        cam->frame_cnt++;
        cam->frame_get_time +=
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                        .count();
        if (cam->frame_cnt == 500) {
            std::cout << "average camera delay(ms):"
                      << cam->frame_get_time / cam->frame_cnt << std::endl;
            cam->frame_get_time = cam->frame_cnt = 0;
        }
    }
}

DHCamera::DHCamera(std::string sn)
        : sn(sn),
          thread_running(false),
          frame_cnt(0),
          frame_get_time(0),
          init_success(false),
          is_energy(false) {
    p_img = cv::Mat(1024, 1280, CV_8UC3);
    p_energy = cv::Mat(1024, 1280, CV_8UC3);
};

DHCamera::~DHCamera() {
    if (init_success) {
        stop();
        if (g_frameData.pImgBuf != NULL) {
            free(g_frameData.pImgBuf);
        }
        if (g_pRGBframeData != NULL) {
            free(g_pRGBframeData);
        }
        GXCloseDevice(g_hDevice);
    }
    GXCloseLib();
}

std::string gc_device_typename[5] = {
        "GX_DEVICE_CLASS_UNKNOWN", "GX_DEVICE_CLASS_USB2", "GX_DEVICE_CLASS_GEV",
        "GX_DEVICE_CLASS_U3V", "GX_DEVICE_CLASS_SMART"};

bool DHCamera::init(int roi_x, int roi_y, int roi_w, int roi_h, float exposure, float gain, bool isEnergy) {
    // 在起始位置调用 GXInitLib()进行初始化，申请资源
    GXInitLib();
    // 更新相机列表
    GXUpdateDeviceList(&nDeviceNum, 1000);
    if (nDeviceNum >= 1) {
        // 尝试寻找对应 SN 的相机
        // 获取设备的基础信息
        GX_DEVICE_BASE_INFO pBaseInfo[nDeviceNum];
        size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);

        status = GXGetAllDeviceBaseInfo(pBaseInfo, &nSize);

        bool found_device = false;
        for (int i = 0; i < nDeviceNum; ++i) {
            //
            std::cout << "device: SN:" << pBaseInfo[i].szSN
                      << " NAME:" << pBaseInfo[i].szDisplayName << " TYPE:"
                      << gc_device_typename[pBaseInfo[i].deviceClass]
                      << std::endl;
            if (std::string(pBaseInfo[i].szSN) == sn) {
                found_device = true;
            }
        }

        if (!found_device) {
            std::cerr << "No device found with SN:" << sn << std::endl;
            return false;
        }

        GX_OPEN_PARAM stOpenParam;
        stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
        stOpenParam.openMode = GX_OPEN_SN;
        stOpenParam.pszContent = const_cast<char *>(sn.c_str());
        status = GXOpenDevice(&stOpenParam, &g_hDevice);

        GXGetInt(g_hDevice, GX_INT_SENSOR_WIDTH, &g_SensorWidth);
        GXGetInt(g_hDevice, GX_INT_SENSOR_HEIGHT, &g_SensorHeight);
        std::cout << "DHCamera Sensor: " << g_SensorWidth << " X "
                  << g_SensorHeight << std::endl;

        // 进行个性化设置
        bool set_failed = false;

        // 设置感兴趣区域
        UPDB(GXSetInt(g_hDevice, GX_INT_OFFSET_X, roi_x), "ROI_X");
        UPDB(GXSetInt(g_hDevice, GX_INT_OFFSET_Y, roi_y), "ROI_Y");
        UPDB(GXSetInt(g_hDevice, GX_INT_WIDTH, roi_w), "ROI_W");
        UPDB(GXSetInt(g_hDevice, GX_INT_HEIGHT, roi_h), "ROI_H");
        // 曝光
        UPDB(GXSetEnum(g_hDevice, GX_ENUM_EXPOSURE_AUTO, GX_EXPOSURE_AUTO_OFF), "ExposureAuto");
        // 增益
        UPDB(GXSetEnum(g_hDevice, GX_ENUM_GAIN_AUTO, GX_GAIN_AUTO_OFF), "GainAuto");
        // 白平衡
//        UPDB(GXSetEnum(g_hDevice, GX_ENUM_BLACKLEVEL_AUTO, GX_BLACKLEVEL_AUTO_OFF), "BlacklevelAuto");
        UPDB(GXSetEnum(g_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS), "BalanceWhiteAuto");
        // 坏点校正
//        UPDB(GXSetEnum(g_hDevice, GX_ENUM_DEAD_PIXEL_CORRECT,GX_DEAD_PIXEL_CORRECT_OFF),"DeadPixelCorrect");
        // 采集模式
        UPDB(GXSetEnum(g_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS), "AcquisitionMode");
        // 采集速度级别
//        UPDB(GXSetInt(g_hDevice, GX_INT_ACQUISITION_SPEED_LEVEL, 4),"AcquisitionSpeed");

        // 获取曝光调节范围
        UPDB(GXSetFloat(g_hDevice, GX_FLOAT_EXPOSURE_TIME, exposure), "Exposure");
        // 获取增益调节范围
        UPDB(GXSetFloat(g_hDevice, GX_FLOAT_GAIN, gain), "Gain");
        // 获取黑电平调节范围
//        UPDB(GXSetFloat(g_hDevice, GX_FLOAT_BLACKLEVEL, 0), "Blacklevel");
        // 增益通道选择
        UPDB(GXSetEnum(g_hDevice, GX_ENUM_GAIN_SELECTOR, GX_GAIN_SELECTOR_ALL), "GainSelector");

        if (set_failed) {
//            LOG(ERROR) << "failed to set some parameters!";
            return false;
        }

        // 获取白平衡调节范围
        GX_FLOAT_RANGE gainRange;
        GXGetFloatRange(g_hDevice, GX_FLOAT_GAIN, &gainRange);
        std::cout << "DHCamera Gain Range: " << gainRange.dMin << "~"
                  << gainRange.dMax << " step size:" << gainRange.dInc
                  << std::endl;
        // 传输层控制
        GXGetInt(g_hDevice, GX_INT_PAYLOAD_SIZE, &nPayLoadSize);

        g_frameData.pImgBuf = malloc(nPayLoadSize);
        g_pRGBframeData = malloc(nPayLoadSize * 3);

        GXGetEnum(g_hDevice, GX_ENUM_PIXEL_FORMAT, &g_nPixelFormat);
        GXGetEnum(g_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &g_nColorFilter);

        init_success = true;
        thread_running = false;
        is_energy = isEnergy;
        return true;
    } else {
        return false;
    }
}

void DHCamera::setParam(float exposure, float gain) {
    if (init_success) {
        GXSetFloat(g_hDevice, GX_FLOAT_EXPOSURE_TIME, exposure);
        GXSetFloat(g_hDevice, GX_FLOAT_GAIN, gain);
    }
}

bool DHCamera::start() {
    if (init_success) {
        frame_cnt = frame_get_time = 0;
        fps_time_point = std::chrono::steady_clock::now();
        GXRegisterCaptureCallback(g_hDevice, this, OnFrameCallbackFun);
        GXSendCommand(g_hDevice, GX_COMMAND_ACQUISITION_START);
        return true;
    }
    return false;
}

void DHCamera::stop() {
    if (init_success) {
        GXSendCommand(g_hDevice, GX_COMMAND_ACQUISITION_STOP);
        GXUnregisterCaptureCallback(g_hDevice);
    }
}

bool DHCamera::init_is_successful() { return init_success; }

bool DHCamera::read(cv::Mat &src) {
    pimg_lock.lock();
    p_img.copyTo(src);
//    cv::swap(p_img, src);
    pimg_lock.unlock();
    return true;
}