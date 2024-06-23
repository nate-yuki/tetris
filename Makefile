#Compiler used
CC = g++

#Compiler variables
VARS = -DDEBUG

#Additional include paths
INCLUDE_PATHS = -I C:/Users/Nate/Documents/Programming/mingw_dev_lib/include

#Additional library paths
LIBRARY_PATHS = -L C:/Users/Nate/Documents/Programming/mingw_dev_lib/lib

#Compilation options
# -w suppresses all warnings
# -Wl, -subsystem, windows gets rid of the console window
COMPILER_FLAGS = -W# -Wl,-subsystem,windows

#Libraries being linked against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#The exectuable name
OBJ_NAME = tetris.exe

%.o: %.cpp
	$(CC) -c $< ${VARS} $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) \
	$(LINKER_FLAGS)
%.exe: main.o
	$(CC) $^ -o $@ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

#Targets
all: tetris.exe
run: tetris.exe
	./tetris.exe
clean:
	rm -f *.o all

tetris.exe: main.o game.o util.o states.o window.o renderer.o font.o texture.o \
particles.o shapes.o text.o textbox.o timer.o timed_media.o menu.o tetris_field.o \
tetrimino.o tetris_layout.o exceptions.o logger.o
main.o: main.cpp game.hpp exceptions.hpp logger.hpp
game.o: game.cpp game.hpp window.hpp renderer.hpp texture.hpp shapes.hpp states.hpp \
util.hpp constants.hpp exceptions.hpp logger.hpp
util.o: util.cpp util.hpp
states.o: states.cpp states.hpp game.hpp texture.hpp timer.hpp menu.hpp \
tetris_layout.hpp util.hpp constants.hpp logger.hpp
window.o: window.cpp window.hpp constants.hpp exceptions.hpp logger.hpp
renderer.o: renderer.cpp renderer.hpp window.hpp util.hpp exceptions.hpp logger.hpp
font.o: font.cpp font.hpp util.hpp exceptions.hpp
texture.o: texture.cpp texture.hpp renderer.hpp font.hpp util.hpp exceptions.hpp \
logger.hpp
particles.o: particles.cpp particles.hpp texture.hpp util.hpp constants.hpp
shapes.o: shapes.cpp shapes.hpp renderer.hpp util.hpp logger.hpp
text.o: text.cpp text.hpp texture.hpp
textbox.o: textbox.cpp textbox.hpp texture.hpp shapes.hpp text.hpp util.hpp logger.hpp
timer.o: timer.cpp timer.hpp
timed_media.o: timed_media.cpp timed_media.hpp text.hpp timer.hpp
menu.o: menu.cpp menu.hpp shapes.hpp textbox.hpp util.hpp
tetris_field.o: tetris_field.cpp tetris_field.hpp texture.hpp
tetrimino.o: tetrimino.cpp tetrimino.hpp tetris_field.hpp texture.hpp
exceptions.o: exceptions.cpp exceptions.hpp
logger.o: logger.cpp logger.hpp
tetris_layout.o: tetris_layout.cpp tetris_layout.hpp tetris_field.hpp tetrimino.hpp \
texture.hpp timer.hpp timed_media.hpp
