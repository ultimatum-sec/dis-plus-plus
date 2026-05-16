# OS and arch already defined on Windows
ifeq ($(shell echo %OS%), Windows_NT)
	# Another defined variable on Windows
	ARCH=$(shell echo %PROCESSOR_ARCHITECTURE%)
	PATHSEP=\\
else
	# If it isn't Windows - that's probably UNIX!
	# Well, it will be checked anyway
	ARCH=$(shell uname -m)
	OS=$(shell uname -s)
	PATHSEP=/
endif

RM=

RUBY=
CC=
CXX=
LD=

CFLAGS=
CXXFLAGS=
MODULESFLAGS=
LDFLAGS=

OPERANDS=.$(PATHSEP)operand$(PATHSEP)
INSNS=.$(PATHSEP)instruction$(PATHSEP)

REGSSCRIPT=./misc/registers.rb

SCRIPTFLAGS=\
	--input-file=$(PATHSEP)Register$(PATHSEP)register.dat \
	--interface-file=$(OPERANDS)Register$(PATHSEP)RegsTable.hpp \
	--impl-file=$(OPERANDS)Register$(PATHSEP)RegsTable.cpp

MODULES=.$(PATHSEP)include$(PATHSEP)dis.disasm.DisStream.cppm

SOURCES=\
	.$(PATHSEP)utils$(PATHSEP)DisassemblyError$(PATHSEP)DisassemblyError.cpp \
	.$(PATHSEP)utils$(PATHSEP)bits$(PATHSEP)bits.cpp \
    $(OPERANDS)AbstractOperand$(PATHSEP)AbstractOperand.cpp \
	$(OPERANDS)AbstractOperand$(PATHSEP)AbstractOperand_AbstractImpl.cpp \
	$(OPERANDS)Register$(PATHSEP)Register.cpp \
	$(OPERANDS)Register$(PATHSEP)Register_RegisterImpl.cpp \
    $(OPERANDS)Register$(PATHSEP)RegsTable.cpp \
    $(OPERANDS)Shift$(PATHSEP)Shift.cpp \
	$(OPERANDS)Shift$(PATHSEP)Shift_ShiftImpl.cpp \
	$(OPERANDS)Condition$(PATHSEP)Condition.cpp \
	$(OPERANDS)Condition$(PATHSEP)ConditionImpl.cpp \
	$(INSNS)AbstractAnalyzer$(PATHSEP)AbstractImpl.cpp \
	$(INSNS)Reserved$(PATHSEP)ReservedImpl.cpp \
	$(INSNS)Reserved$(PATHSEP)Reserved.cpp \
	$(INSNS)DataProcessingImmediate$(PATHSEP)DataProcImmImpl.cpp \
	$(INSNS)DataProcessingImmediate$(PATHSEP)DataProcImm.cpp \
	$(INSNS)DataProcessingRegister$(PATHSEP)DataProcRegImpl.cpp \
    $(INSNS)DataProcessingRegister$(PATHSEP)DataProcReg.cpp \
	$(INSNS)BranchesExceptionsAndSysInsns$(PATHSEP)BranchesExceptionsAndSysInsnsImpl.cpp \
    $(INSNS)BranchesExceptionsAndSysInsns$(PATHSEP)BranchesExceptionsAndSysInsns.cpp \
	$(INSNS)DataProcessingScalarFPAndAdvSIMD$(PATHSEP)AdvSIMDAndFPImpl.cpp \
	$(INSNS)DataProcessingScalarFPAndAdvSIMD$(PATHSEP)AdvSIMDAndFP.cpp \
	$(INSNS)LoadsAndStores$(PATHSEP)LoadsAndStoresImpl.cpp \
	$(INSNS)LoadsAndStores$(PATHSEP)LoadsAndStores.cpp \
	$(INSNS)AnalyzerFactory$(PATHSEP)__Stub$(PATHSEP)__StubImpl.cpp \
	$(INSNS)AnalyzerFactory$(PATHSEP)__Stub$(PATHSEP)__Stub.cpp \
	.$(PATHSEP)dis.disasm.DisStream.cpp

