MODE ?= 64

ifeq ($(MODE),31)
  DATA_MODEL := ilp32
else
  DATA_MODEL := lp64
endif

ASAMODE := AMODE$(MODE)

ifdef CJSON_INCLUDE_DIR
    CPPFLAGS += -I$(CJSON_INCLUDE_DIR)
endif

ifdef CJSON_LIBRARY
    LIBS += $(CJSON_LIBRARY)
endif


# Toolchain and flags
export AS := as
export ASFLAGS := --SYSPARM\($(ASAMODE)\),GOFF,LIST,'SUPRWARN(425,434)'
export CC := xlc
export CFLAGS := -Wc,$(DATA_MODEL)',SUPP(CCN3764),AGGR,LIST(./)'
export CPPFLAGS := -DAMODE=$(MODE) -D_ALL_SOURCE=1 $(CPPFLAGS) -D_AE_BIMODAL=1
export LD := xlc
export LDFLAGS := -Wc,$(DATA_MODEL) $(LDFLAGS)
export LIBS := $(LIBS)

# Directories
SRC_DIR := src
TEST_DIR := test
PREFIX ?= /usr/local

# Targets
.PHONY: all src install check clean

all: src

src:
	cd $(SRC_DIR) && $(MAKE)

install: src
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -d $(PREFIX)/share/man/man1
	install $(SRC_DIR)/libdio.a $(PREFIX)/lib
	install $(SRC_DIR)/libdio.a $(PREFIX)/lib
	install include/*.h $(PREFIX)/include
	install libdio.1 $(PREFIX)/share/man/man1

check: src
	cd $(TEST_DIR) && $(MAKE) check

clean:
	cd $(SRC_DIR) && $(MAKE) clean
	cd $(TEST_DIR) && $(MAKE) clean
