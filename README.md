# EFI Chainloader

I wrote this because it's annoying to boot into EFI Shell every time I want to load a driver and then rerun the same commands every time.

Yes, you can use BCFG to add it as a boot option and then F12 into the boot menu, load it, then select Windows Boot Manager. That option wasn't good enough for me, so I wrote an EFI Application that will do it for me. Could be useful for lazy ones like me or simply for beginners.

It works by getting the path to the driver you want to load then calls LoadImage and StartImage for both drivers one at a time.

## Usage

Modify the `YOUR_DRIVER_PATH` structure in UefiMain and compile it.

1. Get a USB
2. Format it to FAT32
3. [Compile the EDK2 Shell](https://github.com/tianocore/edk2/releases)
4. Set up the following Folder Structure

```bash
USB  
└── EFI  
└──── Boot  
└──────bootx64.efi  
```

Place the compiled `UefiApplication.efi` and the Target Driver you want to chainload in the USB and boot into it.

It should look like this:

![image](https://github.com/v3xat/chainloader/assets/168819146/da26e568-93ff-451f-b12b-6e18a8156b37)


From there copy the `chainload.efi` and the your driver into the EFI System Partition (ESP) which usually is FS1:/

```bash
fs0:
cp chainload.efi fs1:/chainload.efi
cp TARGET.efi fs1:/TARGET.efi
```

FS1 should look like this:

![image](https://github.com/v3xat/chainloader/assets/168819146/c10d25ef-7db4-48b8-a816-296ef098abb7)


After that, you just have to create a new boot entry:

```
bcfg boot add 0 fs1:/chainload.efi "chainloader"
bcfg boot dump
```

If the chainload boot option is 0, change it with `bcfg mv`.

**WARNING: DON'T ADD IT AS A DRIVER (`bcfg driver add ...`). IT WILL MAKE YOUR SYSTEM UNBOOTABLE, AND YOU WILL HAVE TO REMOVE YOUR DRIVE SO IT WILL EVEN START (happened to me).**

After this simply restart your PC and the driver should load

In this case im loading [SecureFakePkg.efi](https://www.unknowncheats.me/forum/anti-cheat-bypass/471614-securefakepkg-trick-windows-thinking-secure-boot.html)

![image](https://github.com/v3xat/chainloader/assets/168819146/65f76c11-1e7b-4a4b-a39b-5936f7745c61)
