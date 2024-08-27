OUT = main.out
SRC = main.cpp
HEAD = helpers.hpp tokenizer.hpp wizparse.base.hpp wizparse.class.hpp

$(OUT): $(addprefix src/,$(SRC) $(HEAD))
	g++ -std=c++14 -Wall -o $(OUT) $(addprefix src/,$(SRC))

run: $(OUT)
	./$(OUT)
