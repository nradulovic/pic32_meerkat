/*
 * app_pdetector.c
 * This file is part of vacuum tester
 *
 * Copyright (C) 2014 - Nenad Radulović
 *
 * vacuum tester is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * vacuum tester is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vacuum tester. If not, see <http://www.gnu.org/licenses/>.
 */

#include "app_pdetector.h"
#include "driver/gpio.h"
#include "vtimer/vtimer.h"
#include "eds/epa.h"
#include "events.h"

#define CONFIG_PDETECTOR_PORT           CONFIG_PTT_PORT
#define CONFIG_PDETECTOR_PIN            CONFIG_PTT_PIN
#define CONFIG_TIMEOUT_MS               20
#define CONFIG_CONSUMER                 Control

static const ES_MODULE_INFO_CREATE("pdetector", "Porator detector", "Nenad Radulovoc");

static esVTimer timeout;

static void timeout_handler(void * arg)
{
    esEvent *           notify;
    esError             error;

    (void)arg;
    if (gpioRead(CONFIG_PDETECTOR_PORT) & (0x1u << CONFIG_PDETECTOR_PIN)) {
        ES_ENSURE(error = esEventCreateI(sizeof(esEvent), EVT_PDETECT_RELEASE, &notify));
    } else {
        ES_ENSURE(error = esEventCreateI(sizeof(esEvent), EVT_PDETECT_PRESS, &notify));
    }

    if (!error) {
        ES_ENSURE(esEpaSendEventI(CONFIG_CONSUMER, notify));
        gpioChangeEnableHandler(CONFIG_PDETECTOR_PORT);
    }
}

static void debounce_handler(void)
{
    gpioChangeDisableHandler(CONFIG_PDETECTOR_PORT);
    esVTimerStartI(&timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TIMEOUT_MS), timeout_handler, NULL);
}

void initPdetectorModule(void)
{
    esVTimerInit(&timeout);
    gpioSetAsInput(CONFIG_PDETECTOR_PORT, CONFIG_PDETECTOR_PIN);
    gpioChangeSetHandler(CONFIG_PDETECTOR_PORT, CONFIG_PDETECTOR_PIN, debounce_handler);
}

