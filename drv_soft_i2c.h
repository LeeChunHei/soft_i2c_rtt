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

#ifndef __DRV_SOFT_I2C__
#define __DRV_SOFT_I2C__

#include <rtthread.h>
#include <rtdevice.h>

#ifdef PKG_USING_SOFT_I2C
#include <rthw.h>

struct rt_soft_i2c_bus_device
{
    struct rt_i2c_bus_device i2c_bus;
    struct rt_i2c_bit_ops ops;
    rt_base_t scl;
    rt_base_t sda;
};
typedef struct rt_soft_i2c_bus_device *rt_soft_i2c_bus_device_t;

rt_soft_i2c_bus_device_t soft_i2c_user_init(const char* bus_name, rt_base_t scl, rt_base_t sda, rt_uint32_t baudrate);

#endif

#endif