FILES = ./build/kernel.asm.o ./build/kernel.o ./build/terminal.o ./build/idt.asm.o ./build/idt.o ./build/memory.o ./build/arch.o ./build/pic.o ./build/io.asm.o ./build/heap.o ./build/kheap.o
BINARY_SECTORS=100
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

.PHONEY: clean

# x86_64 Begin
all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=$(BINARY_SECTORS) >> ./bin/os.bin 

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/arch/x86_64/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/arch/x86_64/boot/boot.asm
	nasm -f bin ./src/arch/x86_64/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/arch/x86_64/kernel.asm
	nasm -f elf -g ./src/arch/x86_64/kernel.asm -o ./build/kernel.asm.o

./build/arch.o: ./src/arch/x86_64/arch.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/arch/x86_64/arch.c -o ./build/arch.o

./build/idt.asm.o: ./src/arch/x86_64/idt/idt.asm
	nasm -f elf -g ./src/arch/x86_64/idt/idt.asm -o ./build/idt.asm.o

./build/idt.o: ./src/arch/x86_64/idt/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/arch/x86_64/idt/idt.c -o ./build/idt.o

./build/terminal.o: ./src/arch/x86_64/support/terminal.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/arch/x86_64/support/terminal.c -o ./build/terminal.o

./build/io.asm.o: ./src/arch/x86_64/support/io.asm
	nasm -f elf -g ./src/arch/x86_64/support/io.asm -o ./build/io.asm.o

./build/pic.o: ./src/arch/x86_64/pic/pic.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/arch/x86_64/pic/pic.c -o ./build/pic.o
# x86_64 End

# Common Begin
./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory.o

./build/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/heap.o

./build/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/kheap.o
# Common End

clean:
	rm -rf ./bin/*.bin
	find ./build -name '*.o' -delete
	find ./src -name '*.o' -delete