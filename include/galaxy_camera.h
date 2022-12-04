// Copyright (c) 2022 GMaster All rights reserved.
// License
// Created by whoismz on 2022-02-01.
// Refactored by AprilNEA on 2022-10-31.
// This is the package code for DaHeng galaxy_camera
//
#ifndef GMASTER_WM_NEW_CAMWRAPPER_H
#define GMASTER_WM_NEW_CAMWRAPPER_H

#include "libgxiapi/GxIAPI.h"
#include<iostream>
#include<thread>
#include<opencv2/opencv.hpp>
#include<opencv2/opencv.hpp>

/**
 * @brief 相机的父类
 * 为后期拓展相机做准备
*/
class Camera {
public:
    virtual bool init(int roi_x, int roi_y, int roi_w, int roi_h,
                      float exposure, float gain, bool isEnergy) = 0;

//    virtual void setParam(float exposure, float gain) = 0;

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual bool init_is_successful() = 0;

    virtual bool read(cv::Mat &src) = 0;
};


class DHCamera : public Camera {
    friend void getRGBImage(DHCamera *camera);

    friend void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

public:

    DHCamera(std::string sn);  // constructor, p_img is a pointer towards a
    // 640*640 8uc3 Mat type

    ~DHCamera();

    bool init(int roi_x, int roi_y, int roi_w, int roi_h, float exposure,
              float gain, bool isEnergy) final;  // camInit galaxy_camera lib and do settings, be called firstly
//    void setParam(float exposure, float gain) final;  // set exposure and gain

    bool start() final;                    // start video stream
    void stop() final;                     // stop receiving frames

//    void calcRoi();                        // autmatic resize parameters
    bool init_is_successful() final;       // return video is available or not

    bool read(cv::Mat &src) final;

private:
    std::string sn; // 相机的序列号
    GX_STATUS status{}; // 上一次调用大恒相机的API是否成功

    int64_t nPayLoadSize{};
    uint32_t nDeviceNum{};

    GX_DEV_HANDLE g_hDevice{}; // 相机句柄
    GX_FRAME_DATA g_frameData{}; // 相机帧数据
    PGX_FRAME_BUFFER g_frameBuffer{}; // 相机帧数据 二进制格式

    int64_t g_nPixelFormat{};
    int64_t g_nColorFilter{};
    int64_t g_SensorHeight{};
    int64_t g_SensorWidth{};
    int64_t frame_cnt;
    double frame_get_time;
    cv::Mat p_img, p_energy;
    void *g_pRGBframeData{};
    void *g_pRaw8Buffer{};
    std::thread cam_run;
    bool thread_running;
    bool init_success;
    bool is_energy;
    std::mutex pimg_lock;
    std::chrono::steady_clock::time_point fps_time_point;


};

#endif //GMASTER_WM_NEW_CAMWRAPPER_H
