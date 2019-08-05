.PHONY: all clean default install uninstall copy_obj

# common variables
PROG_DIR=.
OBJ_DIR=obj
EXE_FILE=crypto-flight

# compiler and its flags
CC=gcc
CFLAGS= -Wall
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

# --------
# targets
# --------
all: $(EXE_FILE)
default: $(EXE_FILE)

# link
$(EXE_FILE): $(OBJECTS) copy_obj
	$(CC) $(LDFLAG) $(OBJ_PATH) $(LIBS) -o $(EXE_FILE)

# compile
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# mv objects to obj/ directory
copy_obj:
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJECTS) $(OBJ_DIR)/

# remove compilation products
clean:
	rm -rf $(OBJ_DIR)/
	rm -f $(EXE_FILE)
