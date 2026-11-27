#include <string.h>
#include <zephyr/init.h>
#include <zephyr/linker/section_tags.h>
#include <zephyr/shell/shell_uart.h>

#if defined(CONFIG_SHELL_BACKEND_SERIAL_API_ASYNC)

BUILD_ASSERT(CONFIG_SHELL_BACKEND_SERIAL_ASYNC_RX_BUFFER_SIZE > 0 &&
	     CONFIG_SHELL_BACKEND_SERIAL_ASYNC_RX_BUFFER_SIZE < BIT(7),
	     "UART async RX helper requires buffer sizes from 1 to 127 bytes");

extern struct shell_transport shell_transport_uart;

/* STM32 DMA requires non-cacheable TX and RX storage. */
static uint8_t shell_dma_tx_buffer[CONFIG_SHELL_PRINTF_BUFF_SIZE] __nocache __aligned(4);
static struct shell_uart_async shell_dma_uart __nocache __aligned(4);
static struct shell_transport_api shell_dma_transport_api;
static const struct shell_transport_api *shell_uart_api;

static int shell_dma_write(const struct shell_transport *transport,
			   const void *data, size_t length, size_t *cnt)
{
	size_t chunk = MIN(length, sizeof(shell_dma_tx_buffer));

	memcpy(shell_dma_tx_buffer, data, chunk);
	return shell_uart_api->write(transport, shell_dma_tx_buffer, chunk, cnt);
}

static int shell_dma_init(void)
{
	memset(shell_dma_tx_buffer, 0, sizeof(shell_dma_tx_buffer));
	memset(&shell_dma_uart, 0, sizeof(shell_dma_uart));
	shell_transport_uart.ctx = &shell_dma_uart;
	shell_uart_api = shell_transport_uart.api;
	shell_dma_transport_api = *shell_uart_api;
	shell_dma_transport_api.write = shell_dma_write;
	shell_transport_uart.api = &shell_dma_transport_api;

	return 0;
}

SYS_INIT(shell_dma_init, PRE_KERNEL_1, 0);

#endif
