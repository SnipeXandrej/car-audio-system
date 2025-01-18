#!/bin/bash
#cp rtl8761b_fw /usr/lib/firmware/rtl_bt/rtl8761bu_fw.bin
#cp rtl8761b_config /usr/lib/firmware/rtl_bt/rtl8761bu_config.bin
#mv /usr/lib/firmware/rtl_bt/rtl8761bu_config.bin.zst rtl8761bu_config.bin.zst.original
#mv /usr/lib/firmware/rtl_bt/rtl8761bu_fw.bin.zst rtl8761bu_fw.bin.zst.original

# Firmware:
# wget "https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_fw"
# wget "https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_config"

mv /usr/lib/firmware/rtl8761bu_config rtl8761bu_config.original
mv /usr/lib/firmware/rtl8761bu_fw rtl8761bu_fw.original
cp rtl8761b_fw /usr/lib/firmware/rtl8761bu_fw
cp rtl8761b_config /usr/lib/firmware/rtl8761bu_config
