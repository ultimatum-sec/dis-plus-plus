module;

#if __has_include(<cxxabi.h>)
#	define __DEMANGLE_CONSTEVAL
#else
#	define __UNWIND_CONSTEVAL 				consteval
#endif

#if __has_include(<libunwind.h>)
#	define __UNWIND_CONSTEVAL
#	if defined(__aarch64__)
#		define __GETTHREADSTATE_CONSTEVAL
#	else
#		define __GETTHREADSTATE_CONSTEVAL	consteval
#	endif
#else
#	define __UNWIND_CONSTEVAL				consteval
#	define __GETTHREADSTATE_CONSTEVAL		consteval
#endif

#include <filesystem>
#include <exception>
#include <expected>
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
#include <array>

export module DisLog;

export class DisLog
{
  private:
	#ifdef __aarch64__
		static constexpr std::array<const char *, 33> s_RegsTable
		{
			"X0", "X1", "X2", "X3", "X4", "X5", "X6", "X7",
			"X8", "X9", "X10", "X11", "X12", "X13", "X14",
			"X15", "X16", "X17", "X18", "X19", "X20", "X21",
			"X22", "X23", "X24", "X25", "X26", "X27", "X28",
			"FP", "LR", "SP", "PC"
		};
	#endif

  private:	
	std::filesystem::path m_ProgName;

  public:
	enum class DemanglingError { ERR_NOLIB, ERR_BADSTATUS };
	enum class UnwindingError { ERR_NOLIB, ERR_BADALLOC, ERR_PROGRAMCOUNTERERROR };
	enum class ThreadStateError { ERR_NOLIB, ERR_ARCHERROR, ERR_REGISTERERROR };

  private:
	static __DEMANGLE_CONSTEVAL std::expected<std::unique_ptr<char, decltype(&std::free)>, DemanglingError>
	__Demangle(const std::string &) noexcept;
	static __UNWIND_CONSTEVAL std::expected<std::vector<std::string>, UnwindingError>
	__UnwindStack(void) noexcept;
	static __GETTHREADSTATE_CONSTEVAL std::expected<std::vector<std::string>, ThreadStateError>
	__GetThreadState(void) noexcept;

  public:
	explicit DisLog(void) noexcept = default;
	explicit DisLog(const std::filesystem::path &) noexcept;	
	DisLog(const DisLog &) noexcept = default;
	DisLog &operator=(const DisLog &) noexcept = default;
	~DisLog(void) noexcept = default;

	int LogErr(const std::exception &) const noexcept;
	int LogErr(void) const noexcept;
};
