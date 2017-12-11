obj-m = usb.o
KERN_SRC := /lib/modules/`uname -r`/build
PWD 	:= $(shell pwd)

module	:
	make -C $(KERN_SRC) M=$(PWD) modules
install:
	make -C $(KERN_SRC) M=$(PWD) install_modules
	depmod -a
clean:
	make -C $(KERN_SRC) M=$(PWD) clean
