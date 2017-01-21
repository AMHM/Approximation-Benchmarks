FILE1 = sobel.c

CC = arm-linux-gnueabi-gcc
CC_FLAGS = -static
ISA_FILE = /home/amir/AMHM_Approximation_Benchmarks/sobel/m5op_arm.S

all: sobel

sobel: ${FILE1} Makefile
	$(CC)  $(CC_FLAGS) ${FILE1} -o sobel $(ISA_FILE)

clean:
	rm -rf sobel output*
