# advCalc

Reimplementation of calc from scratch.

Note: The Qt GUI version is available.

## Prerequisites

Qt and GNU Readline development libraries should be installed beforehand to
build the source successfully.

On Debian and Debian based distributions:

```
sudo apt-get install qt5-default cmake libreadline-dev
```

## Build and Execute

Generate the **Makefile** using `cmake` then start the build:

```
cmake .
make
```

This will create two executables and a library, `src/calc`[CLI app],
`src/calc_ui_qt/calc_qt`[QT GUI app] and `src/libadvCalc.a`[calc core library].
Start using the executables as you wish.

```
./src/calc                # CLI
./src/calc_ui_qt/calc_qt  # QT GUI
```

Developers see [Getting Started](GettingStarted.org) for further reading.
