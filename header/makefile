INCLUDE_DIR := $(shell llvm-config --includedir)
LIBRARY_DIR := $(shell llvm-config --libdir)

CXXFLAGS := -c -I$(INCLUDE_DIR) -march=native -O1 -fPIC -fno-rtti
CXX      := clang++ $(CXXFLAGS)
LDFLAGS  := -L$(LIBRARY_DIR) -L. -Wl,-O1,--as-needed
LD       := clang++ $(LDFLAGS)
MXXFLAGS := -fmodules-ts
MXX      := clang++ $(MXXFLAGS) $(CXXFLAGS)

TEST_SOURCES       := $(wildcard test-*.cpp)
TEST_TARGETS       := $(subst .cpp,,$(TEST_SOURCES))
TEST_OBJECTS       := $(subst .cpp,.o,$(TEST_SOURCES))
MODULE_INT_SOURCES := $(wildcard module-*.cppm)
MODULE_INT_TARGETS := $(subst .cppm,.pcm,$(MODULE_INT_SOURCES))
MODULE_IMP_SOURCES := $(wildcard module-*.cpp)
MODULE_IMP_OBJECTS := $(subst .cpp,.o,$(MODULE_IMP_SOURCES))
MODULE_IMP_TARGETS := $(patsubst %.o,lib%.so,$(MODULE_IMP_OBJECTS))
NAME           := modfcn
LIBRARY_NAME   := mod
LIBRARY_OBJECT := $(LIBRARY_NAME).o
LIBRARY_SO     := lib$(LIBRARY_NAME).so



.PHONY: all clean

all: $(TEST_TARGETS) $(MODULE_INT_TARGETS) $(MODULE_IMP_TARGETS)

ifeq ($(TYPE),shared)
$(TEST_TARGETS): %: $(LIBRARY_SO) %.o
	$(LD) $(word 2,$^) -l$(LIBRARY_NAME) @libs -o $@
$(LIBRARY_SO): $(LIBRARY_OBJECT)
	$(LD) -shared $^ -o $@
else
$(TEST_TARGETS): %: $(LIBRARY_OBJECT) %.o
	$(LD) $^ @libs -o $@
endif
$(LIBRARY_OBJECT): $(NAME).cpp $(NAME).h
	$(CXX) $< -o $@

%.o: %.cpp %.pcm
	$(MXX) -fmodule-file=$(word 2,$^) $< -o $@
%.pcm: %.cppm
	$(MXX) --precompile $^ -o $@
%.pcm.o: %.pcm
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $^ -o $@
lib%.so: %.o
	$(LD) -shared $^ -o $@

clean:
	$(RM) $(TEST_TARGETS) $(TEST_OBJECTS) $(MODULE_IMP_TARGETS) $(MODULE_IMP_OBJECTS) $(MODULE_INT_TARGETS) $(LIBRARY_SO) $(LIBRARY_OBJECT)
