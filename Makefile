CC = cl
PROJ_PATH = .\src
EXEC = so-cpp.exe
OBJECTS = hash_map.obj utils.obj main.obj

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) /Fe$@ $**

main.obj: $(PROJ_PATH)\main.c
	$(CC) /c $**

hash_map.obj: $(PROJ_PATH)\hash_map.c $(PROJ_PATH)\hash_map.h
	$(CC) /c $**

utils.obj: $(PROJ_PATH)\utils.c $(PROJ_PATH)\utils.h
	$(CC) /c $**

clean:
	del *.obj *.exe
