/*
 * I2C bus driver for ADT7316/7/8 ADT7516/7/9 digital temperature
 * sensor, ADC and DAC
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/regmap.h>

#include "adt7316.h"

/*
 * device probe and remove
 */
static int adt7316_i2c_probe(struct i2c_client *client,
			     const struct i2c_device_id *id)
{
	struct regmap *regmap;

	regmap = devm_regmap_init_i2c(client, &adt7316_regmap_config);

	if (IS_ERR(regmap)) {
		dev_err(&client->dev, "Error initializing i2c regmap: %ld\n",
			PTR_ERR(regmap));
		return PTR_ERR(regmap);
	}

	return adt7316_probe(&client->dev, regmap, id ? id->name : NULL,
			     client->irq);
}

static const struct i2c_device_id adt7316_i2c_id[] = {
	{ "adt7316", 0 },
	{ "adt7317", 0 },
	{ "adt7318", 0 },
	{ "adt7516", 0 },
	{ "adt7517", 0 },
	{ "adt7519", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, adt7316_i2c_id);

static const struct of_device_id adt7316_of_match[] = {
	{ .compatible = "adi,adt7316" },
	{ .compatible = "adi,adt7317" },
	{ .compatible = "adi,adt7318" },
	{ .compatible = "adi,adt7516" },
	{ .compatible = "adi,adt7517" },
	{ .compatible = "adi,adt7519" },
	{ },
};

MODULE_DEVICE_TABLE(of, adt7316_of_match);

static struct i2c_driver adt7316_driver = {
	.driver = {
		.name = "adt7316",
		.of_match_table = adt7316_of_match,
		.pm = ADT7316_PM_OPS,
	},
	.probe = adt7316_i2c_probe,
	.id_table = adt7316_i2c_id,
};
module_i2c_driver(adt7316_driver);

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
MODULE_DESCRIPTION("I2C bus driver for Analog Devices ADT7316/7/9 and ADT7516/7/8 digital temperature sensor, ADC and DAC");
MODULE_LICENSE("GPL v2");
