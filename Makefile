obj-m	: 1.o
KERNDIR:= /lib/modules/$(shell uname -r)/build/
PWD:=	$(shell pwd)
default	:
	make -C $(KERNDIR) M=$(PWD)  

install	:
	make -C $(KERNDIR) M=$(PWD) modules_install

clean	:
	make -C $(KERNDIR) M=$(PWD) clean

