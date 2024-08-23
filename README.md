# libdio - A z/OS Dataset I/O library

### Build dependencies

* An IBM XL C/C++ compiler
* cjson
* ZOAU (for testing)
* Coreutils (for testing - get it from z/OS Open Tools)
* Diffutils (for testing - get it from z/OS Open Tools)
* GNU Make

If you have zopen installed, use:
```bash
zopen install cjson coreutils diffutils
```

### Building libdio

The simplest method of building libdio is by leveraging zopen's build framework.

```bash
git clone git@github.com:ZOSOpenTools/libdioport.git
cd libdioport
zopen build -vv
```

If you prefer to build it manually, clone this repo and then use make

```bash
git clone git@github.com:ZOSOpenTools/libdio.git

export CJSON_INCLUDE_DIR=$ZOPEN_PKGINSTALL/cjson/cjson/include/
export CJSON_LIBRARY=$ZOPEN_PKGINSTALL/cjson/cjson/lib/libcjson.a

make

make check # optional: to run tests

make install PREFIX=<yourinstalldir>
```
