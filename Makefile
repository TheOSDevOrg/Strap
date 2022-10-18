SRC_DIR:=src
OBJ_DIR:=objs
OUT_DIR:=out

.DEFAULT_GOAL:=all

CXX=i686-elf-g++
CC=i686-elf-gcc
LD=i686-elf-ld
NASM=nasm
AS=i686-elf-as

NAME=Strap

CXXFLAGS=-fno-threadsafe-statics -Iinc -nostdlib -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers -fsanitize=undefined
CFLAGS=-fno-threadsafe-statics -Iinc -nostdlib -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers -fsanitize=undefined
NASMFLAGS=-felf32
ASFLAGS=--32 -nostdlib

LOADER_SRC=$(SRC_DIR)/boot/loader.cpp
LOADER_OUT=$(OBJ_DIR)/loader.a

CXX_SOURCES=$(shell find $(SRC_DIR) -name '*.cpp' -not -path '$(SRC_DIR)/boot/*')
C_SOURCES=$(shell find $(SRC_DIR) -name '*.c' -not -path '$(SRC_DIR)/boot/*')
AS_SOURCES=$(shell find $(SRC_DIR) -name '*.s' -not -path '$(SRC_DIR)/boot/*')
NASM_SOURCES=$(shell find $(SRC_DIR) -name '*.asm' -not -path '$(SRC_DIR)/boot/*')

CXX_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(CXX_SOURCES:.cpp=.cpp.o))
C_FILES_OUT =  $(subst $(SRC_DIR),$(OBJ_DIR),$(C_SOURCES:.c=.c.o))
AS_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(AS_SOURCES:.s=.s.o))
NASM_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(NASM_SOURCES:.asm=.asm.o))

LINK_SOURCES=$(shell find $(OBJ_DIR) -name '*.o' -not -path "initrd/*")

.PHONY: all

all: loader $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link grub run-kvm
bochs: loader $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link grub run-bochs
video: loadervid $(C_FILES_OUT) $(CXX_FILES_OUT) $(AS_FILES_OUT) $(NASM_FILES_OUT) link grub run-kvm

build: loader $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link grub

#as:
#	$(NASM) $(ASFLAGS) 'src/asm/idt_handlers.asm' -o 'src/asm/idt_handlers.o'
#	$(NASM) $(ASFLAGS) 'src/asm/gdt_helpers.asm' -o 'src/asm/gdt_helpers.o'

loader:
	@$(CXX) $(CXXFLAGS) -c $(LOADER_SRC) -o $(LOADER_OUT)
loadervid:
	@$(CXX) $(CXXFLAGS) -c $(LOADER_SRC) -o $(LOADER_OUT) -D'VBE_MODE'

link:
	@$(LD) -nostdlib -Tscripts/link.ld -o '$(OUT_DIR)/kernel.bin' '$(OBJ_DIR)/loader.a' $(LINK_SOURCES)
	@echo "LD:   $(LINK_SOURCES) -> $(OUT_DIR)/kernel.bin"

clean:
	@-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.bin') $(shell find . -name '*.iso') $(MODULES_OUTPUT)

clean_objects: clean_modules
	@-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.a')
clean_modules:
	@-rm -Rf $(MODULES_OUTPUT)

grub:
	@mkdir -p isoroot/boot/grub
	@mkdir -p isoroot/mods
#	mkdir -p isoroot/initrd
#	echo "test" >> isoroot/mods/test_txt.mod
	@cp $(OUT_DIR)/kernel.bin isoroot/boot
#	mv initrd.img isoroot/initrd
	@cp src/grub/grub.cfg isoroot/boot/grub
	@grub-mkrescue -o $(OUT_DIR)/$(NAME).iso isoroot -V "Commander"
	@rm -rf isoroot

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "G++:  $< -> $@"

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "GCC:  $< -> $@"	

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@
	@echo "AS:   $< -> $@"

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@$(NASM) $(NASMFLAGS) $< -o $@
	@echo "NASM: $< -> $@"

run:
	@qemu-system-i386 -cdrom $(OUT_DIR)/$(NAME).iso -serial stdio -vga std -no-reboot -no-shutdown -m 1G
run-kvm:
	@qemu-system-i386 -cdrom $(OUT_DIR)/$(NAME).iso -serial stdio -vga std -no-reboot -no-shutdown -m 1G -enable-kvm -cpu host
run-bochs:
	bochs -q -f .bochsrc 
test:
	@echo $(AS_FILES_OUT) $(NASM_FILES_OUT)