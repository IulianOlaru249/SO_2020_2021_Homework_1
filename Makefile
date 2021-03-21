CC = cl
PROJ_PATH = .\src
EXEC = so-cpp.exe
OBJECTS = hash_map.obj utils.obj main.obj
FLAGS = /MD

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(FLAGS) /Fe$@ $**

main.obj: $(PROJ_PATH)\main.c
	$(CC) $(FLAGS) /c $**

hash_map.obj: $(PROJ_PATH)\hash_map.c $(PROJ_PATH)\hash_map.h
	$(CC) $(FLAGS) /c $**

utils.obj: $(PROJ_PATH)\utils.c $(PROJ_PATH)\utils.h
	$(CC) $(FLAGS) /c $**

clean:
	del *.obj *.exe
