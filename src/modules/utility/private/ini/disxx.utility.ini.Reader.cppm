module;

#include <disconf.hpp>

#define CHECK_KEY(sect, key) \
	if (!this->m_Table.contains(sect)) [[unlikely]] \
		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}}; \
	else if (!this->m_Table[sect].contains(key)) [[unlikely]] \
		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}}

export module disxx.utility.ini.Reader;

import disxx.utility.ini.BaseParser;
import disxx.utility.error.ParserError;

export import <type_traits>;
export import <charconv>;

export namespace disxx::utility::ini
{
	class __DISXX_PRIVATE__ [[nodiscard]] Reader final : public BaseParser
	{
	  public:
		template <typename T>
		using ReadResult = std::expected
		<
			T,
			disxx::utility::error::ParserError
		>;

	  public:
		Reader(void) noexcept;
		Reader(const std::filesystem::path &) noexcept;
	
		Reader(const Reader &) noexcept;
		Reader &operator=(const Reader &) noexcept;

		Reader(Reader &&) noexcept;
		Reader &operator=(Reader &&) noexcept;
	
		virtual ~Reader(void) noexcept override = default;
	
		template <typename T>
		ReadResult<T> Get(std::string_view) noexcept;
	
		template <>
		ReadResult<bool> Get<bool>(std::string_view) noexcept;
	
		template <>
		ReadResult<std::string> Get<std::string>(std::string_view) noexcept;
	};

	template <typename T>
	Reader::ReadResult<T> Reader::Get(std::string_view str) noexcept
	{
		static_assert
		(
			std::is_integral<T>::value,
			"The function with this type" \
			" does not allowed" 
		);

		const auto &[sect, key]{BaseParser::__Split(str)};
		CHECK_KEY(sect, key);

		const auto &val{this->m_Table[sect][key]};
		if (T n{}; std::from_chars(val.begin(), val.end(), n)) [[likely]]
			return n;
		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	template <>
	Reader::ReadResult<bool> Reader::Get<bool>(std::string_view str) noexcept
	{
		const auto &[sect, key]{BaseParser::__Split(str)};
		CHECK_KEY(sect, key);

		const auto &val{this->m_Table[sect][key]};
		if (val == "true" || val == "yes")
			return true;
		else if (val == "false" || val == "no")
			return false;
		// Non-boolean value
		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	template <>
	Reader::ReadResult<std::string> Reader::Get<std::string>(std::string_view str) noexcept
	{
		const auto &[sect, key]{BaseParser::__Split(str)};
		CHECK_KEY(sect, key);

		return this->m_Table[sect][key];
	}
} /* disx::utility::ini */
