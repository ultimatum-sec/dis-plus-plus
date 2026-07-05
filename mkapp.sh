#!/bin/bash

if [ `uname -s` = "Darwin" ]
then
	make -C ./src/dis++/
	make -C ./src/reporter/
	
	mkdir -p ./dis++.app/Contents/
	cp ./misc/Info.plist ./dis++.app/Contents/Info.plist

	echo "APPL????" > ./dis++.app/Contents/PkgInfo

	mkdir -p ./dis++.app/Contents/Frameworks/
	cp ./src/dis++/build/dependencies/libdisxx-util.dylib ./dis++.app/Contents/Frameworks/libdisxx-util.dylib
	cp ./src/dis++/build/dependencies/libdisxx-ui.dylib ./dis++.app/Contents/Frameworks/libdisxx-ui.dylib
	cp ./src/dis++/build/dependencies/libdisxx-macho.dylib ./dis++.app/Contents/Frameworks/libdisxx-macho.dylib
	cp ./src/dis++/build/dependencies/libdisxx-disasm.dylib ./dis++.app/Contents/Frameworks/libdisxx-disasm.dylib

	# That's some weird stuff with this library: it always uses inorrect install name of utilities!
	install_name_tool ./dis++.app/Contents/Frameworks/libdisxx-disasm.dylib \
		-change build/libdisxx-util.dylib @executable_path/../Frameworks/libdisxx-util.dylib

	mkdir -p ./dis++.app/Contents/MacOS/
	cp ./src/dis++/build/dis++ ./dis++.app/Contents/MacOS/dis++
	cp ./src/reporter/build/reporter ./dis++.app/Contents/MacOS/reporter
else
	make -C ./src/dis++/
	make -C ./src/reporter/

	mkdir -p ./dis++/
	cp ./src/dis++/build/dependencies/libdisxx-util.dylib ./dis++/libdisxx-util.dylib
	cp ./src/dis++/build/dependencies/libdisxx-ui.dylib ./dis++/libdisxx-ui.dylib
	cp ./src/dis++/build/dependencies/libdisxx-macho.dylib ./dis++/libdisxx-macho.dylib
	cp ./src/dis++/build/dependencies/libdisxx-disasm.dylib ./dis++/libdisxx-disasm.dylib

	cp ./src/dis++/build/dis++ ./dis++/dis++
	cp ./src/reporter/build/reporter ./dis++/reporter
fi
