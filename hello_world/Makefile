obj-m := hello_world.o
CFLAGS_hello_world.o := -DDEBUG

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
modules:
	make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	make -C $(KERNEL_DIR) M=`pwd` clean 
