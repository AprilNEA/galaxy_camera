#include "GxIAPI.h"
#include "DxImageProc.h"
#include<iostream>

// 声明相机参数的数据类型
struct Camara_Param {
    GX_API methodName;
    int featureID;
};
//class Camara_Param {
//public:
//    std::string methodName;
//    int featureID;
//};
// 相机类型的名称
std::string GX_DEVICE_TYPENAME[5] = {"GX_DEVICE_CLASS_UNKNOWN", "GX_DEVICE_CLASS_USB2", "GX_DEVICE_CLASS_GEV",
                                     "GX_DEVICE_CLASS_U3V", "GX_DEVICE_CLASS_SMART"};

int main(int argc, char *argv[]) {
    GX_STATUS status = GX_STATUS_SUCCESS;
    // 在起始位置调用 GXInitLib()进行初始化，申请资源
    status = GXInitLib();

    if (status != GX_STATUS_SUCCESS) {
        std::cout << "Failed" << status << std::endl;
    } else {
        std::cout << "Succeed" << std::endl;
    }
    status = GX_STATUS_SUCCESS;
    GX_DEV_HANDLE hDevice = NULL;

    // 打开第一个设备
//    status = GXOpenDeviceByIndex(1, &hDevice);

    // 首先拉取设备列表
    uint32_t nDeviceNum = 0; // 一个存储设备个数的位置
    // TODO 两个函数的区别
//    status = GXUpdateAllDeviceList(&nDeviceNum, 1000);
    GXUpdateDeviceList(&nDeviceNum, 1000);

    if (nDeviceNum >= 1) {
        // 尝试寻找对应 SN 的相机
        // 获取设备的基础信息
        GX_DEVICE_BASE_INFO pBaseInfo[nDeviceNum];
        size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);

        status = GXGetAllDeviceBaseInfo(pBaseInfo, &nSize);

        bool found_device = false;
        for (int i = 0; i < nDeviceNum; ++i) {

            std::cout << "Found "<< nDeviceNum<< " devices"<< std::endl
                      << "--Device "<< i<<"  SN: " << pBaseInfo[i].szSN
                      << " NAME: " << pBaseInfo[i].szDisplayName
                      << " TYPE: " << GX_DEVICE_TYPENAME[pBaseInfo[i].deviceClass]
                      << std::endl;
//            if (std::string(pBaseInfo[i].szSN) == sn) {
//                found_device = true;
//            }
//            std::cout<<sn<<std:endl;
        }
        found_device = true;
        if (!found_device) {
//            std::cerr << "No device found with SN:" << sn << std::endl;
            // FIXME now in mian function if in other function it should be change here
            return 1;
        }
//    if (status == GX_STATUS_SUCCESS && nDeviceNum > 0) {
//        GX_DEVICE_BASE_INFO *pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
//        uint32_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
//        //获取所有设备的基础信息
//        status = GXGetAllDeviceBaseInfo(pBaseinfo, &nSize);
//        std::cout << status << std::endl;
//        delete []pBaseinfo;
//    }
        //在结束的时候调用 GXCLoseLib()释放资源
        status = GXCloseLib();
        return 0;
    }
}