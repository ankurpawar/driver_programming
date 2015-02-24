INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := hello_kobj.o

lkm-objs := hello_kobj.o

else
	KERNELRELEASE ?= /lib/modules/$(shell uname -r)/extra
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	-rm -f *.o *.ko *.mod.c .*.cmd
	-rm -rf modules.order
	-rm -rf Module.symvers
	-rm -rf .tmp_versions
endif

