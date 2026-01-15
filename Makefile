CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
PKG_LIBS = $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null)
LDFLAGS = $(if $(PKG_LIBS),$(PKG_LIBS),$(SFML_LIBS))

all: pong

pong: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f pong main.o
