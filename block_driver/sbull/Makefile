ifneq ($(KERNELRELEASE),)
	obj-m := block_driver.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	-rm -f *.o *.ko *.mod.c .*.cmd
	-rm -rf modules.order
	-rm -rf Module.symvers
	-rm -rf .tmp_versions
endif	
	
