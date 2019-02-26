#include "USBHelper.h"
#include <Setupapi.h>


#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

// #define USB_STORE_NO_ERROR                                       0L
// #define USB_STORE_ERROR_SETUP_DI_GET_CLASS_DEVS                  1L
// #define USB_STORE_ERROR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL     2L
// #define USB_STORE_ERROR_GET_VOLUME_PATH_NAME_FOR_VOLUME_NAME     3L
// #define USB_STORE_ERROR_GET_VOLUME_NAME_FOR_VOLUME_MOUNT_POINT   4L
// #define USB_STORE_ERROR_INVALID_PARAMETRS                        5L
// #define USB_STORE_ERROR_INSUFFICIENT_BUFFER                      6L
//
//
// typedef struct _USB_DEVICE_STRORE {
//  TCHAR *DeviceName;
//  TCHAR *DeviceInstanceID;
//  TCHAR *DevicePhysicalName;
//  TCHAR *DeviceInterfacePath;
//  TCHAR *DeviceVolumeName;
//  TCHAR *DeviceVolumePath;
// } USB_DEVICE_STRORE, *PUSB_DEVICE_STRORE;

DWORD ErrorCode = USB_STORE_NO_ERROR;
#define LocalFreeIf(Pointer) if(Pointer) { LocalFree(Pointer); Pointer = NULL; }

BOOL WINAPI GetUSBStoreDevicesListA(
    __out_bcount_opt(USBDeviceListDataSize)PUSB_DEVICE_STRORE_A USBDeviceList,
    __in DWORD USBDeviceListDataSize,
    __out_opt PDWORD RequiredSize )
{

    if( USBDeviceList == NULL && RequiredSize == NULL ) {
        ErrorCode = USB_STORE_ERROR_INVALID_PARAMETRS;
        return FALSE;
    }

    PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDeviceInterfaceDetailData = NULL;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    SP_DEVINFO_DATA DeviceInfoData;

    HDEVINFO hDevInfo;
    CHAR *lpBuffer = NULL;

    DWORD len   = 0;
    DWORD Index = 0;

    const GUID InterfaceGuid = { 0x53f5630dL, 0xb6bf, 0x11d0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b} };

    hDevInfo = SetupDiGetClassDevsA( &InterfaceGuid,0, 0, DIGCF_PRESENT  | DIGCF_DEVICEINTERFACE  );

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_CLASS_DEVS;
        return FALSE;
    }

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for(DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, 0, &InterfaceGuid, i, &DeviceInterfaceData); ++i)
    {
        ULONG RequiredLength = 0;

        while ( !SetupDiGetDeviceInterfaceDetailA( hDevInfo,
            &DeviceInterfaceData, pDeviceInterfaceDetailData, RequiredLength, &RequiredLength, &DeviceInfoData ) )
        {
            if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
            {
                LocalFreeIf( pDeviceInterfaceDetailData );
                pDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)LocalAlloc(LMEM_FIXED, (RequiredLength + 2));
                pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
            }
            else
            {
                ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL;
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return FALSE;
            }
        }

        if( strstr(pDeviceInterfaceDetailData->DevicePath, "usbstor") )
        {
            if( USBDeviceList == NULL )         { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }
            if( USBDeviceListDataSize == 0 )    { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }
            if( USBDeviceListDataSize < Index ) { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }

            CHAR Volume[256] = { 0 };
            CHAR Letter[256] = { 0 };

            len = strlen( pDeviceInterfaceDetailData->DevicePath );

            USBDeviceList[Index].DeviceInterfacePath = (CHAR*)LocalAlloc(LPTR, len + 1);
            strcpy(USBDeviceList[Index].DeviceInterfacePath, pDeviceInterfaceDetailData->DevicePath);

            pDeviceInterfaceDetailData->DevicePath[len]     = '\\';
            pDeviceInterfaceDetailData->DevicePath[len + 1] = '\0';


            if( GetVolumeNameForVolumeMountPointA(pDeviceInterfaceDetailData->DevicePath, Volume, 255) )
            {
                len = strlen( Volume );
                USBDeviceList[Index].DeviceVolumeName = (CHAR*)LocalAlloc(LPTR, len + 1);
                strcpy(USBDeviceList[Index].DeviceVolumeName, Volume);

                if (GetVolumePathNamesForVolumeNameA(Volume, Letter, 255, &RequiredLength))
                {
                    len = strlen( Letter );
                    USBDeviceList[Index].DeviceVolumePath = (CHAR*)LocalAlloc(LPTR, len + 1);
                    strcpy(USBDeviceList[Index].DeviceVolumePath, Letter);

                }
                else
                {
                    LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
                    ReleaseUSBStroreA(USBDeviceList, Index);

                    ErrorCode = USB_STORE_ERROR_GET_VOLUME_PATH_NAME_FOR_VOLUME_NAME;
                    LocalFreeIf(pDeviceInterfaceDetailData);
                    SetupDiDestroyDeviceInfoList(hDevInfo);

                    return FALSE;
                }

            }
            else
            {
                LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                ReleaseUSBStroreA(USBDeviceList, Index);

                ErrorCode = USB_STORE_ERROR_GET_VOLUME_NAME_FOR_VOLUME_MOUNT_POINT;
                LocalFreeIf(pDeviceInterfaceDetailData);
                SetupDiDestroyDeviceInfoList(hDevInfo);

                return FALSE;
            }

            RequiredLength = 0;
            while ( !SetupDiGetDeviceRegistryPropertyA(hDevInfo,
                &DeviceInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, NULL,
                (PBYTE)lpBuffer, RequiredLength, &RequiredLength) )
            {
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    LocalFreeIf(lpBuffer);
                    lpBuffer = (CHAR*)LocalAlloc(LPTR, RequiredLength + 1);
                }
                else
                {
                    LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
                    ReleaseUSBStroreA(USBDeviceList, Index);

                    ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_DEVICE_REGISTRY_PROPERTY;
                    LocalFreeIf(pDeviceInterfaceDetailData);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    return FALSE;
                }
            }

            len = strlen( lpBuffer );
            USBDeviceList[Index].DevicePhysicalName = (CHAR*)LocalAlloc(LPTR, len + 1);
            strcpy(USBDeviceList[Index].DevicePhysicalName, lpBuffer);

            ++Index;
        }


        LocalFreeIf(lpBuffer);
        LocalFreeIf(pDeviceInterfaceDetailData);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);

    if(RequiredSize != NULL){
            if( Index == 0 ){
                ErrorCode = USB_STORE_NO_MORE_DEVICE;
                return FALSE;
            }

            *RequiredSize = Index;
    }

    if( USBDeviceListDataSize == 0 || USBDeviceListDataSize < Index || USBDeviceList == NULL)
    {
        ErrorCode = USB_STORE_ERROR_INSUFFICIENT_BUFFER;
        return FALSE;
    }

    ErrorCode = USB_STORE_NO_ERROR;
    return TRUE;
}

