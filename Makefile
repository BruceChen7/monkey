CC := g++
FLAGS := -g -I ./src -std=c++11

SRC := $(wildcard src/*.cc)

.PHONY:all test

all: 
	$(CC) -o monkey $(FLAGS) $(SRC) 

test:
	@echo "tests for monkey"

