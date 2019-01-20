obj-m := bug.o
KDIR  := /home/phil/vanilla/linux

modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(shell pwd) modules

clean:
	rm -f modules.order Module.symvers *.o *.ko *.mod.c
