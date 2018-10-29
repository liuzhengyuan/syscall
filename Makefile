ifneq ($(KERNELRELEASE),)
	obj-m := kylin_syscall.o
else
	PWD ?= $(shell pwd)
	KVER ?= $(shell uname -r)
	KDIR ?= /lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean: 
	rm -fr *.ko *.o *.mod* *.order *.symvers
endif