# Object file extension
OBJ=
# Dynamic library file extension
DYLIB=
# Executable file extension
EXEC=

ifeq ($(OS), Windows_NT)
	# Setting the command to delete file	
	RM=del /f

	# Setting extensions
	OBJ=.obj
	DYLIB=.dll	
	EXEC=.exe

	RUBY=$(shell where ruby 2>NUL)
	ifeq ($(RUBY),)
		$(error RubyNotFoundError)
	endif

	# Change directory to detect the MSVC
	CURDIR=$(shell echo %cd%)
	# Using MSVC compiler on Windows	
	ifneq ($(VCINSTALLDIR),)
		ifneq ($(VCINSTALLDIR), $(CURDIR))
			cd $(VCINSTALLDIR)
			
 		   	CC=$(shell where cl 2>NUL)
 		   	CXX=$(shell where cl 2>NUL)
    		LD=$(shell where link 2>NUL)
			
			ifeq ($(ARCH), ARM64)
        		ASM=$(shell where armasm64 2>NUL)
        		ifeq ($(ASM),)
            		ASM=$(CC)
            		ASMFLAGS=/arch:ARM64
        		else
         		   ASMFLAGS=-machine ARM64         # Why does armasm64 flags use '-' instead of '/'?   
        		endif
			endif

			cd $(CURDIR)
		else
    		CC=$(shell where cl 2>NUL)
    		CXX=$(shell where cl 2>NUL)
   			LD=$(shell where link 2>NUL)
		endif
	endif
	
	ifeq ($(CC),)
		$(error CompilerNotFoundError)
	elsif ($(LD),)
		LD=$(CC) /link
	endif
	
	CFLAGS=/c /O2 /Ob2 /Gy /W4 /std:c99 /EHsc /NOENTRY
	CXXFLAGS=/c /O2 /Ob2 /Gy /W4 /MDd /std:c++20 /EHsc /NOENTRY
	LDFLAGS=/OPT:REF /LTCG /fsanitize:address /DLL
	
	ifeq ($(ARCH), ARM64)
		ASM=$(shell where armasm64 2>NUL)
		ifeq ($(ASM),)
			ASM=$(CC)
			ASMFLAGS=/arch:ARM64
		else
			ASMFLAGS=-machine ARM64         # Why does armasm64 flags use '-' instead of '/'?	
		endif

		CFLAGS+=/arch:ARM64
		CXXFLAGS+=/arch:ARM64
		LDFLAGS+=/MACHINE:ARM64
	else ifeq ($(ARCH), AMD64)
		CFLAGS+=/arch:x64
		CXXFLAGS+=/arch:x64
		LDFLAGS+=/MACHINE:x64
	else
		 $(error UnsupportedArchError)
	endif
else ifeq ($(filter $(OS), Linux FreeBSD NetBSD OpenBSD DragonFly MidnightBSD), $(OS))
	# Setting the command to delete file 
	RM=rm -f
	
	# Setting extensions
	OBJ=.o
	DYLIB=.so

	RUBY=$(shell which ruby 2>/dev/null)
    ifeq ($(RUBY),)
        $(error RubyNotFoundError)
    endif

	# Using GCC on Linux and BSD systems
	CC=$(shell which gcc 2>/dev/null)
	CXX=$(shell which g++ 2>/dev/null)
	LD=$(shell which ld 2>/dev/null)

	ifeq ($(CC),)
		$(error CompilerNotFoundError)
	else ifeq ($(CXX),)
		$(error CompilerNotFoundError)
	else ifeq ($(LD),)
		LD=$(CC)
	endif

	CFLAGS=-c -Wall -Wextra -Werror -isysroot $(shell $(CC) -print-file-name=include)
	CXXFLAGS=-c -Wall -Wextra -Werror -isysroot $(shell $(CXX) -print-file-name=include)
	LDFLAGS=-syslibroot $(shell $(CC) -print-file-name=lib)
	
	ifeq ($(ARCH), arm64)
		CFLAGS+=-arch arm64
		CXXFLAGS+=-arch arm64
		LDFLAGS+=-arch arm64

		ifeq ($(ARCH), Linux)
			ASM=$(shell which as 2>/dev/null)
			ifeq ($(ASM),)
				ASM=$(CC)	
			endif

			ASMFLAGS=-c -Wall -Wextra -Werror -arch arm64
		endif
	else ifeq ($(ARCH), x86_64)
		CFLAGS+=-acrh x86_64
		CXXFLAGS+=-acrh x86_64
		LDFLAGS=-acrh x86_64
	else
		$(error UnsupportedArchError: $(ARCH))
	endif
