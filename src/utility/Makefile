ifeq ($(shell echo %OS%), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

BUILDDIR=.$(PATHSEP)build
PRECOMPILEDDIR=$(BUILDDIR)$(PATHSEP)precompiled
OBJECTSDIR=$(BUILDDIR)$(PATHSEP)objects

CXXFLAGS=\
	-c -std=c++26 -O3 -Wall -Wextra -Wpedantic -Wshadow -Werror -arch $(ARCH) -isysroot $(shell xcrun --show-sdk-path) \
	-Xclang -fmodules-local-submodule-visibility -I..$(PATHSEP)include -fstack-protector-all -DNDEBUG \
	-fno-omit-frame-pointer -Wdouble-promotion -Wfloat-equal -Wundef -Wswitch-default -Wimplicit-fallthrough \
	-Wmissing-declarations -Wuninitialized -Wconditional-uninitialized -Wcast-qual -Wcast-align \
	-Wnon-virtual-dtor -Woverloaded-virtual -Wdelete-non-virtual-dtor -Wold-style-cast -Wthread-safety \
	-fstrict-aliasing -Wstrict-aliasing=2 -ftrapv -fwrapv -Wformat -Wformat-security -Wnull-dereference \
	-Wrange-loop-analysis -Werror=return-type -Werror=format-security -Werror=null-dereference \
	-fmodules -fcxx-modules -fvisibility=hidden -Wno-unused-command-line-argument \
	-fprebuilt-module-path=$(PRECOMPILEDDIR)

LDFLAGS=\
	-dylib -demangle -O2 $(LTOFLAGS) -S -arch $(ARCH) -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++
ifeq ($(OS),Darwin)
	LDFLAGS+=-install_name @executable_path/../Frameworks/libdisxx-util.$(DYLIB)
else
	ifne ($(OS), Windows_NT)
		LDFLAGS+=-install_name $$ORIGIN/libdisxx-util.$(DYLIB)
endif

BASEPATH=..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)

MODULES=\
	$(BASEPATH)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.DisassemblyError.cppm \
	$(BASEPATH)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.NullPointerError.cppm \
	$(BASEPATH)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.ParserError.cppm \
	$(BASEPATH)private$(PATHSEP)wrapper$(PATHSEP)disxx.utility.wrapper.AbstractWrapper.cppm \
	$(BASEPATH)private$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.DefaultDeleter.cppm \
	$(BASEPATH)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.BaseParser.cppm \
	$(BASEPATH)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Reader.cppm \
	$(BASEPATH)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Writer.cppm \
	$(BASEPATH)public$(PATHSEP)wrapper$(PATHSEP)disxx.utility.wrapper.Container.cppm \
	$(BASEPATH)public$(PATHSEP)wrapper$(PATHSEP)disxx.utility.wrapper.Pointer.cppm \
	$(BASEPATH)public$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.NonNull.cppm \
	$(BASEPATH)public$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Parser.cppm

SOURCES=\
	.$(PATHSEP)error$(PATHSEP)disxx.utility.error.NullPointerError.cpp \
	.$(PATHSEP)error$(PATHSEP)disxx.utility.error.DisassemblyError.cpp \
	.$(PATHSEP)error$(PATHSEP)disxx.utility.error.ParserError.cpp \
	.$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.BaseParser.cpp \
	.$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Reader.cpp \
	.$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Writer.cpp \
	.$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Parser.cpp

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP), $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

.PHONY: all
all: $(BUILDDIR)$(PATHSEP)libdisxx-util.$(DYLIB)

vpath %.cppm $(sort $(dir $(MODULES)))
vpath %.cpp $(sort $(dir $(SOURCES)))
vpath %.pcm $(sort $(dir $(PRECOMPILED)))

$(PRECOMPILEDDIR):
	$(MKDIR) $@
$(OBJECTSDIR):
	$(MKDIR) $@
$(PRECOMPILED): $(PRECOMPILEDDIR)$(PATHSEP)%.pcm: %.cppm | $(PRECOMPILEDDIR)$(PATHSEP)
	$(CXX) $(CXXFLAGS) -Xclang -emit-module-interface --precompile $< -o $@
$(MOBJECTS): $(PRECOMPILEDDIR)$(PATHSEP)%.o: $(PRECOMPILEDDIR)$(PATHSEP)%.pcm | $(PRECOMPILEDDIR)$(PATHSEP)
	$(CXX) $(CXXFLAGS) $< -o $@
$(OBJECTS): $(OBJECTSDIR)$(PATHSEP)%.o: %.cpp | $(OBJECTSDIR)$(PATHSEP)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BUILDDIR)$(PATHSEP)libdisxx-util.$(DYLIB): $(MOBJECTS) $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
