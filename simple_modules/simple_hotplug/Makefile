INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := simple_hotplug.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules_install
	@rm -rf ${INSTALL_DIR}
	@mkdir ${INSTALL_DIR}
	@mv -f *.o *.ko *.mod.c .*.cmd ${INSTALL_DIR}
clean:
	-rm -rf ${INSTALL_DIR}
	-rm -rf modules.order
	-rm -rf Module.symvers
	-rm -rf .tmp_versions
endif

