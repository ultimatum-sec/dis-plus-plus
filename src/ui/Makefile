OS=$(shell echo %OS%)
ifeq ($(OS), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

MODULESPATH=..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)

BUILDDIR=.$(PATHSEP)build
PRECOMPILEDDIR=$(BUILDDIR)$(PATHSEP)precompiled
OBJECTSDIR=$(BUILDDIR)$(PATHSEP)objects

CXXFLAGS=-c -std=c++26 -arch $(ARCH) -O3 -Wall -Werror -Wextra -Wpedantic -DGL_SILENCE_DEPRECATION \
	-DNDEBUG -I ..$(PATHSEP)include -isysroot $(shell xcrun --show-sdk-path) -fmodules -fcxx-modules \
	-Xclang -fmodules-local-submodule-visibility -fstack-protector-all \
	-Wno-writable-strings -Wno-unused-command-line-argument -fprebuilt-module-path=$(PRECOMPILEDDIR)

LDFLAGS=-O2 $(LTOFLAGS) -dylib -S -arch $(ARCH) -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++ \
	-framework OpenGL -framework GLUT $(INSTALLFLAGS) $(RPATHFLAG)
ifeq ($(OS),Darwin)
	LDFLAGS+=-install_name @executable_path/../Frameworks/libdisxx-ui.$(DYLIB)
else
	ifne ($(OS),Windows_NT)
		LDFLAGS+=-install_name $$ORIGIN/libdisxx-ui.$(DYLIB)
endif

MODULES=\
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Image.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ImageLoader.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ColorTag.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vec.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vertex.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Shape.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.IRenderer.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLRenderer.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Widget.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Button.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.TextInput.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.SourceEditor.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.MainWindow.cppm

SOURCES=\
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Image.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ImageLoader.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ColorTag.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Shape.cpp \
	.$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLRenderer.cpp \
	.$(PATHSEP)disxx.ui.Widget.cpp \
	.$(PATHSEP)disxx.ui.Button.cpp \
	.$(PATHSEP)disxx.ui.TextInput.cpp \
	.$(PATHSEP)disxx.ui.SourceEditor.cpp \
	.$(PATHSEP)disxx.ui.MainWindow.cpp

.PHONY: all
all: $(BUILDDIR)$(PATHSEP)libdisxx-ui.$(DYLIB)

vpath %.cppm $(sort $(dir $(MODULES)))
vpath %.cpp $(sort $(dir $(SOURCES)))
vpath %.pcm $(sort $(dir $(PRECOMPILED)))

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP),  $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

$(PRECOMPILEDDIR):
	$(MKDIR) $@
$(OBJECTSDIR):
	$(MKDIR) $@
$(PRECOMPILED): $(PRECOMPILEDDIR)$(PATHSEP)%.pcm: %.cppm | $(PRECOMPILEDDIR)
	$(CXX) $(CXXFLAGS:-c=) -Xclang -emit-module-interface --precompile $< -o $@
$(MOBJECTS): $(PRECOMPILEDDIR)$(PATHSEP)%.o: $(PRECOMPILEDDIR)$(PATHSEP)%.pcm | $(PRECOMPILEDDIR)
	$(CXX) $(CXXFLAGS) $< -o $@
$(OBJECTS): $(OBJECTSDIR)$(PATHSEP)%.o: %.cpp | $(OBJECTSDIR)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BUILDDIR)$(PATHSEP)libdisxx-ui.$(DYLIB): $(MOBJECTS) $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
