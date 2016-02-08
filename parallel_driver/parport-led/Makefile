INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := lkm.o

lkm-objs := parport_led.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules
	@rm -rf ${INSTALL_DIR}
	@mkdir ${INSTALL_DIR}
	@mv -f *.o *.ko *.mod.c .*.cmd ${INSTALL_DIR}
clean:
	-rm -rf ${INSTALL_DIR}
	-rm -f parport_led.o
	-rm -rf modules
	-rm modules.order
	-rm Module.symvers
	-rm -rf .tmp_versions
endif

