main: main.c
	arm-linux-androideabi-gcc -mandroid --sysroot=/opt/android-ndk/platforms/android-19/arch-arm -march=armv7-a -msoft-float -o test main.c
