#include "GxIAPI.h"
#include<iostream>

int main(int argc, char *argv[]) {
    GX_STATUS status = GX_STATUS_SUCCESS;
    //在起始位置调用 GXInitLib()进行初始化，申请资源
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
    uint32_t nDeviceNum = 0; // 一个存储设备个数的位置
    status = GXUpdateAllDeviceList(&nDeviceNum, 1000);
    std::cout << status << std::endl;
//    std::cout << status << nDeviceNum << std::endl;
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