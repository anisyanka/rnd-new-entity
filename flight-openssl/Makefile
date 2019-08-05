.PHONY: all clean install uninstall directories

# executable output
EXE_FILE        = crypto-flight

# compiler and flags
CC              = gcc
CFLAGS          = -Wall
LDFLAG          = 

# linked libraries
LIBS            = -lssl -lcrypto

# sources and includes
INCLUDES        = -I./inc/
INCLUDES        += -I./thirdparty/json/
INCLUDES        += -I/usr/include/openssl/

C_SRC           = $(wildcard ./src/*.c)
C_SRC           += $(wildcard ./thirdparty/json/*.c)

OBJ_DIR         = obj
OBJ_PATH        = $(OBJ_DIR)/*.o
OBJECTS         = $(patsubst %.c, %.o, $(C_SRC))

DEP_DIR         = dep
DEPENDENCIES    = $(OBJECTS:.o=.d)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------
all: $(EXE_FILE)

# link
$(EXE_FILE): $(OBJECTS) directories
	$(CC) $(LDFLAG) $(OBJ_PATH) $(LIBS) -o $(EXE_FILE)

# pull in dependency info for *existing* .o files
-include $(DEPENDENCIES)

# compile
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $*.c -o $*.o
	$(CC) $(CFLAGS) $(INCLUDES) -MM $*.c > $*.d

# mv objects to obj/ directory
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	@mv -f $(OBJECTS) -t $(OBJ_DIR)/
	@mv -f $(DEPENDENCIES) -t $(DEP_DIR)/

# remove compilation products
clean:
	rm -rf $(OBJ_DIR)/
	rm -rf $(DEP_DIR)/
	rm -f $(EXE_FILE)

install:
uninstall:
