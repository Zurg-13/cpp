#define USBHelper_h__

#include <windows.h>


#define USB_STORE_NO_ERROR                                      0L
#define USB_STORE_NO_MORE_DEVICE                                8L
#define USB_STORE_ERROR_SETUP_DI_GET_CLASS_DEVS                 1L
#define USB_STORE_ERROR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL    2L
#define USB_STORE_ERROR_GET_VOLUME_PATH_NAME_FOR_VOLUME_NAME    3L
#define USB_STORE_ERROR_GET_VOLUME_NAME_FOR_VOLUME_MOUNT_POINT  4L
#define USB_STORE_ERROR_INVALID_PARAMETRS                       5L
#define USB_STORE_ERROR_INSUFFICIENT_BUFFER                     6L
#define USB_STORE_ERROR_SETUP_DI_GET_DEVICE_REGISTRY_PROPERTY   7L

typedef struct _USB_DEVICE_STRORE_A {
    CHAR *DevicePhysicalName;
    CHAR *DeviceInterfacePath;
    CHAR *DeviceVolumeName;
    CHAR *DeviceVolumePath;
} USB_DEVICE_STRORE_A, *PUSB_DEVICE_STRORE_A;

typedef struct _USB_DEVICE_STRORE_W {
    WCHAR *DevicePhysicalName;
    WCHAR *DeviceInterfacePath;
    WCHAR *DeviceVolumeName;
    WCHAR *DeviceVolumePath;
} USB_DEVICE_STRORE_W, *PUSB_DEVICE_STRORE_W;


#ifdef UNICODE
    typedef USB_DEVICE_STRORE_W USB_DEVICE_STRORE;
    typedef PUSB_DEVICE_STRORE_W PUSB_DEVICE_STRORE;
#else
    typedef USB_DEVICE_STRORE_A USB_DEVICE_STRORE;
    typedef PUSB_DEVICE_STRORE_A PUSB_DEVICE_STRORE;
#endif


BOOL WINAPI GetUSBStoreDevicesListA(
    __out_bcount_opt(USBDeviceListDataSize)PUSB_DEVICE_STRORE_A USBDeviceList,
    __in DWORD USBDeviceListDataSize,
    __out_opt PDWORD RequiredSize
    );

BOOL WINAPI GetUSBStoreDevicesListW(
    __out_bcount_opt(USBDeviceListDataSize)PUSB_DEVICE_STRORE_W USBDeviceList,
    __in DWORD USBDeviceListDataSize,
    __out_opt PDWORD RequiredSize
    );


#ifdef UNICODE
    #define GetUSBStoreDevicesList GetUSBStoreDevicesListW
#else
    #define GetUSBStoreDevicesList GetUSBStoreDevicesListA
#endif

VOID WINAPI ReleaseUSBStroreA(
    __in_ecount(USBDeviceListDataSize)PUSB_DEVICE_STRORE_A USBDeviceList,
    __in DWORD USBDeviceListDataSize
    );

VOID WINAPI ReleaseUSBStroreW(
    __in_ecount(USBDeviceListDataSize)PUSB_DEVICE_STRORE_W USBDeviceList,
    __in DWORD USBDeviceListDataSize
    );

#ifdef UNICODE
    #define ReleaseUSBStrore ReleaseUSBStroreW
#else
    #define ReleaseUSBStrore ReleaseUSBStroreA
#endif

DWORD WINAPI GetLastErrorUSBStore();

#endif // USBHelper_h__
