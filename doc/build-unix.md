Copyright (c) 2009-2012 Bitcoin Developers
Distributed under the MIT/X11 software license, see the accompanying
file license.txt or http://www.opensource.org/licenses/mit-license.php.
This product includes software developed by the OpenSSL Project for use in
the OpenSSL Toolkit (http://www.openssl.org/).  This product includes
cryptographic software written by Eric Young (eay@cryptsoft.com) and UPnP
software written by Thomas Bernard.

# STATIC BUILD INSTRUCTIONS
The following instructions have been tested on the following distributions:

1. Ubuntu 16.04.4
2. Linux Mint 18.3

The advantage of building the wallet this way is that it will be able to be executed even on a fresh ubuntu installation without adding additional libraries. There will be no dependencies error messages at startup in case some shared libs are missing. The current release was build that way.

## Get the building environment ready (same as above)

Open a terminal window. If git is not installed in your system, install it by issuing the following command
```
sudo apt install git
```
Install Linux development tools 
```
sudo apt-get install build-essential libtool automake autotools-dev autoconf pkg-config libgmp3-dev libevent-dev bsdmainutils 
```
```
sudo apt-get install libxcb-randr0-dev libxcb-xtest0-dev libxcb-xinerama0-dev libxcb-shape0-dev libxcb-xkb-dev libx11-dev 
```
## Compile all dependencies manually and use their static libs
### Download and build BerkeleyDB 5.0.32.NC
```
cd ~/deps
wget 'http://download.oracle.com/berkeley-db/db-5.0.32.NC.tar.gz'
tar -xzvf db-5.0.32.NC.tar.gz
cd db-5.0.32.NC/build_unix/
../dist/configure --enable-cxx --disable-shared 
make

```

### Compiling Boost 1.58

Download Boost 1.58 here : 
https://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz/download<br>
Put the archive in ~/deps

```
cd ~/deps
wget https://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
tar xvfz boost_1_58_0.tar.gz
cd ~/deps/boost_1_58_0
./bootstrap.sh

./b2 --build-type=complete --layout=versioned --with-chrono --with-filesystem --with-program_options --with-system --with-thread toolset=gcc variant=release link=static threading=multi runtime-link=static stage

```

### Compiling miniupnpc

Install Miniupnpc. Download it from here http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.9.tar.gz<br>
and place it in your deps folder, then :
```
cd ~/deps

wget http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.9.tar.gz

tar xvfz miniupnpc-1.9.tar.gz

cd miniupnpc-1.9
make upnpc-static
```
==> Important : don't forget to rename "miniupnpc-1.9" directory to "miniupnpc"

### Compiling OpenSSL

download 1.0.2g version here : https://www.openssl.org/source/old/1.0.2/openssl-1.0.2g.tar.gz<br>
place archive in deps folders then :
```
cd ~/deps
wget https://www.openssl.org/source/old/1.0.2/openssl-1.0.2g.tar.gz
tar xvfz openssl-1.0.2g.tar.gz
cd openssl-1.0.2g 
./config no-shared no-dso
make depend
make
```

### Compiling QT 5.4.2 statically
Download QT 5.4.2 sources
https://download.qt.io/archive/qt/5.4/5.4.2/single/qt-everywhere-opensource-src-5.4.2.tar.gz<br>
Extract in deps folder
```
cd ~/deps

wget https://download.qt.io/archive/qt/5.4/5.4.2/single/qt-everywhere-opensource-src-5.4.2.tar.gz

tar xvfz qt-everywhere-opensource-src-5.4.2.tar.gz
```
after everything is extracted, create another directory where static libs will be installed. 
For example, i created ~/deps/Qt/5.4.2_static and used that directory in configure command below (it may take a while) :
```
mkdir ~/deps/Qt/5.4.2_static

cd ~/deps/qt-everywhere-opensource-src-5.4.2
```
```
./configure -static -opensource -release -confirm-license -no-compile-examples -nomake tests -prefix ~/deps/Qt/5.4.2_static -qt-zlib -qt-libpng -no-libjpeg -qt-xcb -qt-freetype -qt-pcre -qt-harfbuzz -largefile -no-openssl -gtkstyle -skip wayland -skip qtserialport -skip script -alsa -c++11 -nomake tools -continue
```
After it successfuly ends :
```
make install
```
### Compiling Linda QT wallet

Clone the Lindacoin repository
```
git clone https://github.com/Lindacoin/Linda.git
```
if required, fix the leveldb files permissions
```
cd ~/Linda/src/leveldb
chmod +x build_detect_platform
chmod 775 *
```
you may also be required to build leveldb prior to start the wallet build
```
make clean
make libleveldb.a libmemenv.a
```
build libsecp256k1
```
cd ~/Linda/src/secp256k1
chmod +x autogen.sh
./autogen.sh
./configure --enable-static --disable-shared
make
```
Just by precaution, go to secp256k1/.libs dir and delete all non static libs (all except *.a files) if present, to make sure only static libs will be used during linking

go back to Lindacoin dir to modify Linda-qt.pro if needed :
```
cd ~/Linda
nano Linda-qt.pro
```
All dependencies dir variables to set according to what have been done above in linux {} section :
```
linux {
	DEPS_PATH = $(HOME)/deps
	SECP256K1_LIB_PATH = src/secp256k1/.libs
	SECP256K1_INCLUDE_PATH = src/secp256k1/include
## comment below dependencies if u don't need to compile a static binary on linux
	MINIUPNPC_LIB_PATH = $$DEPS_PATH/miniupnpc
	MINIUPNPC_INCLUDE_PATH = $$DEPS_PATH
	BOOST_LIB_PATH = $$DEPS_PATH/boost_1_58_0/stage/lib
	BOOST_INCLUDE_PATH = $$DEPS_PATH/boost_1_58_0
	BDB_LIB_PATH = $$DEPS_PATH/db-5.0.32.NC/build_unix
	BDB_INCLUDE_PATH = $$DEPS_PATH/db-5.0.32.NC/build_unix
	OPENSSL_LIB_PATH = $$DEPS_PATH/openssl-1.0.2g
	OPENSSL_INCLUDE_PATH = $$DEPS_PATH/openssl-1.0.2g/include
}
```
After saving the .pro file :
```
export PATH=$HOME/deps/Qt/5.4.2_static/bin:$PATH
qmake
make
```

### Compiling Lindad
With dependencies (except QT cause not needed here) compliled as above, put these lines in ~/Linda/src/makefile.unix (first lines)
```
DEPS_PATH = $(HOME)/deps
SECP256K1_LIB_PATH = src/secp256k1/.libs
SECP256K1_INCLUDE_PATH = src/secp256k1/include
MINIUPNPC_LIB_PATH = $(DEPS_PATH)/miniupnpc
MINIUPNPC_INCLUDE_PATH = $(DEPS_PATH)
BOOST_LIB_PATH = $(DEPS_PATH)/boost_1_58_0/stage/lib
BOOST_INCLUDE_PATH = $(DEPS_PATH)/boost_1_58_0
BDB_LIB_PATH = $(DEPS_PATH)/db-5.0.32.NC/build_unix
BDB_INCLUDE_PATH = $(DEPS_PATH)/db-5.0.32.NC/build_unix
OPENSSL_LIB_PATH = $(DEPS_PATH)/openssl-1.0.2g
OPENSSL_INCLUDE_PATH = $(DEPS_PATH)/openssl-1.0.2g/include
```
After that in ~/Linda/src
```
make -f makefile.unix
strip Lindad
```
