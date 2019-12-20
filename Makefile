COMPILE=${CC} -std=c++17
ARCHIVE=ar rvs
DIR=build

cpp_lib: PureParser.a
	make cpp_compile
	make dir_clean

cpp_run: PureParserExamples
	$(DIR)/PureParserExamples
	make dir_clean

c_lib: pure_parser.a
	make c_compile
	make dir_clean

c_run: pure_parser_examples
	$(DIR)/pure_parser_examples
	make dir_clean

swift_run:
	swift run

dir_create:
	mkdir -p $(DIR)

dir_clean:
	rm $(DIR)/*.o

cpp_compile: PureParser.a
	cp cpp_src/PureParser.hpp $(DIR)

PureScanner.o: dir_create
	$(COMPILE) -c cpp_src/PureScanner.cpp -o $(DIR)/PureScanner.o

PureParser.o: dir_create
	$(COMPILE) -c cpp_src/PureParser.cpp -o $(DIR)/PureParser.o

PureParser.a: dir_create PureScanner.o PureParser.o
	$(ARCHIVE) $(DIR)/PureParser.a $(DIR)/PureScanner.o $(DIR)/PureParser.o

PureParserExamples: dir_create PureParser.a
	$(COMPILE) $(DIR)/PureParser.a cpp_src/PureParserExamples.cpp -o $(DIR)/PureParserExamples

c_compile: pure_parser.a
	cp c_wrapper/pure_parser.h $(DIR)

pure_parser.o: dir_create
	$(COMPILE) -c c_wrapper/pure_parser.cpp -o $(DIR)/pure_parser.o

pure_parser.a: dir_create PureScanner.o PureParser.o pure_parser.o
	$(ARCHIVE) $(DIR)/pure_parser.a $(DIR)/PureScanner.o $(DIR)/PureParser.o $(DIR)/pure_parser.o

pure_parser_examples: dir_create pure_parser.a
	$(COMPILE) $(DIR)/pure_parser.a c_wrapper/pure_parser_examples.c -o $(DIR)/pure_parser_examples
