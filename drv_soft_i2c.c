/*
 * Copyright (c) 2021 Lee Chun Hei, Leslie
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef PKG_USING_SOFT_I2C
#include "drv_soft_i2c.h"
#include "drv_gpio.h"

//#define DRV_DEBUG
#define LOG_TAG              "drv.softi2c"
#include <drv_log.h>

#ifdef PKG_SOFTI2C_USING_KCONFIG
    #define PKG_SOFTI2C_SCL      GET_PIN(PKG_SOFTI2C_SCL_GPIO, PKG_SOFTI2C_SCL_PIN)
    #define PKG_SOFTI2C_SDA      GET_PIN(PKG_SOFTI2C_SDA_GPIO, PKG_SOFTI2C_SDA_PIN)
#endif

static void soft_i2c_set_sda(void *data, rt_int32_t state)
{
    rt_soft_i2c_bus_device_t bus = (rt_soft_i2c_bus_device_t)data;
    rt_pin_write(bus->sda, state ? PIN_HIGH : PIN_LOW);
}

static void soft_i2c_set_scl(void *data, rt_int32_t state)
{
    rt_soft_i2c_bus_device_t bus = (rt_soft_i2c_bus_device_t)data;
    rt_pin_write(bus->scl, state ? PIN_HIGH : PIN_LOW);
}

static rt_bool_t soft_i2c_get_sda(void *data)
{
    rt_soft_i2c_bus_device_t bus = (rt_soft_i2c_bus_device_t)data;
    return rt_pin_read(bus->sda);
}

static rt_bool_t soft_i2c_get_scl(void *data)
{
    rt_soft_i2c_bus_device_t bus = (rt_soft_i2c_bus_device_t)data;
    return rt_pin_read(bus->scl);
}

static void soft_i2c_udelay(rt_uint32_t us)
{
#ifdef PKG_SOFTI2C_USE_HW_US_DELAY
    rt_hw_us_delay(us);
#else
    rt_tick_t tick;
    tick = RT_TICK_PER_SECOND * (us / 1000000);
    tick += (RT_TICK_PER_SECOND * (us % 1000000) + 999999) / 1000000;
    rt_thread_delay(tick);
#endif
}


static rt_err_t soft_i2c_bus_unlock(rt_soft_i2c_bus_device_t bus)
{
    if (PIN_LOW == rt_pin_read(bus->sda))
    {
        for (rt_uint8_t i = 0; i < 10; ++i)
        {
            rt_pin_write(bus->scl, PIN_HIGH);
            soft_i2c_udelay(100);
            rt_pin_write(bus->scl, PIN_LOW);
            soft_i2c_udelay(100);
        }
    }
    if (PIN_LOW == rt_pin_read(bus->sda))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

rt_soft_i2c_bus_device_t soft_i2c_user_init(const char* bus_name, rt_base_t scl, rt_base_t sda, rt_uint32_t baudrate)
{
    rt_pin_mode(scl, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(sda, PIN_MODE_OUTPUT_OD);

    rt_pin_write(scl, PIN_HIGH);
    rt_pin_write(sda, PIN_HIGH);

    rt_soft_i2c_bus_device_t dev_obj = rt_malloc(sizeof(struct rt_soft_i2c_bus_device));
	if (dev_obj)
	{
		rt_memset(dev_obj, 0x0, sizeof(struct rt_soft_i2c_bus_device));
        dev_obj->scl = scl;
        dev_obj->sda = sda;
        dev_obj->ops.data = dev_obj;
        dev_obj->ops.set_sda = soft_i2c_set_sda;
        dev_obj->ops.set_scl = soft_i2c_set_scl;
        dev_obj->ops.get_sda = soft_i2c_get_sda;
        dev_obj->ops.get_scl = soft_i2c_get_scl;
        dev_obj->ops.udelay = soft_i2c_udelay;
        dev_obj->ops.delay_us = 1000000 / (2 * baudrate);
        dev_obj->ops.timeout = 100;
        dev_obj->i2c_bus.priv = &dev_obj->ops;
        rt_err_t result;
        result = rt_i2c_bit_add_bus(&dev_obj->i2c_bus, bus_name);
        RT_ASSERT(result == RT_EOK);
        soft_i2c_bus_unlock(dev_obj);
        return dev_obj;
    }
    else
    {
        return RT_NULL;
    }
}

#ifdef PKG_SOFTI2C_USING_KCONFIG
static int soft_i2c_hw_init(void)
{
	if (soft_i2c_user_init(PKG_SOFTI2C_BUS_NAME, PKG_SOFTI2C_SCL, PKG_SOFTI2C_SDA, PKG_SOFTI2C_BAUDRATE) == RT_NULL)
	{
		return -RT_ERROR;
	}
    return RT_EOK;
}
INIT_DEVICE_EXPORT(soft_i2c_hw_init);
#endif

#endif