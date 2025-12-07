#Compiler used
CC = g++

#Directories
SRC_DIR = src
BUILD_DIR = build

#Compiler variables
VARS = -DDEBUG

#Additional include paths
INCLUDE_PATHS = -I C:/Users/$(USERNAME)/Documents/Programming/mingw_dev_lib/include -I $(SRC_DIR)

#Additional library paths
LIBRARY_PATHS = -L C:/Users/$(USERNAME)/Documents/Programming/mingw_dev_lib/lib

#Compilation options
# -w suppresses all warnings
# -Wl, -subsystem, windows gets rid of the console window
COMPILER_FLAGS = -W -Wl,-subsystem,windows

#Libraries being linked against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#The exectuable name
EX_NAME = tetris.exe

#Object files
SOURCES = main.cpp game.cpp util.cpp states.cpp window.cpp renderer.cpp font.cpp \
audio.cpp gamepad.cpp texture.cpp key_layout.cpp particles.cpp shapes.cpp text.cpp \
textbox.cpp timer.cpp timed_media.cpp menu.cpp tetris_field.cpp tetrimino.cpp \
tetris_layout.cpp exceptions.cpp logger.cpp
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

#Pattern rule for building object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< ${VARS} $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) -o $@

#Dependencies
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/game.hpp \
$(SRC_DIR)/exceptions.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/game.o: $(SRC_DIR)/game.cpp $(SRC_DIR)/game.hpp $(SRC_DIR)/window.hpp \
$(SRC_DIR)/renderer.hpp $(SRC_DIR)/font.hpp $(SRC_DIR)/audio.hpp \
$(SRC_DIR)/gamepad.hpp $(SRC_DIR)/texture.hpp $(SRC_DIR)/key_layout.hpp \
$(SRC_DIR)/text.hpp $(SRC_DIR)/shapes.hpp $(SRC_DIR)/textbox.hpp $(SRC_DIR)/menu.hpp \
$(SRC_DIR)/states.hpp $(SRC_DIR)/util.hpp $(SRC_DIR)/constants.hpp \
$(SRC_DIR)/exceptions.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.cpp $(SRC_DIR)/util.hpp

$(BUILD_DIR)/states.o: $(SRC_DIR)/states.cpp $(SRC_DIR)/states.hpp \
$(SRC_DIR)/game.hpp $(SRC_DIR)/audio.hpp $(SRC_DIR)/texture.hpp $(SRC_DIR)/timer.hpp \
$(SRC_DIR)/menu.hpp $(SRC_DIR)/key_layout.hpp $(SRC_DIR)/tetris_layout.hpp \
$(SRC_DIR)/tetrimino.hpp $(SRC_DIR)/util.hpp $(SRC_DIR)/constants.hpp \
$(SRC_DIR)/exceptions.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/window.o: $(SRC_DIR)/window.cpp $(SRC_DIR)/window.hpp \
$(SRC_DIR)/game.hpp $(SRC_DIR)/key_layout.hpp $(SRC_DIR)/constants.hpp \
$(SRC_DIR)/exceptions.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/renderer.o: $(SRC_DIR)/renderer.cpp $(SRC_DIR)/renderer.hpp \
$(SRC_DIR)/window.hpp $(SRC_DIR)/util.hpp $(SRC_DIR)/exceptions.hpp \
$(SRC_DIR)/logger.hpp

$(BUILD_DIR)/font.o: $(SRC_DIR)/font.cpp $(SRC_DIR)/font.hpp $(SRC_DIR)/util.hpp \
$(SRC_DIR)/exceptions.hpp

$(BUILD_DIR)/audio.o: $(SRC_DIR)/audio.cpp $(SRC_DIR)/audio.hpp

$(BUILD_DIR)/gamepad.o: $(SRC_DIR)/gamepad.cpp $(SRC_DIR)/gamepad.hpp \
$(SRC_DIR)/game.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/texture.o: $(SRC_DIR)/texture.cpp $(SRC_DIR)/texture.hpp \
$(SRC_DIR)/renderer.hpp $(SRC_DIR)/font.hpp $(SRC_DIR)/util.hpp \
$(SRC_DIR)/exceptions.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/key_layout.o: $(SRC_DIR)/key_layout.cpp $(SRC_DIR)/key_layout.hpp \
$(SRC_DIR)/game.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/particles.o: $(SRC_DIR)/particles.cpp $(SRC_DIR)/particles.hpp \
$(SRC_DIR)/texture.hpp $(SRC_DIR)/util.hpp $(SRC_DIR)/constants.hpp

$(BUILD_DIR)/shapes.o: $(SRC_DIR)/shapes.cpp $(SRC_DIR)/shapes.hpp \
$(SRC_DIR)/renderer.hpp $(SRC_DIR)/util.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/text.o: $(SRC_DIR)/text.cpp $(SRC_DIR)/text.hpp $(SRC_DIR)/texture.hpp

$(BUILD_DIR)/textbox.o: $(SRC_DIR)/textbox.cpp $(SRC_DIR)/textbox.hpp \
$(SRC_DIR)/texture.hpp $(SRC_DIR)/shapes.hpp $(SRC_DIR)/text.hpp $(SRC_DIR)/util.hpp \
$(SRC_DIR)/logger.hpp

$(BUILD_DIR)/timer.o: $(SRC_DIR)/timer.cpp $(SRC_DIR)/timer.hpp

$(BUILD_DIR)/timed_media.o: $(SRC_DIR)/timed_media.cpp $(SRC_DIR)/timed_media.hpp \
$(SRC_DIR)/text.hpp $(SRC_DIR)/timer.hpp

$(BUILD_DIR)/menu.o: $(SRC_DIR)/menu.cpp $(SRC_DIR)/menu.hpp $(SRC_DIR)/game.hpp \
$(SRC_DIR)/audio.hpp $(SRC_DIR)/textbox.hpp $(SRC_DIR)/util.hpp

$(BUILD_DIR)/tetris_field.o: $(SRC_DIR)/tetris_field.cpp $(SRC_DIR)/tetris_field.hpp \
$(SRC_DIR)/texture.hpp

$(BUILD_DIR)/tetrimino.o: $(SRC_DIR)/tetrimino.cpp $(SRC_DIR)/tetrimino.hpp \
$(SRC_DIR)/tetris_field.hpp $(SRC_DIR)/texture.hpp $(SRC_DIR)/game.hpp \
$(SRC_DIR)/audio.hpp $(SRC_DIR)/key_layout.hpp

$(BUILD_DIR)/tetris_layout.o: $(SRC_DIR)/tetris_layout.cpp \
$(SRC_DIR)/tetris_layout.hpp $(SRC_DIR)/tetris_field.hpp $(SRC_DIR)/tetrimino.hpp \
$(SRC_DIR)/audio.hpp $(SRC_DIR)/texture.hpp $(SRC_DIR)/key_layout.hpp \
$(SRC_DIR)/timer.hpp $(SRC_DIR)/timed_media.hpp $(SRC_DIR)/logger.hpp

$(BUILD_DIR)/exceptions.o: $(SRC_DIR)/exceptions.cpp $(SRC_DIR)/exceptions.hpp

$(BUILD_DIR)/logger.o: $(SRC_DIR)/logger.cpp $(SRC_DIR)/logger.hpp

#Targets
all: $(EX_NAME)
run: $(EX_NAME)
	./$(EX_NAME)
clean:
	rm -rf $(BUILD_DIR)

$(EX_NAME): $(OBJECTS)
	$(CC) $^ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@
