# libdio - A z/OS Dataset I/O Library

`libdio` is a lightweight library designed for efficient dataset I/O operations on z/OS. 
It provides a simple API to interact with z/OS datasets, making it easier to perform common tasks like reading, writing, and managing datasets.

## Build Dependencies

- **IBM XL C/C++ Compiler**
- **cJSON**: C JSON parser (required for building).
- **Coreutils** and **Diffutils**: For testing.
- **ZOAU**: z/OS Open Automation Utilities (required for testing).
- **GNU Make**: For building

To install dependencies using zopen:
```bash
zopen install cjson coreutils diffutils
```

## Building libdio

### Using zopen’s Build Framework
```bash
git clone git@github.com:ZOSOpenTools/libdioport.git
cd libdioport
zopen build -vv
```

### Manual Build
```bash
git clone git@github.com:ZOSOpenTools/libdio.git

export CJSON_INCLUDE_DIR=$ZOPEN_PKGINSTALL/cjson/cjson/include/
export CJSON_LIBRARY=$ZOPEN_PKGINSTALL/cjson/cjson/lib/libcjson.a

make
make check  # Optional: Run tests
```

## Installation
After building the library, you can install it using:
```bash
make install PREFIX=<installation_directory>
```

You can then link your programs against `libdio` by including the library in your build path.

## Usage Example

* Under construction
* See https://github.com/ZOSOpenTools/libdio/tree/main/test for examples

## License
`libdio` is licensed under the [MIT License](LICENSE). Feel free to use and modify the code as per the license terms.
