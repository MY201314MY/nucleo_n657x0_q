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

echo "[INFO] Entering debug mode..."
# BOOT0=0 (PA5) is already pulled low by default
# echo "operate pin PA5 0" > "$CONTROL_PORT"
echo "operate pin PA6 1" > "$CONTROL_PORT"

STM32_Programmer_CLI --power off index=0 --power on index=0

GDB_WRAPPER=$(mktemp)
trap 'rm -f "$GDB_WRAPPER"' EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM
cat > "$GDB_WRAPPER" <<'EOF'
#!/bin/sh
set -e
GDB=$(sed -n 's/^CMAKE_GDB:FILEPATH=//p' build/mcuboot/CMakeCache.txt)
if [ ! -x "$GDB" ]; then
    echo "[ERROR] GDB executable not found: $GDB" >&2
    exit 1
fi
rm -f "$0"
exec "$GDB" -q -iex "set pagination off" -iex "set confirm off" "$@" \
    -ex "tbreak do_boot" \
    -ex "continue" \
    -ex "add-symbol-file build/nucleo_n657x0_q/zephyr/zephyr.elf" \
    -ex "break src/main.c:main" \
    -ex "continue" \
    -ex "set confirm on"
EOF
chmod +x "$GDB_WRAPPER"

west debug --domain mcuboot --gdb "$GDB_WRAPPER"
