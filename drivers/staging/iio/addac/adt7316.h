/*
 * ADT7316 digital temperature sensor driver supporting ADT7316/7/8 ADT7516/7/9
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _ADT7316_H_
#define _ADT7316_H_

#include <linux/types.h>
#include <linux/pm.h>
#include <linux/regmap.h>

#define ADT7316_REG_MAX_ADDR		0x3F

static const struct regmap_config adt7316_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

#ifdef CONFIG_PM_SLEEP
extern const struct dev_pm_ops adt7316_pm_ops;
#define ADT7316_PM_OPS (&adt7316_pm_ops)
#else
#define ADT7316_PM_OPS NULL
#endif
int adt7316_probe(struct device *dev, struct regmap *regmap,
		  const char *name, int irq);

#endif
