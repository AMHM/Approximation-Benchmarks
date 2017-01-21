ARCH = arm
TARGET = ../bin/$(ARCH)/scale

ifeq ($(ARCH),x86)
        CXX = g++
        ISA_FILE = 
else
        CXX =arm-linux-gnueabi-gcc
        ISA_FILE = /home/amir/AMHM_Approximation_Benchmarks/Majid/scale/src/m5op_arm.S
endif

M4_BASE = .
MACROS  = c.m4.pthreads

CSRC	= image.c scale-bi.cpp scale-cbackend.cpp main.cpp

# Default build single precision version
NCO     = -DNCO=4

ifdef chk_err
ERR     = -DERR_CHK
endif

ifdef single
NCO = -DNCO=4
endif

ifdef size
SZ = -DN=$(size)
else
SZ = -DN=960
endif

ifdef double
NCO = -DNCO=2
endif

CXXFLAGS += $(NCO) $(FUNC) $(ERR) $(CSRC)

LDFLAGS= -static -lm
OPTFLAGS= -O0

all: $(TARGET)

$(TARGET): clean $(CSRC)
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET) $(ISA_FILE)

clean:
	rm -f $(TARGET)

