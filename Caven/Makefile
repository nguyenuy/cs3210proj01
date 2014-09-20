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


encode: #testing the encoding 
	insmod morse.ko
	clear
	sudo sh -c "cat test.txt | encodeMorse"
	rmmod morse
	sudo make clean

decode: #testing the decoding
	insmod morse.ko
	clear
	sudo sh -c "cat result.txt | decodeMorse"
	rmmod morse
	sudo make clean

print:
	cat /var/log/kern.log