obj-m := misc_char_lkm.o
CFLAGS_misc_char_lkm.o := -DDEBUG

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
modules:
	make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	make -C $(KERNEL_DIR) M=`pwd` clean 