BOOL WINAPI GetUSBStoreDevicesListW(
    __out_bcount_opt(USBDeviceListDataSize)PUSB_DEVICE_STRORE_W USBDeviceList,
    __in DWORD USBDeviceListDataSize,
    __out_opt PDWORD RequiredSize )
{
    if( USBDeviceList == NULL && RequiredSize == NULL ) {
        ErrorCode = USB_STORE_ERROR_INVALID_PARAMETRS;
        return FALSE;
    }

    PSP_DEVICE_INTERFACE_DETAIL_DATA_W pDeviceInterfaceDetailData = NULL;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    SP_DEVINFO_DATA DeviceInfoData;

    HDEVINFO hDevInfo;
    WCHAR *lpBuffer = NULL;

    DWORD len   = 0;
    DWORD Index = 0;

    const GUID InterfaceGuid = { 0x53f5630dL, 0xb6bf, 0x11d0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b} };

    hDevInfo = SetupDiGetClassDevsW( &InterfaceGuid,0, 0, DIGCF_PRESENT  | DIGCF_DEVICEINTERFACE  );

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_CLASS_DEVS;
        return FALSE;
    }

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for(DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, 0, &InterfaceGuid, i, &DeviceInterfaceData); ++i)
    {
        ULONG RequiredLength = 0;

        while ( !SetupDiGetDeviceInterfaceDetailW( hDevInfo,
            &DeviceInterfaceData, pDeviceInterfaceDetailData, RequiredLength, &RequiredLength, &DeviceInfoData ) )
        {
            if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
            {
                LocalFreeIf( pDeviceInterfaceDetailData );
                pDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)LocalAlloc(LMEM_FIXED, (RequiredLength + 2) * sizeof(WCHAR));
                pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
            }
            else
            {
                ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_DEVICE_INTERFACE_DETAIL;
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return FALSE;
            }
        }

        if( wcsstr(pDeviceInterfaceDetailData->DevicePath, L"usbstor") )
        {
            if( USBDeviceList == NULL )         { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }
            if( USBDeviceListDataSize == 0 )    { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }
            if( USBDeviceListDataSize < Index ) { ++Index; LocalFreeIf(pDeviceInterfaceDetailData); continue; }

            WCHAR Volume[256] = { 0 };
            WCHAR Letter[256] = { 0 };

            len = wcslen( pDeviceInterfaceDetailData->DevicePath );

            USBDeviceList[Index].DeviceInterfacePath = (WCHAR*)LocalAlloc(LPTR, (len + 1) * sizeof(WCHAR));
            wcscpy(USBDeviceList[Index].DeviceInterfacePath, pDeviceInterfaceDetailData->DevicePath);

            pDeviceInterfaceDetailData->DevicePath[len]     = L'\\';
            pDeviceInterfaceDetailData->DevicePath[len + 1] = L'\0';


            if( GetVolumeNameForVolumeMountPointW(pDeviceInterfaceDetailData->DevicePath, Volume, 255) )
            {
                len = wcslen( Volume );
                USBDeviceList[Index].DeviceVolumeName = (WCHAR*)LocalAlloc(LPTR, (len + 1) * sizeof(WCHAR));
                wcscpy(USBDeviceList[Index].DeviceVolumeName, Volume);

                if (GetVolumePathNamesForVolumeNameW(Volume, Letter, 255, &RequiredLength))
                {
                    len = wcslen( Letter );
                    USBDeviceList[Index].DeviceVolumePath = (WCHAR*)LocalAlloc(LPTR, (len + 1) * sizeof(WCHAR));
                    wcscpy(USBDeviceList[Index].DeviceVolumePath, Letter);
                }
                else
                {
                    LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
                    ReleaseUSBStroreW(USBDeviceList, Index);

                    ErrorCode = USB_STORE_ERROR_GET_VOLUME_PATH_NAME_FOR_VOLUME_NAME;
                    LocalFreeIf(pDeviceInterfaceDetailData);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    return FALSE;
                }

            }
            else
            {
                LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                ReleaseUSBStroreW(USBDeviceList, Index);

                ErrorCode = USB_STORE_ERROR_GET_VOLUME_NAME_FOR_VOLUME_MOUNT_POINT;
                LocalFreeIf(pDeviceInterfaceDetailData);
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return FALSE;
            }

            RequiredLength = 0;
            while ( !SetupDiGetDeviceRegistryPropertyW(hDevInfo,
                &DeviceInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, NULL,
                (PBYTE)lpBuffer, RequiredLength, &RequiredLength) )
            {
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    LocalFreeIf(lpBuffer);
                    lpBuffer = (WCHAR*)LocalAlloc(LPTR, (RequiredLength + 1) * sizeof(WCHAR));
                }
                else
                {
                    LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumePath);
                    LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
                    ReleaseUSBStroreW(USBDeviceList, Index);
                    ErrorCode = USB_STORE_ERROR_SETUP_DI_GET_DEVICE_REGISTRY_PROPERTY;
                    LocalFreeIf(pDeviceInterfaceDetailData);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    return FALSE;
                }
            }

            len = wcslen( lpBuffer );
            USBDeviceList[Index].DevicePhysicalName = (WCHAR*)LocalAlloc(LPTR, (len + 1) * sizeof(WCHAR));
            wcscpy(USBDeviceList[Index].DevicePhysicalName, lpBuffer);

            ++Index;
        }


        LocalFreeIf(lpBuffer);
        LocalFreeIf(pDeviceInterfaceDetailData);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);

    if(RequiredSize != NULL){
            if( Index == 0 ){
                ErrorCode = USB_STORE_NO_MORE_DEVICE;
                return FALSE;
            }

            *RequiredSize = Index;
    }

    if( USBDeviceListDataSize == 0 || USBDeviceListDataSize < Index || USBDeviceList == NULL)
    {
        ErrorCode = USB_STORE_ERROR_INSUFFICIENT_BUFFER;
        return FALSE;
    }

    ErrorCode = USB_STORE_NO_ERROR;
    return TRUE;
}

