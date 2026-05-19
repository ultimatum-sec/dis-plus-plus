module;

#include <disconf.hpp>

#include <format>

#define CHECKR(r) \
if (!(r)) [[unlikely]] \
	return std::unexpected{(r).error()}

#define ASSERTNUM(T) \
	static_assert \
	( \
		std::is_arithmetic<T>::value, \
		"The function with type " #T \
		" does not allowed" \
	)

export module disxx.utility.ini.Parser;

export import disxx.utility.error.ParserError;

export import <string_view>;
export import <filesystem>;
export import <concepts>;
export import <expected>;
export import <charconv>;
export import <variant>;
export import <utility>;
export import <fstream>;
export import <string>;

export namespace disxx::utility::ini
{
	class __DISXX_EXPORT__ [[nodiscard]] Parser
	{
	  private:
		std::filesystem::path m_Path;
		std::fstream m_Handle;

	  private:
		static std::pair<std::string, std::string> __SplitSectionWithKey(std::string_view) noexcept(false);

	  private:
		std::expected<std::monostate, disxx::utility::error::ParserError> __AddValue(std::string_view, std::string_view) noexcept;
		std::expected<std::string, disxx::utility::error::ParserError> __FindValue(std::string_view) noexcept;

	  public:
		explicit Parser(void) noexcept;

		explicit Parser(const Parser &) noexcept;
		Parser &operator=(const Parser &) noexcept;

		std::expected<std::monostate, disxx::utility::error::ParserError>
		LoadFile(const std::filesystem::path) noexcept;

		/* Getters for parameters */
	
		template <typename T>
		std::expected<T, disxx::utility::error::ParserError> Get(std::string_view) noexcept;
	
		template <>
		std::expected<bool, disxx::utility::error::ParserError> Get<bool>(std::string_view) noexcept;
	
		/*	
		template <std::integral T>
		std::expected<T, disxx::utility::error::ParserError> Get(std::string_view) noexcept;

		template <std::floating_point T>
		std::expected<T, disxx::utility::error::ParserError> Get(std::string_view) noexcept;
		*/

		template <>
		std::expected<std::string, disxx::utility::error::ParserError> Get<std::string>(std::string_view) noexcept;
	
		/* Setters for parameters */

		template <typename T>
		std::expected<std::monostate, disxx::utility::error::ParserError> Set(std::string_view, T) noexcept;

		template <>
		std::expected<std::monostate, disxx::utility::error::ParserError> Set<bool>(std::string_view, bool) noexcept;
	
		/*	
		template <std::integral T>
		std::expected<std::monostate, disxx::utility::error::ParserError> Set(std::string_view, T) noexcept;

		template <std::floating_point T>
		std::expected<std::monostate, disxx::utility::error::ParserError> Set(std::string_view, T) noexcept;
		*/

		template <>
		std::expected<std::monostate, disxx::utility::error::ParserError> Set<std::string_view>(std::string_view, std::string_view) noexcept;
	};

	template <typename T>
	std::expected<T, disxx::utility::error::ParserError> Parser::Get(std::string_view key) noexcept
	{
		ASSERTNUM(T);

		const auto &result{Parser::__FindValue(key)};
		CHECKR(result);

		if (T n{}; std::from_chars(result.value().begin(), result.value().end(), n)) [[likely]]
			return n;

		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	/*
	template <std::integral T>
	std::expected<T, disxx::utility::error::ParserError> Parser::Get(std::string_view key) noexcept
	{
		const auto &result{Parser::__FindValue(key)};
		CHECKR(result);

		if (T n{}; std::from_chars(result.value().begin(), result.value().end(), n)) [[likely]]
			return std::expected{n};

		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	template <std::floating_point T>
	std::expected<T, disxx::utility::error::ParserError> Parser::Get(std::string_view key) noexcept
	{
		const auto &result{Parser::__FindValue(key)};
		CHECKR(result);

		if (T n{}; std::from_chars(result.value().begin(), result.value().end(), n)) [[likely]]
			return std::expected{n};

		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	template <std::integral T>
	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::Set(std::string_view key, T value) noexcept
	{
		std::string buff{};
		buff.reserve(BUFSIZ);
		if (!std::to_chars(buff.begin(), buff.end(), value)) [[unlikely]]
			return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};

		CHECKR(Parser::__AddValue(key, value));

		return std::expected{std::monostate{}};
	}

	template <std::floating_point T>
	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::Set(std::string_view key, T value) noexcept
	{
		std::string buff{};
		buff.reserve(BUFSIZ);
		if (!std::to_chars(buff.begin(), buff.end(), value)) [[unlikely]]
			return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};

		CHECKR(Parser::__AddValue(key, value));

		return std::expected{std::monostate{}};
	}
	*/

	template <typename T>
	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::Set(std::string_view key, T value) noexcept
	{
		ASSERTNUM(T);

		std::string buff{};
		buff.reserve(BUFSIZ);
		if (!std::to_chars(buff.begin(), buff.end(), value)) [[unlikely]]
			return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};

		CHECKR(Parser::__AddValue(key, value));

		return std::monostate{};
	}
} /* disxx::utility::ini */
