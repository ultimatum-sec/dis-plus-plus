ifeq ($(shell echo %OS%), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

MODULESPATH=..$(PATHSEP)modules$(PATHSEP)loader$(PATHSEP)

BUILDDIR=.$(PATHSEP)build
DEPENDENCIESDIR=$(BUILDDIR)$(PATHSEP)dependencies
PRECOMPILEDDIR=.$(PATHSEP)build$(PATHSEP)precompiled
OBJECTSDIR=.$(PATHSEP)build$(PATHSEP)objects

CXXFLAGS=-std=c++26 -Weverything -Werror -O3 -arch $(ARCH) \
	-isysroot $(shell xcrun --show-sdk-path) -I..$(PATHSEP)include \
	-Wno-c99-extensions -Wno-global-constructors -Wno-exit-time-destructors -Wno-reserved-identifier -Wno-non-virtual-dtor \
	-Wno-switch-enum -Wno-reserved-macro-identifier -Wno-writable-strings -Wno-weak-vtables -Wno-undefined-func-template \
	-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-covered-switch-default -Wno-padded -Wno-sign-conversion \
	-Wno-unused-command-line-argument -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility \
	-fprebuilt-module-path=$(PRECOMPILEDDIR) -fstack-protector-all -DNDEBUG

LDFLAGS=-dylib -arch $(ARCH) -O2 $(LTOFLAGS) -S -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++ \
	-L$(DEPENDENCIESDIR) -ldisxx-util
ifeq ($(OS),Darwin)
	LDFLAGS+=-install_name @executable_path/../Frameworks/libdisxx-macho.$(DYLIB)
else
	ifne ($(OS),Windows_NT)
		LDFLAGS+=-install_name $$ORIGIN/libdisxx-macho.$(DYLIB)
endif

MODULES=\
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)file$(PATHSEP)disxx.utility.file.MappedFile.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.loader.utility.BinaryInfo.cppm \
	$(MODULESPATH)private$(PATHSEP)utility$(PATHSEP)disxx.loader.utility.Label.cppm \
	$(MODULESPATH)private$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.Label.cppm \
	$(MODULESPATH)private$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.Section.cppm \
	$(MODULESPATH)public$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.ExecutableFile.cppm \
	$(MODULESPATH)public$(PATHSEP)disxx.loader.macho.Loader.cppm

SOURCES=\
	.$(PATHSEP)utility$(PATHSEP)disxx.loader.utility.BinaryInfo.cpp \
	.$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.Label.cpp \
	.$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.Section.cpp \
	.$(PATHSEP)executable$(PATHSEP)disxx.loader.executable.ExecutableFile.cpp \
	.$(PATHSEP)disxx.loader.macho.Loader.cpp

all: $(BUILDDIR)$(PATHSEP)libdisxx-macho.$(DYLIB)

UTILITIES=$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-util.$(DYLIB)
DEPENDENCIES=$(UTILITIES)

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP), $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

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
	$(CXX) -c $(CXXFLAGS) -Xclang -emit-module-interface --precompile $< -o $@
$(MOBJECTS): $(PRECOMPILEDDIR)$(PATHSEP)%.o: $(PRECOMPILEDDIR)$(PATHSEP)%.pcm | $(PRECOMPILEDDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(OBJECTS): $(OBJECTSDIR)$(PATHSEP)%.o: %.cpp | $(OBJECTSDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(UTILITIES): | $(DEPENDENCIESDIR)
	make -C ..$(PATHSEP)utility
	$(MOVE) ..$(PATHSEP)utility$(PATHSEP)build$(PATHSEP)libdisxx-util.$(DYLIB) $@
$(BUILDDIR)$(PATHSEP)libdisxx-macho.$(DYLIB): $(MOBJECTS) $(OBJECTS) | $(DEPENDENCIES)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
