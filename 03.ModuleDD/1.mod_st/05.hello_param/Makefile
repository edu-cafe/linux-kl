obj-m 	:= hello_param.o

KDIR	:= /lib/modules/$(shell uname -r)/build
#PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$$PWD clean
