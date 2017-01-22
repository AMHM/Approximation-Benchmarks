FILE = image.c scale-bi.cpp scale-cbackend.cpp main.cpp

CC_ARM = arm-linux-gnueabi-g++
CC_FLAGS_ARM = -static -DAMHM_APPROXIMATION -DERR_CHK -DNCO=4 -O0
ISA_FILE_ARM = ../../shared_lib/m5op_arm.S
LDFLAGS_ARM = -lpthread

CC = g++
CC_FLAGS = -static -DERR_CHK -DNCO=4 -O0
LDFLAGS = -lpthread -lm

BINDIR = ../../bin

all: arm x86

arm:
	$(CC_ARM)  $(CC_FLAGS_ARM) $(FILE) -o $(BINDIR)/arm/scale $(ISA_FILE_ARM) $(LDFLAGS_ARM)
	
x86:
	$(CC)  $(CC_FLAGS) $(FILE) -o $(BINDIR)/x86/scale $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/arm/scale output*
	rm -rf $(BINDIR)/x86/scale output*