VOID WINAPI ReleaseUSBStroreA(
    __in_ecount(USBDeviceListDataSize)PUSB_DEVICE_STRORE_A USBDeviceList,
    __in DWORD USBDeviceListDataSize
    )
{
    if(USBDeviceList == NULL) return;

    for(DWORD Index = 0; Index < USBDeviceListDataSize; ++Index)
    {
        LocalFreeIf(USBDeviceList[Index].DeviceVolumePath);
        LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
        LocalFreeIf(USBDeviceList[Index].DevicePhysicalName);
        LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
    }
}

VOID WINAPI ReleaseUSBStroreW(
    __in_ecount(USBDeviceListDataSize)PUSB_DEVICE_STRORE_W USBDeviceList,
    __in DWORD USBDeviceListDataSize
    )
{
    if(USBDeviceList == NULL) return;

    for(DWORD Index = 0; Index < USBDeviceListDataSize; ++Index)
    {
        LocalFreeIf(USBDeviceList[Index].DeviceVolumePath);
        LocalFreeIf(USBDeviceList[Index].DeviceInterfacePath);
        LocalFreeIf(USBDeviceList[Index].DevicePhysicalName);
        LocalFreeIf(USBDeviceList[Index].DeviceVolumeName);
    }
}

DWORD WINAPI GetLastErrorUSBStore()
{
    return ErrorCode;
}


#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif
