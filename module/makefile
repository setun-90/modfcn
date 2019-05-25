INCLUDE_DIR := $(shell llvm-config --includedir)
LIBRARY_DIR := $(shell llvm-config --libdir)

CXXFLAGS := -c -I$(INCLUDE_DIR) -march=native -O1 -fno-rtti
CXX      := clang++ $(CXXFLAGS)
LDFLAGS  := -s -L$(LIBRARY_DIR) -L. -Wl,-O1,--as-needed
LD       := clang++ $(LDFLAGS)
MXXFLAGS := -fmodules-ts
MXX      := clang++ $(MXXFLAGS) $(CXXFLAGS)

CLANG_LIB_NAMES := -lclangCodeGen -lclangFrontend -lclangSerialization -lclangSema -lclangLex -lclangBasic -lclangAnalysis -lclangAST
LLVM_LIB_NAMES  := $(shell llvm-config --system-libs --libs all)

TEST_SOURCES       := $(wildcard test-*.cpp)
TEST_TARGETS       := $(subst .cpp,,$(TEST_SOURCES))
TEST_OBJECTS       := $(subst .cpp,.o,$(TEST_SOURCES))
MODULE_INT_SOURCES := $(wildcard module-*.cppm)
MODULE_INT_TARGETS := $(subst .cppm,.pcm,$(MODULE_INT_SOURCES))
MODULE_IMP_SOURCES := $(wildcard module-*.cpp)
MODULE_IMP_OBJECTS := $(subst .cpp,.o,$(MODULE_IMP_SOURCES))
MODULE_IMP_TARGETS := $(MODULE_IMP_OBJECTS)
MODULE             := mod.pcm
APPENDIX           := mod.pcm.o
AUXILIARY          := mod.o



.PHONY: all clean

all: $(TEST_TARGETS) $(MODULE_INT_TARGETS) $(MODULE_IMP_TARGETS)

$(TEST_TARGETS): %: $(APPENDIX) $(AUXILIARY) %.o
	$(LD) $^ @libs -o $@
$(TEST_OBJECTS): %.o: %.cpp %.pcm.o
	$(MXX) -fmodule-file=$(word 2,$^) -c $< -o $@

$(AUXILIARY): %.o: %.cpp %.pcm
	$(MXX) -fmodule-file=$(word 2,$^) -c $< -o $@
%.pcm: %.cppm
	$(MXX) --precompile $^ -o $@
%.pcm.o: %.pcm
	$(CXX) $^ -o $@

clean:
	$(RM) $(TEST_TARGETS) $(TEST_OBJECTS) $(MODULE_IMP_TARGETS) $(MODULE_IMP_OBJECTS) $(MODULE_INT_TARGETS) $(APPENDIX) $(AUXILIARY) $(MODULE)
