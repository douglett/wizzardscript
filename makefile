OUT = main.out
SRC = main.cpp
HEAD = helpers.hpp tokenizer.hpp node.hpp wizparse.base.hpp wizparse.class.hpp wizrun.hpp

$(OUT): $(addprefix src/,$(SRC) $(HEAD))
	g++ -std=c++14 -Wall -o $(OUT) $(addprefix src/,$(SRC))

run: $(OUT)
	./$(OUT)
