.PHONY: all clean

INC=$(shell find . -name '*.h')
SRC=$(shell find . -name '*.c')
CCFLAGS= -O0 -g -DDBG_LOG_EN -DERR_LOG_EN

TARGET=xor_enc.a

all: $(INC)
	gcc $(CCFLAGS) $(SRC) -o $(TARGET)

clean: clean_test_file
	rm -rf $(TARGET) *.o

clean_test_file:
	rm -rf *.xored *.dexored