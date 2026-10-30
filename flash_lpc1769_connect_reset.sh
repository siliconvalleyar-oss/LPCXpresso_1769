#!/usr/bin/env bash

cd "$(dirname "$0")"

if [ -z "$1" ]; then
    echo "[!] Falta el archivo .bin a flashear"
    echo "Uso: ./flash_lpc1769_connect_reset.sh archivo.bin"
    exit 1
fi

FIRMWARE="$1"

if [ ! -f "$FIRMWARE" ]; then
    echo "[!] No se encuentra el archivo: $FIRMWARE"
    exit 1
fi

echo "[*] Flasheando $FIRMWARE al LPC1769 con connect under reset (SRST)..."

openocd \
  -f interface/cmsis-dap.cfg \
  -c "transport select swd" \
  -c "adapter speed 50" \
  -f target/lpc17xx.cfg \
  -c "reset_config srst_only srst_nogate connect_assert_srst" \
  -c "adapter srst delay 100" \
  -c "init" \
  -c "halt" \
  -c "flash write_image erase \"$FIRMWARE\" 0x00000000" \
  -c "reset run" \
  -c "shutdown"

echo "[✓] Listo"
