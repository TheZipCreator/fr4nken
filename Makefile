# compilers
# C
CC := gcc
CFLAGS := -g -std=c11
# C++
CXX := g++
CXXFLAGS := -g -std=c++20
# Zig
ZIG := zig
ZIGFLAGS := -fPIC -fcompiler-rt
# Nim
NIMBLE := nimble
NIMFLAGS := --app:staticlib --debuginfo:on --dynlibOverride:libSDL2 --experimental:views
# Final linker flags
LDFLAGS := -lSDL2
# Linker
LD := g++

build: fr4nken;
run: build
	./fr4nken
clean:
	rm -f src/*/*.o
	rm -f src/*/*.a
valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./fr4nken
release: CFLAGS += -O3
release: CXXFLAGS += -O3
release: ZIGFLAGS += -O ReleaseFast
release: NIMFLAGS += -d:release
release: clean fr4nken;

fr4nken: \
	src/c/main.o src/c/object.o src/c/errorhandling.o src/c/string.o src/c/value.o src/c/init.o \
	src/cpp/init.o src/cpp/list.o src/cpp/map.o src/cpp/value.o \
	src/zig/libfr4nken.a \
	src/nim/libfr4nken.a
	$(LD) src/c/*.o src/cpp/*.o src/zig/libfr4nken.a src/nim/libfr4nken.a $(LDFLAGS) -o fr4nken
	

define c_source
	src/c/$(1).o: src/c/$(1).c;
		$$(CC) src/c/$(1).c $$(CFLAGS) -c -o src/c/$(1).o
endef

$(call c_source,main)
$(call c_source,object)
$(call c_source,errorhandling)
$(call c_source,string)
$(call c_source,value)
$(call c_source,init)


define cpp_source
	src/cpp/$(1).o: src/cpp/$(1).cpp;
		$$(CXX) src/cpp/$(1).cpp $$(CXXFLAGS) -c -o src/cpp/$(1).o
endef

$(call cpp_source,init)
$(call cpp_source,list)
$(call cpp_source,map)
$(call cpp_source,value)

src/zig/libfr4nken.a: $(wildcard src/zig/*.zig);
	cd src/zig; \
		$(ZIG) build-lib init.zig $(ZIGFLAGS) --name fr4nken -I.. -lc

src/nim/libfr4nken.a: $(wildcard src/nim/*.nim);
	cd src/nim; \
		$(NIMBLE) build $(NIMFLAGS); \
		mv fr4nken libfr4nken.a
