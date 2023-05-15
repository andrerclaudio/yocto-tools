obj-m := state-ctrl.o

SRC := $(shell pwd)
LDFLAGS += -lgpiod
# KDIR = /lib/modules/$(shell uname -r)/build

all:
# $(MAKE) -C $(KDIR) $(KERNEL_SRC) M=$(SRC)
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
# $(MAKE) -C $(KDIR) $(KERNEL_SRC) M=$(SRC) modules_install
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
