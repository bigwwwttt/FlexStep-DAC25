riscv64-unknown-elf-gcc -fno-common -fno-builtin-printf -specs=htif_nano.specs -c test.c
riscv64-unknown-elf-gcc -static -specs=htif_nano.specs test.o -o test.riscv
riscv64-unknown-elf-objdump -d test.riscv >test.dump