TARGET = calc

LIBS = -lreadline

.PHONY: clean all cleanall
all: calc

$(TARGET): main.o calcOptr.o input_bindings.o calcError.o str.o
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

clean:
	$(RM) *.o

cleanall:
	$(RM) calc *.o
