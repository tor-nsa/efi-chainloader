#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Shell.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi.h>
#include <Protocol/LoadedImage.h>

EFI_GUID gEfiChainloaderProtocolGuid = { 0xd487ddb4, 0x008b, 0x11d9, {0xaf, 0xdc, 0x00, 0x10, 0x83, 0xff, 0xca, 0x4d } };

extern const UINT32 _gUefiDriverRevision = 0;
CHAR8  *gEfiCallerBaseName = "chainloader";

/////////////////////////////////////////////////////////////////////////

typedef struct __chainload_info
{
    EFI_HANDLE image_handle;
    EFI_SYSTEM_TABLE* system_table;

    CHAR16* driver_path;
} __chainload_info ;

EFI_STATUS __cdecl initialize( __chainload_info* chainload )
{
    EFI_STATUS status;
    EFI_HANDLE loaded_image_handle;
    EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
    EFI_DEVICE_PATH* device_path;

    status = gBS->OpenProtocol( chainload->image_handle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image, chainload->image_handle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    if ( EFI_ERROR( status ) )
    {
        Print( L"[ * ] Couldn't get LoadedImageProtocol -> (%r)\n", status );
        return status;
    }

    device_path = FileDevicePath( loaded_image->DeviceHandle, chainload->driver_path );
    if ( device_path == NULL )
    {
        Print( L"[ * ] Unable to create device path\n" );
        return EFI_INVALID_PARAMETER;
    }

    status = gBS->LoadImage( FALSE, chainload->image_handle, device_path, NULL, 0, &loaded_image_handle );
    if ( EFI_ERROR( status ) )
    {
        Print( L"[ * ] Couldn't load driver -> (%r)\n", status );
        return status;
    }

    status = gBS->StartImage( loaded_image_handle, NULL, NULL );
    if ( EFI_ERROR( status ) )
    {
        Print( L"[ * ] Couldn't start driver -> (%r)\n", status );
        return status;
    }
}

EFI_STATUS __cdecl UefiUnload(IN EFI_HANDLE image_handle) {
    return EFI_SUCCESS;
}

EFI_STATUS __cdecl UefiMain( EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table )
{
    __chainload_info YOUR_DRIVER_PATH = {
            .image_handle = image_handle,
            .system_table = system_table,
            .driver_path = L"\\sfp.efi"
    };

    __chainload_info bootmgfw =
    {
        .image_handle = image_handle,
        .system_table = system_table,
        .driver_path = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"
    };

    initialize( &YOUR_DRIVER_PATH );
    initialize( &bootmgfw );

    return EFI_SUCCESS;
}