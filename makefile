OUT = main.out
SRC = main.cpp
HEAD = helpers.hpp tokenizer.hpp node.hpp wizparse.base.hpp wizparse.class.hpp wizparse.expr.hpp wizrun.hpp
UNIT = unittest.hpp test01.hpp

$(OUT): $(addprefix src/,$(SRC) $(HEAD)) $(addprefix test/,$(UNIT))
	g++ -std=c++14 -Wall -o $(OUT) $(addprefix src/,$(SRC))

run: $(OUT)
	./$(OUT)
