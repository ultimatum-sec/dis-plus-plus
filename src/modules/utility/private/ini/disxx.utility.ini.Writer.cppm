module;

#include <disconf.hpp>

#define WRITEF(file, path, table) \
	(file).open \
	( \
		path, \
		std::fstream::out | std::fstream::trunc \
	); \
	if (!(file).is_open()) [[unlikely]] \
		return std::unexpected{disxx::utility::error::ParserError{"FileError"}}; \
	for (const auto &[section, map] : (table)) \
	{ \
		for (const auto ch : std::format("[{}]\n", sect)) \
			file.write(&ch, sizeof(char)); \
		for (const auto &[k, value] : map) \
			for (const auto ch : std::format("{}={}\n", k, value)) \
				(file).write(&ch, sizeof(char)); \
	} \
	(file).close(); \
	return std::monostate{}

export module disxx.utility.ini.Writer;

import disxx.utility.ini.BaseParser;

export import <type_traits>;
export import <expected>;
export import <charconv>;
export import <variant>;
export import <format>;

export namespace disxx::utility::ini
{
	class __DISXX_PRIVATE__ [[nodiscard]] Writer final : public BaseParser
	{
	  public:
		using WriteResult = std::expected
		<
			std::monostate,
			disxx::utility::error::ParserError
		>;

	  public:
		Writer(void) noexcept;
		Writer(const std::filesystem::path &) noexcept;
	
		Writer(const Writer &) noexcept;
		Writer &operator=(const Writer &) noexcept;

		Writer(Writer &&) noexcept;
		Writer &operator=(Writer &&) noexcept;
	
		virtual ~Writer(void) noexcept override = default;

		template <typename T>
		WriteResult Set(std::string_view, T) noexcept;

		template <>
		WriteResult Set<bool>(std::string_view, bool) noexcept;
	
		template <>
		WriteResult Set<std::string_view>(std::string_view, std::string_view) noexcept;
	};

	template <typename T>
	Writer::WriteResult Writer::Set(std::string_view str, T val) noexcept
	{
		static_assert
		(
			std::is_integral<T>::value,
			"The function with this type" \
			" does not allowed" 
		);

		const auto &[sect, key]{BaseParser::__Split(str)};
	
		std::string buff{};
		buff.reserve(1024);
		if (!std::to_chars(buff.begin(), buff.end(), val)) [[unlikely]]
			return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
		this->m_Table[sect][key] = buff;

		WRITEF(this->m_Handle, this->m_Path, this->m_Table);
	}

	template <>
	Writer::WriteResult Writer::Set<bool>(std::string_view str, bool val) noexcept
	{
		const auto &[sect, key]{BaseParser::__Split(str)};
		this->m_Table[sect][key] = val ? "true" : "false";

		WRITEF(this->m_Handle, this->m_Path, this->m_Table);
	}

	template <>
	Writer::WriteResult Writer::Set<std::string_view>(std::string_view str, std::string_view val) noexcept
	{
		const auto &[sect, key]{BaseParser::__Split(str)};
		this->m_Table[sect][key] = val;

		WRITEF(this->m_Handle, this->m_Path, this->m_Table);
	}
} /* disx::utility::ini */
