ifeq ($(shell echo %OS%), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

BUILDDIR=.$(PATHSEP)build
DEPENDENCIESDIR=$(BUILDDIR)$(PATHSEP)dependencies
PRECOMPILEDDIR=$(BUILDDIR)$(PATHSEP)precompiled
OBJECTSDIR=$(BUILDDIR)$(PATHSEP)objects

CXXFLAGS=\
   	-c -std=c++26 -Wall -Wextra -Wpedantic -Wdeprecated -Werror -O3 -arch $(ARCH) -isysroot $(shell xcrun --show-sdk-path) \
	-fno-omit-frame-pointer -Wshadow -Wdouble-promotion -Wfloat-equal -Wundef -Wswitch-default -Wimplicit-fallthrough \
	-Wmissing-declarations -Wuninitialized -Wconditional-uninitialized -Wcast-qual -Wcast-align -Wformat -Wformat-security -Wnull-dereference \
	-Wno-writable-strings -Wnon-virtual-dtor -Woverloaded-virtual -Wdelete-non-virtual-dtor -Wrange-loop-analysis -Wold-style-cast -Wthread-safety -fstrict-aliasing \
	-Wstrict-aliasing=2 -ftrapv -fwrapv -Wno-\#warnings -fsanitize-address-use-after-scope -Wno-unused-command-line-argument -Wno-c99-extensions \
	-fmodules -Xclang -fmodules-local-submodule-visibility -fcxx-modules -fprebuilt-module-path=$(PRECOMPILEDDIR) \
	-DGL_SILENCE_DEPRECATION -I..$(PATHSEP)include -fstack-protector-all -fvisibility=hidden -DNDEBUG

LDFLAGS=\
	-O2 $(LTOFLAGS) -S -demangle -arch $(ARCH) -pie -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++ \
	-L$(DEPENDENCIESDIR) -ldisxx-util -ldisxx-ui

MODULES=\
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.NullPointerError.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.ParserError.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.BaseParser.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Reader.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Writer.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)wrapper$(PATHSEP)disxx.utility.wrapper.AbstractWrapper.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)wrapper$(PATHSEP)disxx.utility.wrapper.Pointer.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)ini$(PATHSEP)disxx.utility.ini.Parser.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Image.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ImageLoader.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vec.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vertex.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Shape.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.IRenderer.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLRenderer.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)public$(PATHSEP)disxx.ui.Widget.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)public$(PATHSEP)disxx.ui.Button.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)public$(PATHSEP)disxx.ui.SourceEditor.cppm \
	..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)public$(PATHSEP)disxx.ui.MainWindow.cppm \
	.$(PATHSEP)FailHandler$(PATHSEP)FailHandler.cppm

SOURCES=\
	.$(PATHSEP)FailHandler$(PATHSEP)FailHandler.cpp \
	.$(PATHSEP)main.cpp

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP), $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

__DYLIBS=libdisxx-util.$(DYLIB) libdisxx-ui.$(DYLIB)
DYLIBS=$(addprefix $(DEPENDENCIESDIR)$(PATHSEP), $(__DYLIBS))

.PHONY: all
all: $(BUILDDIR)$(PATHSEP)reporter

vpath %.cppm $(sort $(dir $(MODULES)))
vpath %.cpp $(sort $(dir $(SOURCES)))
vpath %.pcm $(sort $(dir $(PRECOMPILED)))

$(DEPENDENCIESDIR):
	$(MKDIR) $@
$(PRECOMPILEDDIR):
	$(MKDIR) $@
$(OBJECTSDIR):
	$(MKDIR) $@
$(PRECOMPILED): $(PRECOMPILEDDIR)$(PATHSEP)%.pcm: %.cppm | $(PRECOMPILEDDIR)
	$(CXX) $(CXXFLAGS) --precompile $< -o $@
$(MOBJECTS): $(PRECOMPILEDDIR)$(PATHSEP)%.o: $(PRECOMPILEDDIR)$(PATHSEP)%.pcm | $(PRECOMPILEDDIR)
	$(CXX) $(CXXFLAGS) $< -o $@
$(OBJECTS): $(OBJECTSDIR)$(PATHSEP)%.o: %.cpp | $(OBJECTSDIR)
	$(CXX) $(CXXFLAGS) $< -o $@
$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-util.$(DYLIB): | $(DEPENDENCIESDIR)
	make -C ..$(PATHSEP)utility
	$(MOVE) ..$(PATHSEP)utility$(PATHSEP)build$(PATHSEP)$(notdir $@) $@
$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-ui.$(DYLIB): | $(DEPENDENCIESDIR)
	make -C ..$(PATHSEP)ui
	$(MOVE) ..$(PATHSEP)ui$(PATHSEP)build$(PATHSEP)$(notdir $@) $@
$(BUILDDIR)$(PATHSEP)reporter: $(MOBJECTS) $(OBJECTS) | $(DYLIBS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
