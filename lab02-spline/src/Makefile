# stolen from
#  - https://github.com/srjheam/ufes-tbo2/blob/b0fa0c0ab919e591a2f171ffc4ac8b27e83b9a04/src/Makefile

# Makefile que percorre toda a biblioteca, procurando os códigos fontes .cpp para compilar

LIBGLSRDIR        = glsrlib
LIBGLSR           = glsr
LIBGLSRDIRRELEASE = $(LIBGLSRDIR)/build/release
LIBGLSRDIRDEBUG   = $(LIBGLSRDIR)/build/debug

LIBSRUTILDIR        = srutillib
LIBSRUTIL           = srutil
LIBSRUTILDIRRELEASE = $(LIBSRUTILDIR)/build/release
LIBSRUTILDIRDEBUG   = $(LIBSRUTILDIR)/build/debug

INCLUDES = -lGL -lGLU -lglut -lm -I $(LIBGLSRDIR)/include -I $(LIBSRUTILDIR)/include 

MAIN              = spline
OUT               = spline

BUILDIR           = build
DEBUGDIR          = $(BUILDIR)/debug
RELEASEDIR        = $(BUILDIR)/release

# G++ - é o compilador usado
# CFLAGS - são as flags dadas ao compilador durante a compilação
CPP = g++
override CPPFLAGS += -Wall -Wextra $(INCLUDES)

DEBUGFLAGS = -ggdb3 -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG

MAKE = make

VALGRINDOUT = valgrind-out.txt

all : release

release : pre-build-release build-release

debug: pre-build-debug build-debug

# Preparativos para o build; Garante que o diretório build/ existe
pre-build:
	mkdir -p $(BUILDIR)/

pre-build-release: pre-build
	mkdir -p $(RELEASEDIR)/
	$(MAKE) -C $(LIBGLSRDIR)/

pre-build-debug: pre-build
	mkdir -p $(DEBUGDIR)/
	$(MAKE) -C $(LIBGLSRDIR)/ debug

build-release : $(MAIN).cpp
	$(CPP) -o $(RELEASEDIR)/$(OUT) $< -L $(LIBGLSRDIRRELEASE) -l$(LIBGLSR) $(CPPFLAGS) $(RELEASEFLAGS)

build-debug : $(MAIN).cpp
	$(CPP) -o $(DEBUGDIR)/$(OUT) $< -L $(LIBGLSRDIRDEBUG) -l$(LIBGLSR) $(CPPFLAGS) $(DEBUGFLAGS)

# Rode make clean para remover tudo que é gerado pela build
clean:
	rm -rf $(BUILDIR)/ $(VALGRINDOUT)
	$(MAKE) -C $(LIBGLSRDIR)/ clean

valgrind: debug
	rm -f $(VALGRINDOUT)

	valgrind --leak-check=full \
			 --show-leak-kinds=all \
			 --track-origins=yes \
			 --verbose \
			 --log-file=$(VALGRINDOUT) \
			 ./$(DEBUGDIR)/$(OUT) $(VALARGS)

	@echo "Head to $(VALGRINDOUT)"
