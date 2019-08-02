# common variables
PROG_DIR=.
OBJ_DIR=obj
EXE_FILE=crypto-flight

# compiler and its flags
CC=gcc
CFLAGS= -Wall -c -static
LDFLAG=

# linked libraries
LIBS= -lssl -lcrypto -I/usr/include/openssl/

# path to all includes
INCLUDES= -I$(PROG_DIR)/inc/
INCLUDES += -I$(PROG_DIR)/thirdparty/json/

# find all sources
C_SRC=$(wildcard $(PROG_DIR)/src/*.c)
C_SRC += $(wildcard $(PROG_DIR)/thirdparty/json/*.c)

# do "*.o" files
OBJECTS=$(patsubst %.c, %.o, $(C_SRC))
OBJ_PATH=$(OBJ_DIR)/*.o

# targets
.PHONY: all
all: $(EXE_FILE)

.PHONY: default
default: $(EXE_FILE)

$(EXE_FILE): $(OBJECTS)
	$(CC) $(LDFLAG) $(OBJ_PATH) $(LIBS) -o $(EXE_FILE)

$(OBJECTS): $(C_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJECTS) $(OBJ_DIR)/

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/
	rm -f $(EXE_FILE)

.PHONY: install
install:

.PHONY: uninstall
uninstall:
