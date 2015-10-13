INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := lkm.o

lkm-objs := init.o cleanup.o open_dev.o close_dev.o write_dev.o read_dev.o

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
	-rm -f init.o
	-rm -f cleanup.o
	-rm -f open_dev.o
	-rm -f close_dev.o
	-rm -f write_dev.o
	-rm -f read_dev.o
	-rm -rf modules
	-rm modules.order
	-rm Module.symvers
	-rm -rf .tmp_versions
endif

