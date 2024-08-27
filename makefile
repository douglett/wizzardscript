OUT = main.out
SRC = main.cpp
HEAD = helpers.hpp tokenizer.hpp wizparse.base.hpp

$(OUT): $(SRC) $(HEAD)
	g++ -std=c++14 -Wall -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)
