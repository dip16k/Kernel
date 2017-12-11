obj-m := add.o mul.o calsi.o

KERN_SRC := /lib/modules/`uname -r`/build
PWD	:= $(shell pwd)

modules :
	make -C $(KERN_SRC) M=$(PWD) modules
install :
	make -C $(KERN_SRC) M=$(PWD) modules_install
	depmod -a
clean :
	make -C $(KERN_SRC) M=$(PWD) clean
