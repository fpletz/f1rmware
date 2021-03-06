# Makefile to create filesystem image and flash rad10 badge

default: gather
test: build gather
	$(MAKE) UDISK=test image
all: build gather run

fsdir?=IMG
mountpoint?=/mnt
fsimg?=filesystem.img
hdrimg?=header.img
fullimg?=full.img
APP=../flashapp/flashapp.dfu
FILEORDER= \
    camp.b1n \
    hackrf.b1n \
    testapp.b1n

gather:
	@echo
	@echo '### Gathering contents for image'
	[ ! -d $(fsdir) ] && mkdir $(fsdir) || true
	cp ../hackrf/firmware/hackrf_usb/build/hackrf_usb.bin $(fsdir)/hackrf.b1n
	cp ../testapp/testapp.b1n $(fsdir)/
	cp ../campapp/camp.b1n $(fsdir)/
	-cp ../l0dables/*.c1d $(fsdir)/
	-for a in ../l0dables/*.n1k ; do b=$${a##*/}; cp $$a ${fsdir}/$${b#nick_} ; done
	cp ../hackrf/firmware/cpld/sgpio_if/default.xsvf $(fsdir)/cpld.xsv
	# Add f0nts, images, l0dables, etc.
	cp ../files/* $(fsdir)/
	echo -n "camp.b1n" > $(fsdir)/boot.cfg
	cp ../flashapp/flashapp.dfu .
	@echo
	@echo '### Creating boot header image'
	dd if=/dev/zero bs=1024 count=512 |tr "\000" "\377" > $(hdrimg)
	dd if=../bootloader/boot.b1n of=$(hdrimg) conv=notrunc

run:
	./FLASHgui

### Rebuild everything 
rebuild: build gather
build:
	@echo
	@echo '### Building everything'
#	cd ../hackrf/firmware/hackrf_usb/build && make
	cd ../hackrf && make lib hackrf-usb
	cd ../testapp && make testapp.b1n
	cd ../bootloader && make boot.b1n
	cd ../flashapp && make flashapp.dfu
	cd ../l0dables && make

dfu-util:
	cd ../../dfu-util && sh autogen.sh && ./configure && make

clean:
	$(RM) -rf $(fsdir).* $(fsdir)
	$(RM) $(fsimg).* $(hdrimg) $(fullimg).* flashapp.dfu


### Building individual images with different random data per badge
fsimage:
	$(RM) -rf $(fsimg).$(UDISK) $(fsdir).$(UDISK)
	mkdir $(fsdir).$(UDISK)
	cp -a $(fsdir)/* $(fsdir).$(UDISK)
ifneq ("$(wildcard privkey1.pem)","")
	dd if=/dev/urandom bs=1 count=64 of=$(fsdir).$(UDISK)/key1.bin 2>/dev/null
	openssl dgst -sha256 -sign privkey1.pem -out $(fsdir).$(UDISK)/key1.sha256 $(fsdir).$(UDISK)/key1.bin
	echo "KEY1" | cat - $(fsdir).$(UDISK)/key1.bin $(fsdir).$(UDISK)/key1.sha256 | qrencode -o $(fsdir).$(UDISK)/key1.png -l M -8 -s 2
	../tools/img2lcd.pl -b 8 $(fsdir).$(UDISK)/key1.png
	rm $(fsdir).$(UDISK)/key1.png
endif
	mkfs.fat -F 12 -I -S 512 -C $(fsimg).$(UDISK) 1536 >/dev/null
	mcopy -i $(fsimg).$(UDISK) `perl order-files $(fsdir).$(UDISK) $(FILEORDER)` ::

image: fsimage
	dd if=$(hdrimg) bs=1024 of=$(fullimg).$(UDISK) 2>/dev/null
	dd if=$(fsimg).$(UDISK) bs=1024 seek=512 of=$(fullimg).$(UDISK) 2>/dev/null


### Targets for FLASHgui
flash: image
	dd if=$(fullimg).$(UDISK) bs=512 of=/dev/$(UDISK) conv=fsync 2>/dev/null

dfu:
	dfu-util -p $(UPATH) --download $(APP) 2>&1 >/dev/null  | grep -v "unable to read DFU status after completion" || true

msc:
	mcopy -D o -i /dev/$(UDISK) `perl order-files $(fsdir) $(FILEORDER)` ::

sdr:
	osmocom_fft -s 10000000 -f 432000000 -g 30 --peak-hold
