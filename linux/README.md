# Bluetooth
The ORICO BTA-508 is buggy as hell with the mainline kernel driver and firmware, so here are the workarounds I found that make the bluetooth adapter work more reliably

### bluetooth-driver
This driver works much better than the mainline one.

It comes from [this](https://aur.archlinux.org/packages/rtl8761usb-dkms) AUR, and I just modified it a bit.


### bluetooth-firmware-replacement
The bluetooth firmware that comes from [Realtek's Android kernel](https://github.com/Realtek-OpenSource/android_hardware_realtek/tree/rtk1395/bt/rtkbt/Firmware/BT) works better than the one that's in linux-firmware

The `run.sh` script should be run as root.