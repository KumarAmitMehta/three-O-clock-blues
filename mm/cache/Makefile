obj-m := simple_cache.o
CFLAGS_simple_cache.o := -DDEBUG

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
modules:
	make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	make -C $(KERNEL_DIR) M=`pwd` clean 
