
all: liblifelib.so

clean: 
	-rm liblifelib.so liblifelib.o

CXX?=clang++

info:
	@echo CXX=$(CXX)

liblifelib.so: liblifelib.o
	$(CXX) -shared -o $@ $^

liblifelib.o: lifelib.cpp automata.hpp rundemos.hpp
	$(CXX) -O3 -std=c++20 -Wall -Wextra -fPIC $(CXXFLAGS) -c -o $@ $< -lm


