# Dis++

## Idea of the project

I created the project as a ~~stupid~~, simple, fast and lightweight disassembler
for macOS under M and intel chips. I try to keep project's code in object oriented
style and use the latest C++ versions (now it's C++26).

## Building

Dis++ requires Apple SDK installed. It also builds only on macOS
under both x86-64 and AArch64. Some STL versions contain a bug,
which may interrupt building (see the [the first issue](https://github.com/ultimatum-sec/dis-plus-plus/issues/1#issue-4459837456)).
Just clone this repo, "cd" to directory, containing
the source and type:
```bash
./mkapp.sh
```

## Code

I almost confident that code contains a HUGE BUNCH of bugs,
maybe because I wrote it under influence of alcohol,
antidepressants, neuroleptics, whatever :p Feel free to
fork the code and improve it. I'd be glad to accept some
help to fix all the bugs!

## License

dis++ is distributed under the terms of both the MIT license and the Apache
License (Version 2.0).
