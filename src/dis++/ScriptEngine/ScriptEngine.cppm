module;

#include <disconf.hpp>

#include <ruby.h>

export module ScriptEngine;

export import <string_view>;
export import <filesystem>;

export class ScriptEngine
{
  private:
	VALUE m_Disxx{Qnil};
	VALUE m_Loader{Qnil};
	VALUE m_ExecutableFile{Qnil};
	VALUE m_Section{Qnil};
	VALUE m_Label{Qnil};
	VALUE m_Disassembler{Qnil};

  public:
	[[clang::always_inline]] static inline void Init(void) noexcept;

  public:
	explicit ScriptEngine(void) noexcept;

	ScriptEngine(const ScriptEngine &) noexcept = delete;
	ScriptEngine &operator=(const ScriptEngine &) noexcept = delete;

	ScriptEngine(ScriptEngine &&) noexcept = delete;
	ScriptEngine &operator=(ScriptEngine &&) noexcept = delete;

	~ScriptEngine(void) noexcept;

	void ExecFile(const std::filsystem::path &) noexcept(false);
	void ExecString(std::string_view) noexcept;
};

[[clang::always_inline]] static inline void ScriptEngine::Init(void) noexcept { RUBY_INIT_STACK }
