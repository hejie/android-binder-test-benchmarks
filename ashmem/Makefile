TARGET	:= 	ashmem_my

ifneq ($(KERNELRELEASE),) 
obj-m	:=	$(TARGET).o	
$(TARGET)-y 	:= 	ashmem.o deps.o

$(obj)/deps.o: $(src)/deps.h
$(obj)/deps.h: $(src)/gen_deps.sh
	sudo $(src)/gen_deps.sh > $@
else

KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	-rm deps.h
install:	rm
	sudo cp $(TARGET).ko /lib/modules/`uname -r`/kernel/drivers/
	sudo insmod $(TARGET).ko
rm:
	- sudo rmmod $(TARGET).ko

endif
