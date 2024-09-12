OUT = main.out
SRC = main.cpp
HEAD = helpers.hpp tokenizer.hpp node.hpp wizparse.base.hpp wizparse.class.hpp wizparse.expr.hpp wizrun.hpp
UNIT = unittest.hpp 01_basic/test01.hpp 02_maths/test02.hpp 03_arrays/test03.hpp

$(OUT): $(addprefix src/,$(SRC) $(HEAD)) $(addprefix test/,$(UNIT))
	g++ -std=c++14 -Wall -o $(OUT) $(addprefix src/,$(SRC))

run: $(OUT)
	./$(OUT)
