# export CONFIG_MODULE_SIG=n

CC = gcc
# CFLAGS += -I/usr/include
# LDFLAGS += -lgpiod

SRC := $(shell pwd)
KDIR = /lib/modules/$(shell uname -r)/build

obj-m := state-ctrl.o

all:
	$(MAKE) -C $(KDIR) $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KDIR) $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
