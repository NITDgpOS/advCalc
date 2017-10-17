TARGET = calc
QT_TARGET = calc_qt

LIBS = -lreadline
CFLAGS = -std=c++14

TESTS_FILE = tests/simpleTests.calc
BUGS_FILE = tests/bugTests.calc

PREFIX = /usr/local

LIBOBJ = str.o calcError.o calcOptr.o
OBJECTS = $(LIBOBJ) input_bindings.o main.o

RM = rm -f

.PHONY: clean all cleanall install tests lib all_gui
all: calc

$(TARGET): main.o calcOptr.o input_bindings.o calcError.o str.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $<

clean:
	-$(RM) *.o
	-cd calc_ui_qt && make clean

cleanall:
	-$(RM) $(TARGET) $(QT_TARGET) *.o *.gch
	-cd calc_ui_qt && make clean
	-cd calc_ui_qt && $(RM) Makefile calc_ui_qt *.gch *.o

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/bin

tests: $(TARGET)
	./$(TARGET) -f $(TESTS_FILE)
	./$(TARGET) -f $(BUGS_FILE)

all_gui: $(QT_TARGET)

$(QT_TARGET): calc_ui_qt/
	-cd calc_ui_qt && qmake calc_ui_qt.pro && make
