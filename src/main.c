#include <zephyr.h>
#include <drivers/uart.h>
#include <stdio.h>
#include <string.h>
#include <dk_buttons_and_leds.h>

static uint8_t uart_buf[1024];
static uint8_t bmv_buf[1024];
static uint8_t prop_name[1024];
static uint8_t prop_value[1024];
static size_t recv_off;
static size_t bmv_off;

static struct k_delayed_work leds_update_work;

/* Interval in milliseconds between each time status LEDs are updated. */
#define LEDS_UPDATE_INTERVAL K_MSEC(500)

/**@brief Update LEDs state. */
static void leds_update(struct k_work *work)
{
	static bool led_on;
	led_on = !led_on;
	dk_set_led(DK_LED1, led_on);
	k_delayed_work_submit(&leds_update_work, LEDS_UPDATE_INTERVAL);
}
const struct uart_config uart_cfg = {
	.baudrate = 19200,
	.parity = UART_CFG_PARITY_NONE,
	.stop_bits = UART_CFG_STOP_BITS_1,
	.data_bits = UART_CFG_DATA_BITS_8,
	.flow_ctrl = UART_CFG_FLOW_CTRL_NONE};

void uart_cb(struct device *x, void *user_data)
{
	uart_irq_update(x);
	if (uart_irq_rx_ready(x))
	{
		int data_length = uart_fifo_read(x, (uint8_t *)&uart_buf + recv_off, sizeof(uart_buf) - recv_off);
		recv_off += data_length;
		if (uart_buf[recv_off - 1] == 0x0A) // last char of '\r\n'
		{
			// Handle current BMV value
			int tabPos = 0;
			for (size_t i = 0; i <= bmv_off; i++)
			{
				if (bmv_buf[i] == 0x09)
				{
					tabPos = i;
					break;
				}
			}
			if (tabPos > 0)
			{
				(void)memset(prop_name, 0, sizeof(prop_name));
				(void)memset(prop_value, 0, sizeof(prop_value));
				memcpy((uint8_t *)&prop_name, (uint8_t *)&bmv_buf, tabPos);
				memcpy((uint8_t *)&prop_value, (uint8_t *)&bmv_buf + tabPos + 1, bmv_off - tabPos);
				if (strcmp("PID", prop_name) == 0)
				{
					printk("==================\n");
				}
				printk("%s:\t%s\r\n", prop_name, prop_value);
			}

			// Reset BMV value buffer
			(void)memset(bmv_buf, 0, sizeof(bmv_buf));
			bmv_off = 0;
		}
		if (uart_buf[recv_off - 1] != 0x0A && uart_buf[recv_off - 1] != 0x0D)
		{
			// Copy input from UART to BMV buffer
			memcpy((uint8_t *)&bmv_buf + bmv_off, (uint8_t *)&uart_buf + recv_off - data_length, data_length);
			bmv_off += data_length;
		}
		if (uart_buf[recv_off - 1] == 0x0A) // last char of '\r\n'
		{
			// Reset UART buffer size
			(void)memset(uart_buf, 0, sizeof(uart_buf));
			recv_off = 0;
		}
	}
}

static void work_init(void)
{
	k_delayed_work_init(&leds_update_work, leds_update);
	k_delayed_work_submit(&leds_update_work, LEDS_UPDATE_INTERVAL);
}

void main(void)
{
	printf("Hello World!\n");

	int err = dk_leds_init();
	if (err) {
		printf("ledError, error: %d", err);
		return;
	}

	work_init();

	struct device *uart = device_get_binding("UART_1");
	uart_configure(uart, &uart_cfg);
	uart_irq_callback_set(uart, uart_cb);
	uart_irq_rx_enable(uart);
	printf("UART loopback start!\n");
	while (1)
	{
		k_cpu_idle();
	}
}