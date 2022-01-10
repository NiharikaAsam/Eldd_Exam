obj-m:= ques2.o

KDIR = /lib/modules/$(shell uname -r)/build

PWD = $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
	gcc write.c -o write
	gcc read.c -o read
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm write read
