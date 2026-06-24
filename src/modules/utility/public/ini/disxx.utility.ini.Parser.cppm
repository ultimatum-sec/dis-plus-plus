module;

#include <disconf.hpp>

export module disxx.utility.ini.Parser;

export import disxx.utility.error.ParserError;

export import <string_view>;
export import <filesystem>;

export import disxx.utility.ini.Reader;
export import disxx.utility.ini.Writer;

export namespace disxx::utility::ini
{
	class __DISXX_EXPORT__ [[nodiscard]] Parser
	{
	  private:
		Reader m_Reader{};
		Writer m_Writer{};

	  public:
		explicit Parser(void) noexcept;

		explicit Parser(const Parser &) noexcept;
		Parser &operator=(const Parser &) noexcept;

		explicit Parser(Parser &&) noexcept;
		Parser &operator=(Parser &&) noexcept;

		std::expected<std::monostate, disxx::utility::error::ParserError>
		Load(const std::filesystem::path &) noexcept;

		template <typename T>
		Reader::ReadResult<T> Read(std::string_view) noexcept;

		template <typename T>
		Writer::WriteResult Write(std::string_view, T) noexcept;
	};

	template <typename T>
	Reader::ReadResult<T> Parser::Read(std::string_view str) noexcept
	{ return this->m_Reader.Get<T>(str); }

	template <typename T>
	Writer::WriteResult Parser::Write(std::string_view str, T val) noexcept
	{ return this->m_Writer.Set<T>(str, val); }
} /* disxx::utility::ini */
