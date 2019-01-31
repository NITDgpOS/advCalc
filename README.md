# advCalc

[![gsocheat](https://img.shields.io/badge/GSoC%20Heat-2019-orange.svg)](https://nitdgpos.github.io/gsoc_heat)

Reimplementation of calc from scratch.

Note: The Qt GUI version is available.

## Prerequisites

Qt and GNU Readline development libraries should be installed beforehand to
build the source successfully.

On Debian and Debian based distributions:

```
sudo apt-get install qt5-default cmake libreadline-dev
```

On macOS
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
sudo brew install cmake
sudo brew install qt5
```

## Build and Execute

# For Linux
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

# For macOS

```
cmake . -DCMAKE_PREFIX_PATH=$(brew --prefix qt5)
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
