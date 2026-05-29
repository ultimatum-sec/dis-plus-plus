module;

#if __has_include(<cxxabi.h>)
#	include <cxxabi.h>
#	define __DEMANGLE_CONSTEVAL
#else
#	define __UNWIND_CONSTEVAL 				consteval
#endif

#if __has_include(<libunwind.h>)
#	include <libunwind.h>
#	define __UNWIND_CONSTEVAL
#	if defined(__aarch64__)
#		define __GETTHREADSTATE_CONSTEVAL
#	else
#   	warning "Building for some other platform than AArch64 - distress output might be limited!"
#		define __GETTHREADSTATE_CONSTEVAL	consteval
#	endif
#else
#   warning "Building without libunwind.h - distress output might be limited!"
#	define __UNWIND_CONSTEVAL				consteval
#	define __GETTHREADSTATE_CONSTEVAL		consteval
#endif

#ifdef _WIN32
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#include <filesystem>
#include <exception>
#include <typeinfo>
#include <expected>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <format>
#include <chrono>
#include <vector>
#include <string>
#include <regex>
#include <print>
#include <array>
#include <ctime>

import disxx.utility.ini.Parser;

#define BEGIN_LOG(parser, path, pid) \
	parser.Load(DisLog::s_LogPath); \
	\
	auto now{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())}; \
	\
	const auto &_{parser.Write<std::string_view>("crash.time", std::format("{}", *std::localtime(&now)))}; \
	\
	const auto &_ \
	{ \
		parser.Write<std::string_view> \
		( \
			"crash.path", \
			path.is_absolute() \
				? path.string() \
				: std::filesystem::absolute(path).string() \
		) \
	}; \
	\
	const auto &_{parser.Write<std::string_view>("crash.pid", std::format("{}", pid))}

module DisLog;

template <> struct std::formatter<DisLog::DemanglingError> : public std::formatter<std::string>
{
  public:
	auto format(const DisLog::DemanglingError &err, std::format_context &ctx) const
	{
		return std::format_to
		(
			ctx.out(),
			"{}",
			[](const DisLog::DemanglingError &err) -> std::string_view
			{
				switch (err)
				{
				  using enum DisLog::DemanglingError;
				  case ERR_NOLIB: return "libunwind not found";
				  case ERR_BADSTATUS: return "unable to demangle the function";
				  default: return "unable to complete the operation due to unknown error";
				}
			}(err)
		);
	}
};

template <> struct std::formatter<DisLog::UnwindingError> : public std::formatter<std::string>
{
  public:
	auto format(const DisLog::UnwindingError &err, std::format_context &ctx) const
	{
		return std::format_to
		(
			ctx.out(),
			"{}",
			[](const DisLog::UnwindingError &err) -> std::string_view
			{
				switch (err)
				{
				  using enum DisLog::UnwindingError;
				  case ERR_NOLIB: return "libunwind not found";
				  case ERR_BADALLOC: return "unable to allocate the memory";
				  case ERR_PROGRAMCOUNTERERROR: return "unable to read the program counter or it equals to 0";
				  default: return "unable to complete the operation due to unknown error";	
				}
			}(err)
		);
	}
};

template <> struct std::formatter<DisLog::ThreadStateError> : public std::formatter<std::string>
{
  public:
	auto format(const DisLog::ThreadStateError &err, std::format_context &ctx) const
	{
		return std::format_to
		(
			ctx.out(),
			"{}",
			[](const DisLog::ThreadStateError &err) -> std::string_view
			{
				switch (err)
				{
				  using enum DisLog::ThreadStateError;
				  case ERR_NOLIB: return "libunwind not found";
				  case ERR_ARCHERROR: return "unable to read the registers for this platform";
				  case ERR_REGISTERERROR: return "unable to read the register(s)";	
				  default: return "unable to complete the operation due to unknown error";	
				}
			}(err)
		);
	}
};

