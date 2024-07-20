obj-m=pcd_device_setup.o pcd_platform_driver.o
BDIR=/lib/modules/6.5.0-28-generic/build

all:
	make -C ${BDIR} M=${PWD} modules

clean:
	make -C ${BDIR} M=${PWD} clean
