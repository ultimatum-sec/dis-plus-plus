module;

#include <disconf.hpp>

#include <ruby.h>

export module ScriptEngine;

export class ScriptEngine
{
  public:
	[[clang::always_inline]] static inline void Init(void) noexcept;
};