// Overloading std::formatter for ctime struct
// because of Apple clang 17 doesn't support std::chrono::current_zone()
// so I can't just format std::tm with "{:%d.%m.%Y %H:%M:%S}"
template <> struct std::formatter<std::tm> : public std::formatter<std::string>
{
  public:
	auto format(const std::tm &time, std::format_context &ctx) const
	{
		return std::format_to
		(
			ctx.out(),
			"{:02d}.{:02d}.{:d} {:02d}:{:02d}:{:02d}",
			time.tm_mday, time.tm_mon + 1, time.tm_year + 1900,
			time.tm_hour, time.tm_min, time.tm_sec
		);
	}
};

DisLog::DisLog(void) noexcept
	: m_Parser{}
	, m_ProgName{"unknown"}
{
	const auto pid
	{
		#ifdef _WIN32
			::GetCurrentProcessId()
		#else
			::getpid()
		#endif
	};
	
	BEGIN_LOG
	(
		this->m_Parser,
		this->m_ProgName,
		pid
	);
}

DisLog::DisLog(const std::filesystem::path &prog) noexcept
	: m_Parser{}
	, m_ProgName{prog}
{
	const auto pid
	{
		#ifdef _WIN32
			::GetCurrentProcessId()
		#else
			::getpid()
		#endif
	};

	BEGIN_LOG
	(
		this->m_Parser,
		this->m_ProgName,
		pid
	);
}

__DEMANGLE_CONSTEVAL std::expected<std::unique_ptr<char, decltype(&std::free)>, DisLog::DemanglingError>
DisLog::__Demangle([[maybe_unused]] const std::string &sym) noexcept
{
	#if __has_include(<cxxabi.h>)
		int status{};
		std::unique_ptr<char, decltype(&std::free)> ptr
        {
            abi::__cxa_demangle
            (
                sym.c_str(),
                nullptr,
				nullptr,
				&status
            ),
            &std::free
        };
		
		if (status) [[unlikely]]
			return std::unexpected{DemanglingError::ERR_BADSTATUS};
		else [[likely]]
			return std::move(ptr);
	#else
		return std::unexpected{DemanglingError::ERR_NOLIB};
	#endif
}

__UNWIND_CONSTEVAL
std::expected<std::vector<std::string>, DisLog::UnwindingError> DisLog::__UnwindStack(void) noexcept
{
	#if __has_include(<libunwind.h>)
		std::vector<std::string> funcs{};
		unw_context_t ctx{};
		unw_cursor_t cursor{};

		unw_getcontext(&ctx);
		unw_init_local(&cursor, &ctx);

		while (unw_step(&cursor) > 0)
		{
			unw_word_t offset{}, pc{};
			
			// pSym should be of type std::unique_ptr<char, decltype(&std::free)>
			std::unique_ptr<char, decltype(&std::free)> pSym{static_cast<char *>(std::malloc(BUFSIZ)), &std::free};
			if (!pSym) [[unlikely]]
				return std::unexpected{UnwindingError::ERR_BADALLOC};
			
			if (unw_get_reg(&cursor, UNW_REG_IP, &pc) != UNW_ESUCCESS) [[unlikely]]
				return std::unexpected{UnwindingError::ERR_PROGRAMCOUNTERERROR};
			else if (!pc) [[unlikely]]
				return std::unexpected{UnwindingError::ERR_PROGRAMCOUNTERERROR};

			if (unw_get_proc_name(&cursor, pSym.get(), BUFSIZ, &offset) == UNW_ESUCCESS) [[likely]]
			{
				const auto demangled{__Demangle(pSym.get())};
				funcs.emplace_back
				(
					std::format
					(
						"{} + 0x{:x} [0x{:016x}]",
						(
							demangled
								? *demangled
								: pSym
						).get(),
						offset,
						pc
					)
				);
			}
			else [[unlikely]]
			{
				funcs.emplace_back
				(
					std::format
					(
						"??? + 0x{:x} [0x{:016x}]",
						offset,
						pc
					)
				);
			}
		}

		return funcs;
	#else
		return std::unexpected{UnwindingError::ERR_NOLIB};
	#endif
}

