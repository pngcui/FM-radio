/*
 *使用GPIO口模拟I2C
 *author @pngcui 20160414
 *
 *EXYNOS4212_GPJ1(2)----SDA----pin14-CAM_HREF
 *EXYNOS4212_GPJ0(2)----SCL----pin17-CAM_PCLK
 */
 
#include <linux/init.h>
#include <linux/module.h>

#define SDA EXYNOS4212_GPJ1(2)
#define SCL EXYNOS4212_GPJ0(2)

void IIC_start(void);

void IIC_stop(void);

void IIC_ack(void);

void IIC_nack(void);

unsigned char IIC_waitack(void);

unsigned char IIC_read8bit(unsigned char ack);

void IIC_write8bit(unsigned char addr);
