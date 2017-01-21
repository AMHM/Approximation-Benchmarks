ARCH = arm
TARGET = ../bin/$(ARCH)/susan

ifeq ($(ARCH),x86)
        CXX = g++
        ISA_FILE = 
else
        CXX =arm-linux-gnueabi-gcc
        ISA_FILE = /home/amir/AMHM_Approximation_Benchmarks/Majid/susan/src/m5op_arm.S
endif

susan: susan.c Makefile
	$(CXX) -static -O0 -o $(TARGET) susan.c -lm $(ISA_FILE)

clean:
	rm -rf $(TARGET)

