CPP=clang++
CPP_FLAGS=-O3 -Wall -Wextra -Wpedantic -std=c++11 -fPIC 
PY_LIBS=$(shell python3 -m pybind11 --includes)
PY_EXT=$(shell python3-config --extension-suffix)

OBJECTS=tracklib/cells.o tracklib/types.o

all: $(OBJECTS) TrackLib$(PY_EXT)
.PHONY: test

$(OBJECTS): tracklib/%.o: tracklib/%.cpp
	$(CPP) -o $@ -c $< $(CPP_FLAGS)

TrackLib$(PY_EXT): tracklib/bind.cpp $(OBJECTS)
	$(CPP) -o $@ $^ $(CPP_FLAGS) -shared $(PY_LIBS)

test:
	./unittests.py