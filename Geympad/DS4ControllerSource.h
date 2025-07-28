#pragma once

#include <iostream>
#include <iomanip>
#include "libusb.h"
#include <string>
#define VID 0x054c // idVendor
//#define PID 0x05c4
#define PID 0x09cc //idProduct
#define BULK_EP_OUT     0x84
#define BULK_EP_IN      0x03

using namespace std;

class DS4Controller
{
public:
    ~DS4Controller()
    {
        int r = 0;
        r = libusb_release_interface(handle, 0);
        if (r != 0)
        {
            return;
        }
        libusb_close(handle);
        libusb_exit(context);
    }
private:
    void Make_data()
    {
        //Тип data
        _data[0] = 0;
        //Код операции
        _data[1] = 0;

        _data[4] = 0;
        _data[5] = 0;
        _data[6] = 0;
        //Врум-врум.
        _data[7] = 0;
        _data[8] = 0;
        // R
        _data[9] = 0;
        // G
        _data[10] = 0;
        // B
        _data[11] = 0;
        // volume
        _data[25] = 0;
    }

    unsigned char _data[78];
    libusb_context* context = NULL;
    libusb_device_handle* handle = NULL;
};