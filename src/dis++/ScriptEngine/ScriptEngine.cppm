module;

#include <Ruby/ruby.h>

#include <exception>
#include <string>

export module ScriptEngine;

export import <string_view>;
export import <filesystem>;
export import <expected>;
export import <variant>;

export class ScriptEngine
{
  private:
	VALUE m_Disxx{};
	VALUE m_Loader{};
	VALUE m_ExecutableFile{};
	VALUE m_Section{};
	VALUE m_Label{};
	VALUE m_Disassembler{};

  public:
	[[clang::always_inline]] static inline void Init(void) noexcept;

  public:
	class EngineError final : public std::exception
	{
	  private:
		std::string m_Err{};

	  public:
		explicit EngineError(void) noexcept;
		explicit EngineError(std::string_view) noexcept;

		EngineError(const EngineError &) noexcept;
		EngineError &operator=(const EngineError &) noexcept;

		EngineError(EngineError &&) noexcept;
		EngineError &operator=(EngineError &&) noexcept;

		virtual ~EngineError(void) noexcept override = default;

		virtual const char *what(void) const noexcept override;
	};

  public:
	using ExecResult = std::expected<std::monostate, EngineError>;

  public:
	explicit ScriptEngine(void) noexcept;

	ScriptEngine(const ScriptEngine &) noexcept = delete;
	ScriptEngine &operator=(const ScriptEngine &) noexcept = delete;

	ScriptEngine(ScriptEngine &&) noexcept = delete;
	ScriptEngine &operator=(ScriptEngine &&) noexcept = delete;

	~ScriptEngine(void) noexcept;

	ExecResult ExecFile(const std::filesystem::path &) noexcept;
	ExecResult ExecString(std::string_view) noexcept;
};

[[clang::always_inline]] inline void ScriptEngine::Init(void) noexcept { RUBY_INIT_STACK }
