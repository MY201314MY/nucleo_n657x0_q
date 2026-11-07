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

	LOG_INF("qualifiers: %s", CONFIG_BOARD_QUALIFIERS);
	LOG_INF("board: %s", CONFIG_BOARD);
	LOG_INF("frequency: %d MHz", CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC/1000000);

	return 0;
}
