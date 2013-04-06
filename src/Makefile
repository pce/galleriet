WORKDIR = `pwd`
APP_NAME=galleriet

CC = gcc
CXX = g++
AR = ar
LD = gcc
# LD = g++
STRIP=strip

INC =  -I/usr/include/libxml2
# xslt-config --cflags
CFLAGS =  -Wall `pkg-config gtk+-2.0 --cflags`  
RESINC = 
LIBDIR = -L/usr/lib 
LIB =  -lgd -lxml2 -lxslt -lexif
LDFLAGS =  `pkg-config gtk+-2.0 --libs` `xslt-config --libs`
INSTALL_BINDIR=/usr/bin
RESOURCES_DIR=/usr/share/$(APP_NAME)

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -g -DDEBUG
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS) -lgd -lxml2
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/$(APP_NAME)

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -O2
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/$(APP_NAME)

OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LDFLAGS_DEBUG) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG)

$(OBJDIR_DEBUG)/main.o: main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.c -o $(OBJDIR_DEBUG)/main.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 
	$(STRIP) $(OUT_RELEASE)
	
release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LDFLAGS_RELEASE) $(LIBDIR_RELEASE) $(OBJ_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE)

$(OBJDIR_RELEASE)/main.o: main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.c -o $(OBJDIR_RELEASE)/main.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)


install:	all
		-mkdir -p $(INSTALL_BINDIR)
		cp $(OUT_RELEASE) $(INSTALL_BINDIR)
		cp $(RESOURCES) $(RESOURCES_DIR)
		@echo "------------------------------------------------------------------------"
		@echo
		@echo " executable '${TARGET}' went into ${BINDIR}"
		@echo
		@echo "------------------------------------------------------------------------"



.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

