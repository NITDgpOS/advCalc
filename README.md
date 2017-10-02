# advCalc

Reimplementation of calc from scratch.

Note: The Qt GUI version is available.

## Prerequisites

Qt and GNU Readline development libraries should be installed beforehand to
build the source successfully.

## Build and Execute

Firstly, clean everything if you had built it previously:

```
make cleanall
```

Then start the build:

```
make
```

To build the GUI instead, run the following command:

```
make all_gui
```

Now, you can start the calculator using:

```
./calc
```

Or the GUI using:

```
./calc_qt
```

provided you have compiled it successfully. For a system wide installation do:

```
sudo make install
```

Now you will be able to execute it simply using:

```
calc
```

Systemwide GUI installation will be provided in near future. Wait till then.


## How to Contribute

To look at the program flow you need to be able to debug. To do so, first
create a debugger friendly executable. Start by removing any executable you had
previously built:

```
make cleanall
```

then build the source code using:

```
make CFLAGS+=-ggdb
```

which will enable **GDB** friendly debugging information. For other debuggers
use `-g` instead of `-ggdb`.
