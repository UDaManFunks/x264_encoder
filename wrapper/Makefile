BASE_DIR = ../
OBJ_DIR = ./build
CFLAGS = -O3 -I../include -Wall -Wno-unused-variable -Wno-multichar -fPIC -std=c++20
HEADERS = host_api.h plugin_api.h
SRCS = host_api.cpp plugin_api.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
CXX = g++

.PHONY: all

all: prereq $(HEADERS) $(SRCS) $(OBJS)

prereq:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm $(OBJS)
	rmdir $(OBJ_DIR)