__GETTHREADSTATE_CONSTEVAL
std::expected<std::vector<std::string>, DisLog::ThreadStateError> DisLog::__GetThreadState(void) noexcept
{
	#if __has_include(<libunwind.h>) && defined(__aarch64__)
		std::vector<std::string> registers{};
		unw_context_t ctx{};
		unw_cursor_t cursor{};

		unw_getcontext(&ctx);
		unw_init_local(&cursor, &ctx);

		for (auto i{0}; unw_step(&cursor) > 0; ++i)
		{
			// Get register state on a last function excepting logger's method
			if (i == 0) continue;
			else if (i > 1) break;

			std::string accumulator{};
			for (unsigned short j{UNW_AARCH64_X0}; j <= UNW_AARCH64_PC; ++j)
			{
				if (unw_word_t state{}; unw_get_reg(&cursor, j, &state) == UNW_ESUCCESS) [[likely]]
				{
					if (j > 0 && j % 4 == 0)
					{
						registers.push_back(accumulator);
						accumulator.clear();
					}

					accumulator += std::format("{}: 0x{:016x} ", s_RegsTable.at(j), state);
				}
				else [[unlikely]]
					return std::unexpected{ThreadStateError::ERR_REGISTERERROR};
			}

			registers.push_back(accumulator);
		}

		return registers;
	#elif !defined(__aarch64__)
		return std::unexpected{ThreadStateError::ERR_ARCHERROR};
	#else
		return std::unexpected{ThreadStateError::ERR_NOLIB};
	#endif
}

// TODO: add macro for unwinding the stack and printing thread state

int DisLog::LogErr(const std::exception &err) const noexcept
{
	auto demangled{__Demangle(typeid(err).name())};
	
	std::println
	(
		std::cerr,
		"{} -> {}",
		demangled
			? demangled->get()
			: typeid(err).name(),
		err.what()
	);

	__UnwindStack().and_then
	(
		[](const std::vector<std::string> &funcs) -> std::invoke_result<decltype(DisLog::__UnwindStack)>::type
		{
			for (const auto &func : funcs | std::views::drop(1))
				std::println(std::cerr, "|  at {}", func);
			return funcs;
		}
	).or_else
	(
		[](const DisLog::UnwindingError &error) -> std::invoke_result<decltype(DisLog::__UnwindStack)>::type
		{
			std::println(std::cerr, "|  Unable to unwind the stack: {}!", error);
			return std::unexpected{error};
		}
	);

	__GetThreadState().and_then
	(
		[](const std::vector<std::string> &registers) -> std::invoke_result<decltype(DisLog::__GetThreadState)>::type
		{
			std::println("ARM64 thread state:");
			for (const auto &reg : registers)
				std::println(std::cerr, "|  {}", reg);
			return registers;
		}
	).or_else
	(
		[](const DisLog::ThreadStateError &error) -> std::invoke_result<decltype(DisLog::__GetThreadState)>::type
		{
			std::println(std::cerr, "Unable to get ARM64 thread state: {}!", error);
			return std::unexpected{error};
		}
	);

	return EXIT_FAILURE;
}

int DisLog::LogErr(void) const noexcept
{
	std::println(std::cerr, "[unknown]");

	__UnwindStack().and_then
	(
		[](const std::vector<std::string> &funcs) -> std::invoke_result<decltype(DisLog::__UnwindStack)>::type
		{
			for (const auto &func : funcs | std::views::drop(1))
				std::println(std::cerr, "|  at {}", func);
			return funcs;
		}
	).or_else
	(
		[](const DisLog::UnwindingError &error) -> std::invoke_result<decltype(DisLog::__UnwindStack)>::type
		{
			std::println(std::cerr, "|  Unable to unwind the stack: {}!", error);
			return std::unexpected{error};
		}
	);

	__GetThreadState().and_then
	(
		[](const std::vector<std::string> &registers) -> std::invoke_result<decltype(DisLog::__GetThreadState)>::type
		{
			std::println("ARM64 thread state:");
			for (const auto &reg : registers)
				std::println(std::cerr, "|  {}", reg);
			return registers;
		}
	).or_else
	(
		[](const DisLog::ThreadStateError &error) -> std::invoke_result<decltype(DisLog::__GetThreadState)>::type
		{
			std::println(std::cerr, "Unable to get ARM64 thread state: {}!", error);
			return std::unexpected{error};
		}
	);
	
	return EXIT_FAILURE;
}
