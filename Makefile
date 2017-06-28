CC := g++
FLAGS := -g -I  src -std=c++11

MAIN_SRC := src/main.cc
SRC := $(wildcard src/*.cc)
SRC := $(filter-out $(MAIN_SRC), $(SRC))


.PHONY:all test clean

all: 
	@echo $(SRC)
	$(CC) -o bin/monkey $(FLAGS) $(MAIN_SRC) $(SRC) 

test:
	@echo "tests for monkey"
	@mkdir -p bin
	$(CC) -o bin/test_lexer  $(FLAGS) tests/test_lexer.cc $(SRC)
	$(CC) -o bin/test_parser $(FLAGS) tests/test_parser.cc $(SRC)
	$(CC) -o bin/test_eval $(FLAGS) tests/test_eval.cc $(SRC)

clean:
	@rm -rf bin/
	

