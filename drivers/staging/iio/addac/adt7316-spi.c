/*
 * API bus driver for ADT7316/7/8 ADT7516/7/9 digital temperature
 * sensor, ADC and DAC
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>

#include "adt7316.h"

#define ADT7316_SPI_MAX_FREQ_HZ		5000000

/*
 * device probe and remove
 */
static int adt7316_spi_probe(struct spi_device *spi_dev)
{
	struct regmap *regmap;

	/* don't exceed max specified SPI CLK frequency */
	if (spi_dev->max_speed_hz > ADT7316_SPI_MAX_FREQ_HZ) {
		dev_err(&spi_dev->dev, "SPI CLK %d Hz?\n",
			spi_dev->max_speed_hz);
		return -EINVAL;
	}

	regmap = devm_regmap_init_spi(spi_dev, &adt7316_regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(&spi_dev->dev, "Error initializing spi regmap: %ld\n",
			PTR_ERR(regmap));
		return PTR_ERR(regmap);
	}

	/* switch from default I2C protocol to SPI protocol */
	regmap_write(regmap, 0, 0);
	regmap_write(regmap, 0, 0);
	regmap_write(regmap, 0, 0);

	return adt7316_probe(&spi_dev->dev, regmap, spi_dev->modalias,
			     spi_dev->irq);
}

static const struct spi_device_id adt7316_spi_id[] = {
	{ "adt7316", 0 },
	{ "adt7317", 0 },
	{ "adt7318", 0 },
	{ "adt7516", 0 },
	{ "adt7517", 0 },
	{ "adt7519", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi, adt7316_spi_id);

static struct spi_driver adt7316_driver = {
	.driver = {
		.name = "adt7316",
		.pm = ADT7316_PM_OPS,
	},
	.probe = adt7316_spi_probe,
	.id_table = adt7316_spi_id,
};
module_spi_driver(adt7316_driver);

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
MODULE_DESCRIPTION("SPI bus driver for Analog Devices ADT7316/7/8 and ADT7516/7/9 digital temperature sensor, ADC and DAC");
MODULE_LICENSE("GPL v2");
