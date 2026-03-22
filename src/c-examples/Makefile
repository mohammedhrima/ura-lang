CXX      = clang
CXXFLAGS = -std=c++17 -O2 -I/opt/homebrew/include
LDFLAGS  = -L/opt/homebrew/lib -lraylib \
           -framework CoreVideo -framework IOKit \
           -framework Cocoa -framework GLUT -framework OpenGL

tetris: game.cpp
	$(CXX) $(CXXFLAGS) game.cpp $(LDFLAGS) -o tetris

clean:
	rm -f tetris

.PHONY: clean
