obj-m := simple_list.o
CFLAGS_simple_list.o := -DDEBUG

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
modules:
	make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	make -C $(KERNEL_DIR) M=`pwd` clean 
