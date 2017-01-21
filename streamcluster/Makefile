FILE1 = streamcluster.cpp parsec_barrier.cpp

CC = arm-linux-gnueabi-g++
CC_FLAGS = -static -DENABLE_THREADS
ISA_FILE = /home/amir/AMHM_Approximation_Benchmarks/streamcluster/m5op_arm.S
LDFLAGS := -lpthread

all: streamcluster

streamcluster: ${FILE1} Makefile
	      $(CC)  $(CC_FLAGS) ${FILE1} -o streamcluster $(ISA_FILE) $(LDFLAGS) 

clean:
	rm -rf streamcluster output*
