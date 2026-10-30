#!/usr/bin/env bash

cd "$(dirname "$0")"

# Verificar reglas udev para Picoprobe
if [ ! -f /etc/udev/rules.d/99-picoprobe.rules ]; then
    echo "[*] Creando reglas udev para Picoprobe (requiere sudo una vez)..."
    sudo tee /etc/udev/rules.d/99-picoprobe.rules > /dev/null << 'RULES'
# Raspberry Pi Pico Picoprobe
SUBSYSTEM=="usb", ATTR{idVendor}=="2e8a", ATTR{idProduct}=="0004", MODE="0666", GROUP="plugdev"
SUBSYSTEM=="usb", ATTR{idVendor}=="2e8a", ATTR{idProduct}=="000c", MODE="0666", GROUP="plugdev"
RULES
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    echo "[*] Agregando usuario a grupo plugdev..."
    sudo usermod -aG plugdev $USER
    echo "[!] Necesita salir/login o 'newgrp plugdev' para aplicar grupo"
    echo "[*] Ejecutando flasheo con permisos actuales..."
fi

echo "[*] Flasheando om13085_rgb.bin con Raspberry Pi Pico (Picoprobe)..."

openocd \
  -f interface/picoprobe.cfg \
  -c "transport select swd" \
  -c "adapter speed 2000" \
  -f target/lpc11xx.cfg \
  -c "init" \
  -c "reset halt" \
  -c "flash write_image erase om13085_rgb.bin 0x00000000" \
  -c "verify_image om13085_rgb.bin 0x00000000" \
  -c "reset run" \
  -c "shutdown"

echo "[✓] Listo"
