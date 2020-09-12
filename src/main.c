#include <zephyr.h>
#include <drivers/uart.h>
#include <stdio.h>

static uint8_t uart_buf[1024];

void uart_cb(struct device *x, void *user_data)
{
	uart_irq_update(x);
	int data_length = 0;

	if (uart_irq_rx_ready(x)) {
		data_length = uart_fifo_read(x, uart_buf, sizeof(uart_buf));
		uart_buf[data_length] = 0;
	}
	printf("%s", uart_buf);
}

void main(void)
{
	printf("Hello World!\n");

	struct device *uart = device_get_binding("UART_1");

	uart_irq_callback_set(uart, uart_cb);
	uart_irq_rx_enable(uart);
	printf("UART loopback start!\n");
	while (1) {
		k_cpu_idle();
	}
}