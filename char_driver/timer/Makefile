INSTALL_DIR=modules
ifneq (${KERNELRELEASE},)
	obj-m := lkm.o

lkm-objs := init.o cleanup.o open_dev.o close_dev.o trim_dev.o write_dev.o create_scull.o read_dev.o llseek_dev.o scull_follow.o proc_ops.o seq_ops.o jit_func.o

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
	-rm -f trim_dev.o
	-rm -f write_dev.o
	-rm -f create_scull.o
	-rm -f read_dev.o
	-rm -f llseek_dev.o
	-rm -f scull_follow.o
	-rm -f proc_ops.o
	-rm -f seq_ops.o
	-rm -f jit_func.o
	-rm -rf modules
	-rm modules.order
	-rm Module.symvers
	-rm -rf .tmp_versions 
endif
