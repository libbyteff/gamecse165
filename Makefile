CC = g++ 

ifdef OS
   exe = elementalgame.exe
   comp = -Wall -DUSEGLEW game.cpp -lwinmm -lSDL -lSDL_mixer -lfreeglut -lglfw3 -lglew32 -lglu32 -lSOIL -lopengl32 -lm -o
else
   ifeq ($(shell uname), Linux)
      exe = elementalgame.exe  #edit exe personlize
	  comp = game.cpp -lglut -lGL -lGLEW -lGLU -lSDL -lSDL_mixer -lSOIL -o
   endif
endif

all:
	$(CC) $(comp) $(exe) 

debug: 
	g++ -g OpenGL.cpp -o broken

debugtest: 
	gdb broken

run: 
	./$(exe)

clean:
	rm -f $(exe)