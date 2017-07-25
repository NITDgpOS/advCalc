TARGET = calc

LIBS = -lreadline

CFLAGS = -std=c++14

.PHONY: clean all cleanall
all: calc

$(TARGET): main.o calcOptr.o input_bindings.o calcError.o str.o
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $<

clean:
	$(RM) *.o

cleanall:
	$(RM) calc *.o
