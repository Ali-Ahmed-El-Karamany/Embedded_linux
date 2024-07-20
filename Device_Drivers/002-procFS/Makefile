obj-m=myproc.o
BDIR=/lib/modules/6.5.0-28-generic/build

all:
	make -C ${BDIR} M=${PWD} modules

clean:
	make -C ${BDIR} M=${PWD} clean