cat > /home/optimus/Documentos/lpcxpresso/flash_lpc1769.sh << 'EOF'
#!/usr/bin/env bash

cd "$(dirname "$0")"

if [ -z "$1" ]; then
    echo "[!] Falta el archivo .bin a flashear"
    echo "Uso: ./flash_lpc1769.sh archivo.bin"
    exit 1
fi

FIRMWARE="$1"

if [ ! -f "$FIRMWARE" ]; then
    echo "[!] No se encuentra el archivo: $FIRMWARE"
    exit 1
fi

echo "[*] Flasheando $FIRMWARE al LPC1769 con Raspberry Pi Pico (Picoprobe/CMSIS-DAP)..."

openocd \
  -f interface/cmsis-dap.cfg \
  -c "transport select swd" \
  -c "adapter speed 1000" \
  -f target/lpc17xx.cfg \
  -c "init" \
  -c "reset halt" \
  -c "flash write_image erase \"$FIRMWARE\" 0x00000000" \
  -c "reset run" \
  -c "shutdown"

echo "[✓] Listo - Checksum corregido por OpenOCD"
EOF
