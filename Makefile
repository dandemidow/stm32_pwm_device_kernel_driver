# SPDX-License-Identifier: GPL-2.0-only
servo-stm-y := \
	servo.o

obj-$(CONFIG_SERVO_STM_MG996) += servo-stm.o
