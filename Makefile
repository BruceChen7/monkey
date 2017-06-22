CC := g++
FLAGS := -g -I src -std=c++11

SRC := $(wildcard src/*.cc)

.PHONY:all test clean

all: 
	$(CC) -o bin/monkey $(FLAGS) $(SRC) 

test:
	@echo "tests for monkey"
	@mkdir -p bin
	$(CC) -o bin/test_lexer  $(FLAGS) tests/test_lexer.cc $(SRC)
	$(CC) -o bin/test_parser $(FLAGS) tests/test_parser.cc $(SRC)

clean:
	@rm -rf bin/
	

