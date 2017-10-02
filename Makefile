TARGET = calc

LIBS = -lreadline

CFLAGS = -std=c++14

TESTS_FILE = tests/simpleTests.calc

BUGS_FILE = tests/bugTests.calc

PREFIX = /usr/local

.PHONY: clean all cleanall install tests
all: calc

$(TARGET): main.o calcOptr.o input_bindings.o calcError.o str.o
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $<

clean:
	$(RM) *.o

cleanall:
	$(RM) calc *.o

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/bin

tests: $(TARGET)
	./$(TARGET) -f $(TESTS_FILE)
	./$(TARGET) -f $(BUGS_FILE)
