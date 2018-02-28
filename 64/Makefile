NDK=/home/tomzh/android-ndk-r10d
SYSROOT=$(NDK)/platforms/android-21/arch-arm64/
GCC=$(NDK)/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-gcc --sysroot=$(SYSROOT)

sdram_asm64: sdram_asm64.c
	$(GCC) -static -D__ASSEMBLY__ -O0 -o sdram_asm64 sdram_asm64.c

read64: read.c
	$(GCC) -static -D__ASSEMBLY__ -O0 -o read64 read.c

write64: write.c
	$(GCC) -static -D__ASSEMBLY__ -O0 -o write64 write.c
