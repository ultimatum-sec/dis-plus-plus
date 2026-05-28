module disxx.utility.ini.Parser;

namespace disxx::utility::ini
{
	Parser::Parser(void) noexcept
		: m_Reader{}
		, m_Writer{}
	{}

	Parser::Parser(const Parser &other) noexcept
		: m_Reader{other.m_Reader}
		, m_Writer{other.m_Writer}
	{}

	Parser &Parser::operator=(const Parser &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Reader = other.m_Reader;
			this->m_Writer = other.m_Writer;
		}

		return *this;
	}

	Parser::Parser(Parser &&other) noexcept
		: m_Reader{std::move(other.m_Reader)}
		, m_Writer{std::move(other.m_Writer)}
	{}

	Parser &Parser::operator=(Parser &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Reader = std::move(other.m_Reader);
			this->m_Writer = std::move(other.m_Writer);
		}

		return *this;
	}

	std::expected<std::monostate, disxx::utility::error::ParserError>
	Parser::Load(const std::filesystem::path &path) noexcept
	{
		if (const auto &result{this->m_Reader.Load(path)});
		else return result;

		if (const auto &result{this->m_Writer.Load(path)});
		else return result;
	
		return std::monostate{};
	}
} /* disxx::utility::ini */
