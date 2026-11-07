set -e

set -- /dev/serial/by-id/*Zephyr_USB_CDC_ACM*-if00
if [ ! -e "$1" ]; then
    echo "[ERROR] No Zephyr USB CDC ACM control device found" >&2
    exit 1
fi
if [ "$#" -ne 1 ]; then
    echo "[ERROR] Multiple Zephyr USB CDC ACM devices found; connect only one control board" >&2
    exit 1
fi
CONTROL_PORT=$1
echo "[INFO] Using control device: $CONTROL_PORT"

echo "[INFO] Entering download mode..."
# BOOT0=0 (PA5) is already pulled low by default
# echo "operate pin PA5 0" > "$CONTROL_PORT"
echo "operate pin PA6 1" > "$CONTROL_PORT"

STM32_Programmer_CLI -c port=SWD mode=UR -hardRst

west flash

echo "[INFO] west flash completed successfully"

echo "[INFO] Entering XIP mode..."
echo "operate pin PA6 0" > "$CONTROL_PORT"

STM32_Programmer_CLI --power off index=0 --power on index=0
