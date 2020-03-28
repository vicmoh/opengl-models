
all: compile run

# framework information for older version of MACOS
#INCLUDES = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# frameworks for newer MACOS, where include files are moved 
INCLUDES = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

TARGET = -mmacosx-version-min=10.8

SRC_DIR=./src/
INC_DIR=./include/
BIN_DIR=./bin/
TEST_DIR=./test/
LIB_DIR=./lib/include/

LIB=./lib/shared/*.so
INC=-I$(INC_DIR) -I$(LIB_DIR)
FLAGS=clang -Wno-nullability-completeness

FILE=''
EXEC_FILE=a4

packages:
	export LD_LIBRARY_PATH=./lib/shared:$$LD_LIBRARY_PATH

# Compile the C code.
compile: packages
	$(FLAGS) $(SRC_DIR)*.c $(INC) -o ./$(EXEC_FILE) $(INCLUDES) $(LIB) $(TARGET)

# Run the program.
run:
	./$(EXEC_FILE) $(FILE)

# Test the program.
sure: packages
	$(FLAGS) $(TEST_DIR)*.c $(INC) -o $(BIN_DIR)test $(LIB)
	$(BIN_DIR)test

clean:
	rm ./bin/*

# Quick git commit and push.
m=[AUTO]
git:
	git add -A
	git commit -m "$(m)"
	git push

portable-string:
	cp -rf ../portable-string/lib/include ./lib/
	cp -rf ../portable-string/lib/shared ./lib/
