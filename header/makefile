INCLUDE_DIR := $(shell llvm-config --includedir)
LIBRARY_DIR := $(shell llvm-config --libdir)

CXXFLAGS := -c -I$(INCLUDE_DIR) -march=native -O1 -frtti
CXX      := clang++ $(CXXFLAGS)
LDFLAGS  := -s -L$(LIBRARY_DIR) -L. -Wl,-O1,--as-needed
LD       := clang++ $(LDFLAGS)
MXXFLAGS := -fmodules-ts
MXX      := clang++ $(MXXFLAGS) $(CXXFLAGS)

TEST_TARGETS       = test-1 test-2
TEST_OBJECTS       = test-1.o test-2.o
MODULES            = module-1.pcm
MODULE_APPENDICES  = module-1.pcm.o
MODULE_AUXILIARIES = module-1.o
NAME      = modfcn
AUXILIARY = mod.o
LIBRARY   = libmod.so



.PHONY: all clean

all: $(TEST_TARGETS) $(MODULE_AUXILIARIES)

ifeq ($(TYPE),shared)
$(TEST_TARGETS): %: $(LIBRARY) %.o
	$(LD) $(word 2,$^) -l:$< @libs -o $@
$(LIBRARY): $(AUXILIARY)
	$(LD) -shared $^ -o $@
else
$(TEST_TARGETS): %: $(AUXILIARY) %.o
	$(LD) $^ @libs -o $@
endif

%.pcm: %.cppm
	$(MXX) --precompile $^ -o $@
%.pcm.o: %.pcm
	$(CXX) $^ -o $@
$(MODULE_AUXILIARIES): %.o: %.cpp %.pcm
	$(MXX) -fmodule-file=$(word 2,$^) $< -o $@

$(TEST_OBJECTS): %.o: %.cpp
	$(CXX) $^ -o $@
$(AUXILIARY): $(NAME).cpp $(NAME).h
	$(CXX) -fPIC $< -o $@

clean:
	$(RM) $(TEST_TARGETS) $(TEST_OBJECTS) $(AUXILIARY) $(LIBRARY) $(MODULES) $(MODULE_APPENDICES) $(MODULE_AUXILIARIES)
