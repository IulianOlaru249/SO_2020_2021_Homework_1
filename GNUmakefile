CC = gcc
CFLAGS = -Wall -g
PROJ_PATH = ./src
EXEC = so-cpp
OBJECTS = hash_map.o utils.o main.o

LDFLAGS = 

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

main.o: $(PROJ_PATH)/main.c
	$(CC) $(CFLAGS) $^ -c

hash_map.o: $(PROJ_PATH)/hash_map.c $(PROJ_PATH)/hash_map.h
	$(CC) $(CFLAGS) $^ -c

utils.o: $(PROJ_PATH)/utils.c $(PROJ_PATH)/utils.h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o so-cpp
