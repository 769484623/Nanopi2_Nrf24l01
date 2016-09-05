MODULE_NAME := NRF24l01
RESMAN_CORE_OBJS:=NRF24l01_Main.o nRF24L01.o
RESMAN_GLUE_OBJS:=GPIO.o
$(MODULE_NAME)-objs:=$(RESMAN_GLUE_OBJS) $(RESMAN_CORE_OBJS)
obj-m :=NRF24l01.o
KVER ?= $(shell uname -r)
#KDIR := /lib/modules/$(KVER)/build
KDIR := /home/lnstar/nanopi/linux-3.4.y
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
	rm -rf .*.cmd *.o *.mod.c .tmp_versions *.symvers *.order
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.symvers *.order
