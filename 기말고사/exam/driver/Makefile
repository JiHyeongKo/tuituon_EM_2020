obj-m:= exam_drv.o
KDIR:= ~/linux_kernel/
PWD:= $(shell pwd)
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	$(CROSS_COMPILE)gcc -o exam_test exam_test.c 
	
clean:
	-rm *.o *.mod.c .*.cmd modules.order Module.symvers *.ko
