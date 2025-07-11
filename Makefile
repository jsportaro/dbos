all:
	nasm -f bin boot.asm -o boot.bin
	dd if=./Message.txt >> ./boot.bin
	dd if=/dev/zero bs=512 count=1 >> ./boot.bin