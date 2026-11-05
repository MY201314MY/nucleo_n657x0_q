/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	int blink_init(void);

	blink_init();

	LOG_INF("Hello World! %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
