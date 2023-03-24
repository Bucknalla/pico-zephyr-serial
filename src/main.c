/*
 * Written by Alex Bucknall.
 *
 * Copyright (c) 2022 Alex Bucknall. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the LICENSE file.
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <string.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>

/* Check overlay exists for CDC UART console */ 
BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
	    "Console device is not ACM CDC UART device");

void main(void)
{
	/* Configure to set Console output to USB Serial */ 
	const struct device *usb_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
	uint32_t dtr = 0;

    /* Check if USB can be initialised, bails out if fail is returned */
	if (usb_enable(NULL) != 0) {
		return;
	}

	/* Wait for a console connection, if the DTR flag was set to activate USB.
     * If you wish to start generating serial data immediately, you can simply
     * remove the while loop, to not wait until the control line is set.    
     */
	while (!dtr) {
		uart_line_ctrl_get(usb_device, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	}

	while (1) {
		printk("Hello from the Zephyr Console on the RPi Pico...\n");
		k_sleep(K_MSEC(5000));
	}
}