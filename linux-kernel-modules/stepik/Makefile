MODULE_NALE=solution

obj-m += $(MODULE_NALE).o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	rm -rf test
	sudo make -C /lib/modules/$(shell uname -r)/build  M=$(shell pwd) clean

load:
	sudo dmesg -c >> /dev/null
	sudo insmod $(MODULE_NALE).ko delays=150,500,850,50,1000,350,200 
	dmesg

unload:
	sudo dmesg -c >> /dev/null
	sudo rmmod $(MODULE_NALE).ko
	dmesg