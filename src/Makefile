IINCDIR=../intinclude
INCDIR=../include
MACDIR=../maclib

AR=ar
ARFLAGS=rcs
ARCHIVE=libdio.a

OBJ_FILES=dio.o call31.o call31a.o s99.o s99a.o defaults.o json_parser.o

all: $(ARCHIVE)

clean:
	rm -f *.o *.lst $(ARCHIVE)

call31.o: call31.c $(IINCDIR)/wrappers.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(IINCDIR) -c $<

call31a.o: call31a.s
	$(AS) $(ASFLAGS) -I$(MACDIR) $< >$*.lst

s99a.o: s99a.s
	$(AS) $(ASFLAGS) -I$(MACDIR) $< >$*.lst

dio.o: dio.c $(IINCDIR)/dioint.h $(IINCDIR)/wrappers.h $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

s99.o: s99.c $(IINCDIR)/dioint.h $(IINCDIR)/wrappers.h $(IINCDIR)/s99.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

json_parser.o: json_parser.c
	$(CC) -qASCII $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

defaults.o: defaults.c
	$(CC) -qASCII $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

defaults.c: ../configuration/default.json json2c.py
	python3 json2c.py ../configuration/default.json defaults.c

$(ARCHIVE): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $(OBJ_FILES)

