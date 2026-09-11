CC = clang
AR = ar

CFLAGS = -g -Wall -Wextra -std=c11 -Iinclude

PREFIX = /usr/local
INCLUDE_DIR = $(PREFIX)/include
LIB_DIR = $(PREFIX)/lib

all:
	mkdir -p build
	$(CC) $(CFLAGS) -c src/buffer/ring_buffer.c -o build/ring_buffer.o
	$(CC) $(CFLAGS) -c src/byteBitMap/byteBitMap.c -o build/byteBitMap.o
	$(CC) $(CFLAGS) -c src/container/array.c -o build/array.o
	$(CC) $(CFLAGS) -c src/controller/controller.c -o build/controller.o
	$(AR) rcs build/libfkae-shtae.a build/ring_buffer.o build/byteBitMap.o build/array.o build/controller.o

test: testRingBuffer testBitMap testArray testController

testRingBuffer: all
	$(CC) $(CFLAGS) tests/ring_buffer_test.c build/libfkae-shtae.a -o build/ring_buffer_test
	./build/ring_buffer_test

testBitMap: all
	$(CC) $(CFLAGS) tests/byteBitMapTest.c build/libfkae-shtae.a -o build/byte_bitmap_test
	./build/byte_bitmap_test

testArray: all
	$(CC) $(CFLAGS) tests/array_test.c build/libfkae-shtae.a -o build/array_test
	./build/array_test


testController: all
	$(CC) $(CFLAGS) tests/controller_test.c build/libfkae-shtae.a -o build/controller_test
	./build/controller_test

install: all
	mkdir -p $(INCLUDE_DIR)/libfkae-shtae
	mkdir -p $(LIB_DIR)
	cp -R include/libfkae-shtae/* $(INCLUDE_DIR)/libfkae-shtae/
	cp build/libfkae-shtae.a $(LIB_DIR)/

uninstall:
	rm -rf $(INCLUDE_DIR)/libfkae-shtae
	rm -f $(LIB_DIR)/libfkae-shtae.a

clean:
	rm -rf build

.PHONY: all test testRingBuffer testBitMap install uninstall clean
