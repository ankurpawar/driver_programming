INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := lkm.o

lkm-objs := init.o cleanup.o open_pipe.o close_pipe.o trim_pipe.o write_pipe.o create_pipe.o read_pipe.o

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
	-rm -f open_pipe.o
	-rm -f close_pipe.o
	-rm -f trim_pipe.o
	-rm -f write_pipe.o
	-rm -f create_pipe.o
	-rm -f read_pipe.o
	-rm -rf modules
	-rm modules.order
	-rm Module.symvers
endif
