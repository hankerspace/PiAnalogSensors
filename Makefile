DIR_BIN = ./bin

OBJ_C = $(wildcard *.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = main
CC = gcc

DEBUG = -g -O0 -Wall
CFLAGS += $(DEBUG) 

LIB = -lbcm2835 -lm

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@  $(LIB)

${DIR_BIN}/%.o : /%.c
	$(CC) $(CFLAGS) -c  $< -o $@  $(LIB)


	
clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 