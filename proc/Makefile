# SPDX-License-Identifier: GPL-3.0-only
#
# Makefile for Procfile LKM
#

MNAME := proc
MDIR := $(if $(PWD),$(PWD),.)
KDIR := /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS += -D_USE_SEQ_OPS

obj-m += $(MNAME).o
$(MNAME)-y += module.o pops.o
ifeq ($(findstring -D_USE_SEQ_OPS,$(EXTRA_CFLAGS)), -D_USE_SEQ_OPS)
	$(MNAME)-y += sops.o
endif

.PHONY: all clean insmod modinfo rmmod dmesg

all:
	$(MAKE) -C $(KDIR) M=$(MDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(MDIR) clean

insmod:
	sudo insmod $(MDIR)/$(MNAME).ko

modinfo:
	modinfo $(MDIR)/$(MNAME).ko

rmmod:
	sudo rmmod $(MNAME)

dmesg:
	sudo dmesg | grep -E "^\[\s+?[0-9]+\.[0-9]+\] \[$(MNAME)\]" | tail -24
