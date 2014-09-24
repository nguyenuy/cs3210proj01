ifneq ($(KERNELRELEASE),)
    obj-m += encodeMorse.o
    obj-m += decodeMorse.o
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
	insmod encodeMorse.ko
	clear
	sudo sh -c "cat test1.txt | encodeMorse > result1.txt"
	sudo sh -c "cat test2.txt | encodeMorse > result2.txt"
	rmmod encodeMorse.ko
	sudo make clean

decode: #testing the decoding
	insmod decodeMorse.ko
	clear
	sudo sh -c "cat result1.txt | decodeMorse"
	sudo sh -c "cat result2.txt | decodeMorse"
	rmmod decodeMorse.ko
	sudo make clean

print:
	cat /var/log/kern.log