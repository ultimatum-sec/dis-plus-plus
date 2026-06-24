#ifndef __DISCONF_HPP__
#define __DISCONF_HPP__

#define __DISXX_MAJOR__			0
#define __DISXX_MINOR__ 		2
#define __DISXX_PATCHLEVEL__	0
#define __DISXX_VERSION__ 		"0.2.0-beta"

#if defined(__clang__) || defined(__GNUC__)
#	define __DISXX_PRIVATE__	__attribute__((__visibility__("hidden")))
#	define __DISXX_EXPORT__		__attribute__((__visibility__("default")))
#elif defined(_MSC_VER)
#	define __DISXX_PRIVATE__
#	define __DISXX_EXPORT__		__declspec(dllexport)
#else
#	error "UnsupportedPlatformError"
#endif

#endif /* disconf.hpp */
