# Makefile main 

CC = gcc
# CC=/home/vishs/Programs/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

CFLAGS = -Iinc/ -g -lpthread

# Note : Make sure only last file contains main()
# SRC_1 and OBJ_1 for building all the Publisher proc 
SRC_1 = src/TCP_lib.c src/SHM_lib.c src/string_lib.c src/DiscMsg_check.c src/GenDiscMsg.c src/publisher.c src/pub_node_main.c 
OBJ_1 = BIN/pub_node_1

# SRC_2 & OBJ_2 For Building all the TCP - Synchronizer Proc 
SRC_2 = src/TCP_lib.c src/SHM_lib.c src/string_lib.c src/DiscMsg_check.c src/synchronizer_main.c 
OBJ_2 = BIN/synchronizer_2

# SRC_3 & OBJ_3 For Building all the Subscriber proc 
SRC_3 = src/TCP_lib.c src/SHM_lib.c src/string_lib.c src/DiscMsg_check.c src/subscriber.c src/sub_node_main.c
OBJ_3 = BIN/sub_node_1
	
# SAMPLE and OBJ_4 for buliding any sample problems or Unit test codes 
# SAMPLE = src/TCP_lib.c src/SHM_lib.c src/GenDiscMsg.c src/sample_Gen_mesg.c
SAMPLE =  src/TCP_lib.c src/SHM_lib.c src/string_lib.c src/DiscMsg_check.c src/GenDiscMsg.c src_tests/publisher_new_imp.c
# SAMPLE = src_tests/publisher_new_imp.c 
OBJ_4 = src_tests/pub_node_sample

pub: 
	$(CC) $(CFLAGS) $(SRC_1) -o $(OBJ_1)

sync:
	$(CC) $(CFLAGS) $(SRC_2) -o $(OBJ_2) 

sub:
	$(CC) $(CFLAGS) $(SRC_3) -o $(OBJ_3) 

test:
	$(CC) $(CFLAGS) $(SAMPLE) -o $(OBJ_4)

all:
	$(CC) $(CFLAGS) $(SRC_1) -o $(OBJ_1)
	$(CC) $(CFLAGS) $(SRC_2) -o $(OBJ_2)
	$(CC) $(CFLAGS) $(SRC_3) -o $(OBJ_3)

clean_all:
	rm -f $(OBJ_1) && rm -f $(OBJ_2) && rm -f $(OBJ_3)