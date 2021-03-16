CC = cl
PROJ_PATH = .\src
EXEC = so-cpp.exe
OBJECTS = main.obj hash_map.obj

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $**

main.obj: $(PROJ_PATH)\main.c
	$(CC) /c $**

hash_map.obj: $(PROJ_PATH)\hash_map.c $(PROJ_PATH)\hash_map.h
	$(CC) /c $**

clean:
	del *.obj *.exe
