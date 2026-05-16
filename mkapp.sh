#!/bin/bash

if [ `uname -s` = "Darwin" ]
then
	mkdir -p ./dis++.app/Contents/
	cp ./Info.plist ./dis++.app/Contents/Info.plist

	echo "APPL????" > ./dis++.app/Contents/PkgInfo

	mkdir -p ./dis++.app/Contents/Frameworks/
	cp ./build/dependencies/libdisxx-util.dylib ./dis++.app/Contents/Frameworks/libdisxx-util.dylib
	cp ./build/dependencies/libdisxx-ui.dylib ./dis++.app/Contents/Frameworks/libdisxx-ui.dylib
	cp ./build/dependencies/libdisxx-macho.dylib ./dis++.app/Contents/Frameworks/libdisxx-macho.dylib
	cp ./build/dependencies/libdisxx-disasm.dylib ./dis++.app/Contents/Frameworks/libdisxx-disasm.dylib

	# That's some weird stuff with this library: it always uses inorrect install name of utilities!
	install_name_tool ./dis++.app/Contents/Frameworks/libdisxx-disasm.dylib \
		-change build/libdisxx-util.dylib @executable_path/../Frameworks/libdisxx-util.dylib

	mkdir -p ./dis++.app/Contents/MacOS/
	cp ./build/dis++ ./dis++.app/Contents/MacOS/dis++
else
	mkdir -p ./dis++/
	cp ./build/dependencies/libdisxx-util.dylib ./dis++/libdisxx-util.dylib
	cp ./build/dependencies/libdisxx-ui.dylib ./dis++/libdisxx-ui.dylib
	cp ./build/dependencies/libdisxx-macho.dylib ./dis++/libdisxx-macho.dylib
	cp ./build/dependencies/libdisxx-disasm.dylib ./dis++/libdisxx-disasm.dylib

	cp ./build/dis++ ./dis++/dis++
fi
