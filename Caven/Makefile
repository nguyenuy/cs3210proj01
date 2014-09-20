ifneq ($(KERNELRELEASE),)
	#obj-m := encodeMorse.o
	obj-m := morse.o
	morse-objs := encodeMorse.o 
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#KERNELDIR ?= /home/nitin/Desktop/src/linux-3.11.1/

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


test: 
	insmod morse.ko
	clear
	cat test.txt | encodeMorse	
	rmmod morse
	sudo make clean
	echo "the result is in result.txt; see the encodeMorse script;"
	cat result.txt
print:
	cat /var/log/kern.log