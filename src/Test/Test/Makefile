ARCH = arm
TARGET = ./bin/Test

ifeq ($(ARCH),x86)
        CXX = g++
        ISA_FILE = 
else
        CXX =arm-linux-gnueabi-g++
        ISA_FILE = /home/amir/AMHM_Approximation_Benchmarks/Test/Test/m5op_arm.S
endif

Test: Test.c Makefile
	$(CXX) -static -O0 -o $(TARGET) Test.c -lm $(ISA_FILE)

clean:
	rm -rf $(TARGET)

