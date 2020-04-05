obj-m += RAM/201513700_ram.o
obj-m += CPU/201513700_cpu.o
obj-m += ARBOL/201513700_arbol.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

montar:
	dmesg -C
	insmod RAM/201513700_ram.ko
	insmod CPU/201513700_cpu.ko
	insmod ARBOL/201513700_arbol.ko
	dmesg

desmontar:
	dmesg -C
	rmmod RAM/201513700_ram.ko
	rmmod CPU/201513700_cpu.ko
	rmmod ARBOL/201513700_arbol.ko
	dmesg
