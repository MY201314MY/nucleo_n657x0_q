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

echo "[INFO] Setting BOOT0=0 (PA5), BOOT1=1 (PA6)"
echo "operate pin PA6 1" > "$CONTROL_PORT"

STM32_Programmer_CLI -c port=SWD mode=UR -hardRst
echo "[INFO] STM32 Reset complete"

echo "[INFO] Starting west flash; waiting for completion"
west flash || {
    status=$?
    echo "[ERROR] west flash failed (exit code: $status); stopping" >&2
    exit "$status"
}
echo "[INFO] west flash completed successfully"

echo "[INFO] Setting BOOT1=0 (PA6); BOOT0 remains 0 (PA5)"
echo "operate pin PA6 0" > "$CONTROL_PORT"

echo "[INFO] Powering device off (PA7=0)"
echo "operate pin PA7 0" > "$CONTROL_PORT"
echo "[INFO] Waiting for 0.5 seconds before powering on"
sleep 0.5
echo "[INFO] Powering device on (PA7=1), BOOT0=0, BOOT1=0"
echo "operate pin PA7 1" > "$CONTROL_PORT"
