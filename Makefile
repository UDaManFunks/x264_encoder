OS_TYPE := $(shell uname -s)
BASE_DIR = ./
OBJ_DIR = ./build
BUILD_DIR = ./bin
X264_DIR = ../x264_pkg
WRAPPER_DIR = ./wrapper
TARGET = $(BUILD_DIR)/x264_encoder.dvcp
CFLAGS = -O3 -Iinclude -Iwrapper -I$(X264_DIR)/include -Wall -Wno-unused-variable -Wno-multichar -fPIC -std=c++20
HEADERS = plugin.h x264_encoder.h x264_encoder_main.h x264_encoder_high.h x264_encoder_h422.h
SRCS = plugin.cpp x264_encoder.cpp x264_encoder_main.cpp x264_encoder_high.cpp x264_encoder_h422.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

ifeq ($(OS_TYPE), Linux)
LDFLAGS = -shared -lpthread -Wl,-Bsymbolic
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
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET):
	$(CC) $(WRAPPER_DIR)/build/*.o $(OBJ_DIR)/*.o $(LDFLAGS) -o $(TARGET)

clean: clean-subdirs
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)

make-subdirs:
	(cd wrapper; make; cd ..)

clean-subdirs:
	(cd wrapper; make clean; cd ..)