else
	ifeq ($(OS), Darwin)
		# Setting the command to delete file
		RM=rm -f

		# Setting extensions	
		OBJ=.o
		DYLIB=.dylib

		XCRUN=$(shell xcrun 2>/dev/null)	
		ifeq ($(XCRUN),)
			$(error XcrunNotFoundError)
		endif
	
		RUBY=$(shell xcrun --find ruby 2>/dev/null)
    	ifeq ($(RUBY),)
    	    $(error RubyNotFoundError)
    	endif

		# Using clang on Darwin
		CC=$(shell xcrun --find clang 2>/dev/null)
   	    CXX=$(shell xcrun --find clang++ 2>/dev/null)
		LD=$(shell xcrun --find ld 2>/dev/null)

		ifeq ($(CC),)
			$(error CompilerNotFoundError)
		else ifeq ($(CXX),)
			$(error CompilerNotFoundError)
		else ifeq ($(LD),)
			LD=$(CC)
		endif
   	    
		CFLAGS=-c -std=c99 -O3 -Wall -Wextra -Werror -isysroot $(shell xcrun --show-sdk-path)
       	CXXFLAGS=\
				-c -std=c++26 -O3 -Wall -Wextra -Werror -pedantic -arch arm64 -isysroot $(shell xcrun --show-sdk-path) \
				-fmodules -fcxx-modules -fprebuilt-module-path=./include -Xclang -fmodules-local-submodule-visibility -I ./include \
				-fstack-protector-all -DNDEBUG 
				#-fno-omit-frame-pointer -fstack-protector-all -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -fPIC \
				#-fsanitize=address -fsanitize=undefined
				#-fmodules -fcxx-modules -fprebuilt-module-path=. -Xclang -fmodules-local-submodule-visibility
		MODULESFLAGS=-Xclang -emit-module-interface --precompile -Wno-unused-command-line-argument
		LDFLAGS=-dylib -demangle -dead_strip -arch arm64 -syslibroot $(shell xcrun --show-sdk-path) -O2 -lSystem -lc++
			#-platform_version macos 15.0.0 15.4
			#-current_version 0.1.0 -compatibility_version 0.1.0 -lclang_rt.asan_osx_dynamic \
			#-L$(shell $(CXX) -print-search-dirs | grep 'libraries: ' | sed 's/libraries: =//')/lib/darwin
            #-rpath @executable_path -rpath $(shell $(CXX) -print-search-dirs | grep 'libraries: ' | sed 's/libraries: =//')/lib/darwin
			#$(shell $(CXX) -print-search-dirs | grep 'libraries: ' | sed 's/libraries: =//')/lib/darwin/libclang_rt.osx.a \	
		#ifeq ($(ARCH), arm64)
		#	ASM=$(shell xcrun --find as)
		#	ifeq ($(ASM),)
		#		ASM=$(CC)
		#	endif
#
#			ASMFLAGS=-Wall -Wextra -Werror -arch arm64
#			CFLAGS+=-arch arm64
#			CXXFLAGS+=-arch arm64
#			LDFLAGS+=-arch arm64
#		else
#			CFLAGS+=-arch x86_64
#			CXXFLAGS+=-arch x86_64
#			LDFLAGS+=-arch x86_64
#		endif
	else
		$(error UnsupportedPlatformError: $(OS))
	endif
endif
