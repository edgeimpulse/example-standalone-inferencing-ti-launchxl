/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Include ----------------------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "ti/drivers/Timer.h"
#include <stdlib.h>
#include <stdio.h>
/* Driver configuration */
#include "ti_drivers_config.h"

/* Extern CPP functions ---------------------------------------------------- */
extern int ei_main();

/* Forward declerations ---------------------------------------------------- */
static int init_timer(void);

/* Private variables ------------------------------------------------------- */
static UART_Handle uart;
static uint64_t timer_count = 0;


/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{    
    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    if(init_timer()) {
        UART_write(uart, "Timer init error", 16);
    }

    ei_main();

    return NULL;
}

/**
 * @brief Write data to the serial output
 *
 * @param string
 * @param length
 */
void Serial_Out(char *string, int length)
{
    UART_write(uart, string, length);
}

/**
 * @brief Get current time in ms
 *
 * @return uint64_t
 */
uint64_t Timer_getMs(void)
{
    return timer_count;
}

/**
 * @brief Called by timer interrupt
 *
 * @param myHandle
 * @param status
 */
static void timer_Callback(Timer_Handle myHandle, int_fast16_t status)
{
    timer_count++;
}

/**
 * @brief Setup and start timer
 *
 * @return Error if unequal to 0
 */
static int init_timer(void)
{
    Timer_Handle timer_handle;
    Timer_Params timer_params;

    Timer_init();

    /* Create a timer that fire's the callback every 1000 microseconds */
    Timer_Params_init(&timer_params);
    timer_params.period = 1000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timer_Callback;

    timer_handle = Timer_open(CONFIG_TIMER_0, &timer_params);

    if(timer_handle == NULL) {
        return -1;
    }

    if(Timer_start(timer_handle) == Timer_STATUS_ERROR) {
        return -2;
    }
    else {
        return 0;
    }
}
