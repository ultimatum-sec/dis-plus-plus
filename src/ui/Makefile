OS=$(shell echo %OS%)
ifeq ($(OS), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

MODULESPATH=..$(PATHSEP)modules$(PATHSEP)ui$(PATHSEP)

BUILDDIR=.$(PATHSEP)build
DEPENDENCIESDIR=$(BUILDDIR)$(PATHSEP)dependencies
PRECOMPILEDDIR=$(BUILDDIR)$(PATHSEP)precompiled
OBJECTSDIR=$(BUILDDIR)$(PATHSEP)objects

CXXFLAGS=-c -std=c++26 -arch $(ARCH) -O3 -Weverything -Werror -DGL_SILENCE_DEPRECATION \
	-DNDEBUG -DBACKEND_CTX_GLUT -I ..$(PATHSEP)include -isysroot $(shell xcrun --show-sdk-path) -fmodules -fcxx-modules \
	-Wno-c99-extensions -Wno-global-constructors -Wno-exit-time-destructors -Wno-reserved-identifier -Wno-non-virtual-dtor \
	-Wno-switch-enum -Wno-reserved-macro-identifier -Wno-writable-strings -Wno-weak-vtables -Wno-undefined-func-template \
	-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-covered-switch-default -Wno-padded -Wno-sign-conversion \
	-Wno-float-conversion -Wno-implicit-int-float-conversion \
	-Xclang -fmodules-local-submodule-visibility -fstack-protector-all \
	-Wno-writable-strings -Wno-unused-command-line-argument -fprebuilt-module-path=$(PRECOMPILEDDIR)

LDFLAGS=-O2 $(LTOFLAGS) -dylib -S -arch $(ARCH) -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++ \
	-L $(DEPENDENCIESDIR) -ldisxx-util -framework OpenGL -framework GLUT $(INSTALLFLAGS) $(RPATHFLAG)
ifeq ($(OS),Darwin)
	LDFLAGS+=-install_name @executable_path/../Frameworks/libdisxx-ui.$(DYLIB)
else
	ifne ($(OS),Windows_NT)
		LDFLAGS+=-install_name $$ORIGIN/libdisxx-ui.$(DYLIB)
endif

MODULES=\
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.NullPointerError.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.DefaultDeleter.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.NonNull.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Image.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ImageLoader.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ColorTag.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vec.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Vertex.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Renderable.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Text.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Shape.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.IRenderer.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLRenderer.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.IContext.cppm \
	$(MODULESPATH)private$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLUTContext.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Widget.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Frame.cppm \
	$(MODULESPATH)private$(PATHSEP)disxx.ui.MenuEntry.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Button.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Label.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.MenuEntry.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.Menu.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.TextInput.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.SourceEditor.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.ui.MainWindow.cppm

SOURCES=\
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Image.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ImageLoader.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.ColorTag.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Renderable.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Text.cpp \
	.$(PATHSEP)utility$(PATHSEP)disxx.ui.utility.Shape.cpp \
	.$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLRenderer.cpp \
	.$(PATHSEP)backend$(PATHSEP)disxx.ui.backend.GLUTContext.cpp \
	.$(PATHSEP)disxx.ui.Widget.cpp \
	.$(PATHSEP)disxx.ui.Frame.cpp \
	.$(PATHSEP)disxx.ui.Button.cpp \
	.$(PATHSEP)disxx.ui.Label.cpp \
	.$(PATHSEP)disxx.ui.MenuEntry.cpp \
	.$(PATHSEP)disxx.ui.Menu.cpp \
	.$(PATHSEP)disxx.ui.TextInput.cpp \
	.$(PATHSEP)disxx.ui.SourceEditor.cpp \
	.$(PATHSEP)disxx.ui.MainWindow.cpp

.PHONY: all
all: $(BUILDDIR)$(PATHSEP)libdisxx-ui.$(DYLIB)

DEPENDENCIES=build$(PATHSEP)libdisxx-util.dylib

vpath %.cppm $(sort $(dir $(MODULES)))
vpath %.cpp $(sort $(dir $(SOURCES)))
vpath %.pcm $(sort $(dir $(PRECOMPILED)))

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP),  $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

DEPENDENCIES=$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-util.$(DYLIB)

$(DEPENDENCIESDIR):
	$(MKDIR) $@
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
$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-util.$(DYLIB): | $(DEPENDENCIESDIR)
	make -C ..$(PATHSEP)utility
	$(MOVE) ..$(PATHSEP)utility$(PATHSEP)build$(PATHSEP)libdisxx-util.$(DYLIB) $@
$(BUILDDIR)$(PATHSEP)libdisxx-ui.$(DYLIB): $(MOBJECTS) $(OBJECTS) | $(DEPENDENCIES)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
