// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <windows.h>
#include <WinIOCtl.h>
#include <usbioctl.h>
#include <Setupapi.h>

#include <string>
#include <functional>

#include "../_srv/std.h"

#include "DevInfo.h"


// CONST. ----------------------------------------------------------------------
//------------------------------------------------------------------------------

//DEFINE_GUID(GUID_DEVINTERFACE_USB_DISK
//  , 0x53f56307L, 0xb6bf, 0x11d0
//  , 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

EXTERN_C const GUID DECLSPEC_SELECTANY GUID_DEVINTERFACE_USB_DISK =
    {  0x53f56307L, 0xb6bf, 0x11d0
    , {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b} };

/* Дополнительные функции. ****************************************************/
/******************************************************************************/

// Вернуть номер устройства. ---------------------------------------------------
//------------------------------------------------------------------------------
DWORD getDeviceNumber(HANDLE deviceHandle) {
    STORAGE_DEVICE_NUMBER sdn;
    DWORD dwBytesReturned = 0;

    sdn.DeviceNumber = -1;
    if(!DeviceIoControl(
        deviceHandle, IOCTL_STORAGE_GET_DEVICE_NUMBER
      , NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL ))
    {
      // handle error - like a bad handle.
        return -1; //unsigned

    }// if(!DeviceIoControl( ...
    return sdn.DeviceNumber;
}// DWORD getDeviceNumber(HANDLE deviceHandle)


/* DevInfo. *******************************************************************/
/******************************************************************************/

// Форматированный вывод. ------------------------------------------------------
//------------------------------------------------------------------------------
QString DevInfo::format(QString fmt) {
    return fmt
        .replace("{VEN}", ven)
        .replace("{PRD}", prd)
        .replace("{REV}", rev)
        .replace("{SNM}", snm);
}// format

// Опросить устройство. --------------------------------------------------------
//------------------------------------------------------------------------------
DevInfo DevInfo::inquire(int vol) {
    DevInfo ret;
    wchar_t devicePath[7] = L"\\\\.\\@:";
    devicePath[4] = (wchar_t)(vol + 'A');
    HANDLE deviceHandle = CreateFile(
        devicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL
      , OPEN_EXISTING, 0, NULL );

    if(deviceHandle == INVALID_HANDLE_VALUE) return ret;

    // to get the device number
    DWORD volumeDeviceNumber = getDeviceNumber(deviceHandle);
    CloseHandle(deviceHandle);

    // Get device interface info set handle
    // for all devices attached to the system
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_USB_DISK, NULL, NULL
      , DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );

    if(hDevInfo == INVALID_HANDLE_VALUE) return ret;

    // Шаблоны считывания строки информации устройства.
    QRegExp ven_re("ven_([^&#]+)"), prd_re("prod_([^&#]+)");
    QRegExp rev_re("rev_([^&#]+)"), snm_re("&[^#]*#([^&#]+)");

    // Get a context structure for the device interface
    // of a device information set.
    const int BUF_SIZE = 1024;
    char buf_dtl[BUF_SIZE];
    char buf_res[BUF_SIZE/2 + 1];

    PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd =
        (PSP_DEVICE_INTERFACE_DETAIL_DATA) buf_dtl;
    SP_DEVICE_INTERFACE_DATA         spdid;
    SP_DEVINFO_DATA                  spdd;

    spdid.cbSize = sizeof(spdid);

    DWORD dwIndex = 0;
    while(true) {
        if(!SetupDiEnumDeviceInterfaces(
            hDevInfo, NULL, &GUID_DEVINTERFACE_USB_DISK, dwIndex, &spdid ))
        { break; }

        DWORD dwSize = 0;
        SetupDiGetDeviceInterfaceDetail(
            hDevInfo, &spdid, NULL, 0, &dwSize, NULL );

        if( (dwSize != 0) && (dwSize <= sizeof(buf_dtl)) ) {
            pspdidd->cbSize = sizeof( *pspdidd ); // 5 Bytes!

            ZeroMemory((PVOID)&spdd, sizeof(spdd));
            spdd.cbSize = sizeof(spdd);

            // Запрос параметров устройства.
            long res = SetupDiGetDeviceInterfaceDetail(
                hDevInfo, &spdid, pspdidd,
                dwSize, &dwSize, &spdd );

            if(res) {
                HANDLE hDrive = CreateFile(
                    pspdidd->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE
                  , NULL, OPEN_EXISTING, 0, NULL );

                if(hDrive != INVALID_HANDLE_VALUE) {
                    DWORD usbDeviceNumber = getDeviceNumber(hDrive);

                    if(usbDeviceNumber == volumeDeviceNumber) {

                        int cnt = 0;
                        for(int i=4; i<1024; i+=2) {
                            buf_res[cnt] = buf_dtl[i];
                            cnt++;
                        }// i
                        QString str = QString::fromLatin1(buf_res, cnt);

                        auto CAP = [&str](QRegExp re) -> QString {
                            return re.indexIn(str) == -1 ? "" : re.cap(1);
                        };

                        ret = DevInfo(
                            CAP(ven_re), CAP(prd_re)
                          , CAP(rev_re), CAP(snm_re) );

                    }// if(usbDeviceNumber == volumeDeviceNumber)
                }// if(hDrive != INVALID_HANDLE_VALUE)
                CloseHandle(hDrive);

            }// if(res)
        }// if( (dwSize != 0) && (dwSize <= sizeof(Buf)) )

        dwIndex++;
    }// while(true)

   SetupDiDestroyDeviceInfoList(hDevInfo);
   return ret;
}// getDeviceInfo

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString DevInfo::toString(void) {
    return QString("DeviceInfo{")
      + "ven=" + ven + ", prd=" + prd + ", "
      + "rev=" + rev + ", snm=" + snm + "}";
}// toString

//------------------------------------------------------------------------------
