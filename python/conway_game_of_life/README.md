
Game of Life Hacks to Play with C++, Python, Cppyy, Codon...
============================================================

C++ Version
-----------

Clang is probably needed to allow constexpr std::vector. 

Makefile will run conan to install stuff if neccesary.

To build life (and life_userparams)

    make -j3 CXX=clang++

Disable constexpr:

    make -j3 CXX=clang++ CXXFLAGS=-DNO_CONSTEXPR

Disable constexpr and use g++:

    make -j3 CXX=g++ CXXFLAGS=-DNO_CONSTEXPR

Python version with cPython or PyPy:
------------------------------------

To run with standard cPython:

    python3 life.py

To install pypy3:

    sudo apt install pypy3

To run with pypy:

    pypy3 life.py

cppyy versions:
---------------

To install cppyy:

    python3 -m venv conway_game_of_life
    cd conway_game_of_life
    . bin/activate
    python3 -m pip install cppyy

To run with cppyy, activate venv directory if not already (see above), then:

    bin/python3 life-cppyy.py

There are additional variations life-cppyy-2.py and life-cppyy-3.py to try as well.

Codon version:
--------------

To install codon:

    bash -c `curl -fsSL https://exaloop.io/install.sh`

To build and run with codon:

    ~/.codon/bin/codon run -release life-codon.py

Or:

    ~/.codon/bin/codon build -release -exe life-codon.py
    ./life-codon


