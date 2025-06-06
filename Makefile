# compilers
LD := ld # linker
CC := gcc # c compiler
CFLAGS := -g -std=c11 # c flags
CXX := g++ # c++ compiler
CXXFLAGS := -g -std=c++17 # c++ flags

build: fr4nken;
run: build
	./fr4nken
clean:
	rm -f src/*/*.o
valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./fr4nken

fr4nken: src/c/main.o src/c/object.o src/c/errorhandling.o src/c/string.o src/c/value.o src/c/init.o src/cpp/init.o src/cpp/list.o
	$(CXX) src/c/*.o src/cpp/*.o -o fr4nken
	

define c_source
	src/c/$(1).o: src/c/$(1).c;
		$(CC) src/c/$(1).c $(CFLAGS) -c -o src/c/$(1).o
endef

$(call c_source,main)
$(call c_source,object)
$(call c_source,errorhandling)
$(call c_source,string)
$(call c_source,value)
$(call c_source,init)

define cpp_source
	src/cpp/$(1).o: src/cpp/$(1).cpp;
		$(CXX) src/cpp/$(1).cpp $(CXXFLAGS) -c -o src/cpp/$(1).o
endef

$(call cpp_source,init)
$(call cpp_source,list)
