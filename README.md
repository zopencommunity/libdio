libdio - A z/OS dataset I/O library

### Build dependencies

* An IBM XL C/C++ compiler
* cjson
* ZOAU (for testing)
* Coreutils (for testing - get it from z/OS Open Tools)
* Diffutils (for testing - get it from z/OS Open Tools)

### Building

```
make

make check # optional: to run tests

make install PREFIX=<yourinstalldir>
```
