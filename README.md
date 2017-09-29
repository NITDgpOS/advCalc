# advCalc

Reimplementation of calc from scratch.


## Build and Execute

Firstly, clean everything if you had built it previously:

```
$ make cleanall
```

Then start the build:

```
$ make
```

Now, you can start the calculator using:

```
$ ./calc
```

provided you have compiled it successfully. For a system wide installation do:

```
$ sudo make install
```

Now you will be able to execute it using:

```
$ calc
```


## How to Contribute

To look at the program flow you need to be able to debug. To do so, first
create a debugger friendly executable. Start by removing any executable you had
previously built:

```
$ make cleanall
```

then build the source code using:

```
$ make CFLAGS+=-ggdb
```

which will enable **GDB** friendly debugging information. For other debuggers
use `-g` instead of `-ggdb`.
