OS_TYPE := $(shell uname -s)
BASE_DIR = ./
OBJ_DIR = ./build
BUILD_DIR = ./bin
X264_DIR = ../x264_pkg
WRAPPER_DIR = ./wrapper
TARGET = $(BUILD_DIR)/x264_encoder.dvcp
CPP = g++
CFLAGS = -O3 -Iinclude -Iwrapper -I$(X264_DIR)/include -Wall -Wno-unused-variable -fPIC -Wno-multichar -std=c++20
HEADERS = plugin.h uisettings_controller.h x264_encoder.h x264_encoder_main.h x264_encoder_high.h x264_encoder_h422.h
SRCS = plugin.cpp uisettings_controller.cpp x264_encoder.cpp x264_encoder_main.cpp x264_encoder_high.cpp x264_encoder_h422.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

ifeq ($(OS_TYPE), Linux)
LDFLAGS = -fPIC -shared -lpthread -Wl,-Bsymbolic -Wl,--no-undefined -static-libstdc++ -static-libgcc -std=c++20 -lstdc++
else
LDFLAGS = -dynamiclib
endif

LDFLAGS += -L$(X264_DIR)/lib -lx264 

.PHONY: all

all: prereq make-subdirs $(HEADERS) $(SRCS) $(OBJS) $(TARGET)

prereq:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

$(TARGET):
	$(CPP) $(WRAPPER_DIR)/build/*.o $(OBJ_DIR)/*.o $(LDFLAGS) -o $(TARGET)

clean: clean-subdirs
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)

make-subdirs:
	(cd wrapper; make; cd ..)

clean-subdirs:
	(cd wrapper; make clean; cd ..)
