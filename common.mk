OS=$(shell echo %OS%)
ifeq ($(OS), Windows_NT)
	PATHSEP=\\
	ARCH=$(shell echo %PROCESSORARCHITECTURE%)
else
	OS=$(shell uname -s)
	PATHSEP=/
	ARCH=$(shell uname -m)
endif

# Check if there is Windows, or else
# treat this system as UNIX
ifeq ($(OS), Windows_NT)
	# Script for assembling all the binaries together
	MKAPP=.$(PATHSEP)mkapp.bat
	
	# File extensions
	DYLIB=dll

	# Build tools
	RUBY=$(shell where ruby 2>NUL)
	ASM=$(shell where armasm64 2>NUL)
	LD=$(shell where link 2>NUL)
	CXX=$(shell where clang++ 2>NUL)

	# Check if they haven't just been found
	ifeq ($(ASM),)
		$(error Unable to find an assembler)
	endif
	ifeq ($(LD),)
		$(error Unable to find a linker)
	endif
	ifeq ($(CXX),)
		$(error Unable to find a C++ compiler)
	endif

	# Base commands (I infer they always should be in the current OS)
	MKDIR=mkdir -p
	MOVE=move /Y
	RMDIR=rmdir /s /q
	RM=def /f
else
	# Script for assembling all the binaries together
	MKAPP=.$(PATHSEP)mkapp.sh

	ifeq ($(OS), Darwin)
		# File extensions	
		DYLIB=dylib

		RUBY=$(shell xcrun --find ruby 2>/dev/null)
		ASM=$(shell xcrun --find as 2>/dev/null)
		LD=$(shell xcrun --find ld 2>/dev/null)
		CXX=$(shell xcrun --find clang++ 2>/dev/null)
	else
		# File extensions
		DYLIB=so

		RUBY=$(shell command -v ruby)
		ASM=$(shell command -v as)
		LD=$(shell command -v ld)
		CXX=$(shell command -v clang++)
	endif

	# Check if they haven't just been found
	ifeq ($(ASM),)
		$(error Unable to find an assembler)
	endif
	ifeq ($(LD),)
		$(error Unable to find a linker)
	endif
	ifeq ($(CXX),)
		$(error Unable to find a C++ compiler)
	endif

	# Optional LTO library
	LIBLTO=$(dir $(LD))..$(PATHSEP)lib$(PATHSEP)libLTO.$(DYLIB)
	ifneq ($(wildcard $(LIBLTO)),)
		LTOFLAGS=-lto_library $(LIBLTO)
	endif

	# Base commands (I infer they always should be in the current OS)
	MKDIR=mkdir -p
	MOVE=mv
	RMDIR=rm -rf
	RM=rm -f
endif
