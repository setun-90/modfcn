INCLUDE_DIR := $(shell llvm-config --includedir)
LIBRARY_DIR := $(shell llvm-config --libdir)

CXXFLAGS := -c -I$(INCLUDE_DIR) -march=native -O1
CXX      := clang++ $(CXXFLAGS)
LDFLAGS  := -s -L$(LIBRARY_DIR) -L. -Wl,-O1,--as-needed,--gc-sections
LD       := clang++ $(LDFLAGS)
MXXFLAGS := -fmodules-ts
MXX      := clang++ $(MXXFLAGS) $(CXXFLAGS)

CLANG_LIB_NAMES := -lclangCodeGen -lclangFrontend -lclangSerialization -lclangSema -lclangLex -lclangBasic -lclangAnalysis -lclangAST
LLVM_LIB_NAMES  := $(shell llvm-config --system-libs --libs all)

MODULE    = mod.pcm
APPENDIX  = mod.pcm.o
TARGETS   = test-1 test-2
OBJECTS   = test-1.o test-2.o
AUXILIARY = mod.o



.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: $(APPENDIX) $(AUXILIARY) %.o
	$(LD) $^ $(CLANG_LIB_NAMES) $(LLVM_LIB_NAMES) -o $@
$(OBJECTS): %.o: %.cpp %.pcm.o
	$(MXX) -fmodule-file=$(word 2,$^) -c $< -o $@

$(AUXILIARY): %.o: %.cpp %.pcm
	$(MXX) -fmodule-file=$(word 2,$^) -c $< -o $@
%.pcm: %.cppm
	$(MXX) --precompile $^ -o $@
%.pcm.o: %.pcm
	$(CXX) $^ -o $@

clean:
	$(RM) $(TARGETS) $(OBJECTS) $(APPENDIX) $(AUXILIARY) $(MODULE)
