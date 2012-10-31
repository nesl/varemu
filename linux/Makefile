BB=busybox-1.20.2
LINUX=linux-3.6.3
FS=fs

patches :
	cp -r $(LINUX)-variability/* $(LINUX)/

image:

	cp -r $(BB)/_install/* img/rootfs
	cp -r $(FS)/* img/rootfs/
	cd img/rootfs; find . | cpio -o --format=newc > ../rootfs.img
	gzip -c img/rootfs.img > img/rootfs.img.gz
	rm img/rootfs.img

	cp $(LINUX)/arch/arm/boot/zImage img/

