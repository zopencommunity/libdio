MODE ?= 64

ifeq ($(MODE),31)
  DATA_MODEL := ilp32
else
  DATA_MODEL := lp64
endif

ASAMODE := AMODE$(MODE)

# Toolchain and flags
export AS := as
export ASFLAGS := --SYSPARM\($(ASAMODE)\),GOFF,LIST,'SUPRWARN(425,434)'
export CC := xlc
export CFLAGS := -Wc,$(DATA_MODEL)',SUPP(CCN3764),AGGR,LIST(./)'
export CPPFLAGS := -DAMODE=$(MODE) -D_ALL_SOURCE=1
export LD := xlc
export LDFLAGS := -Wc,$(DATA_MODEL)

# Directories
SRC_DIR := src
TEST_DIR := test
PREFIX ?= /usr/local

# Targets
.PHONY: all src install check clean

all: src

src:
	cd $(SRC_DIR) && $(MAKE)

test:
	cd $(TEST_DIR) && $(MAKE) check

install: src
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install $(SRC_DIR)/libdio.a $(PREFIX)/lib
	install include/*.h $(PREFIX)/include

check: src
	cd $(TEST_DIR) && $(MAKE) check

clean:
	cd $(SRC_DIR) && $(MAKE) clean
	cd $(TEST_DIR) && $(MAKE) clean
