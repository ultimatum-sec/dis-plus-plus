module;

#include <fstream>
#include <ranges>
#include <format>
#include <regex>

#include <print>

module disxx.utility.ini.BaseParser;

namespace disxx::utility::ini
{
	BaseParser::BaseParser(void) noexcept
		: m_Handle{}
		, m_Path{}
		, m_Table{}
	{}

	BaseParser::BaseParser(const std::filesystem::path &path) noexcept
		: m_Handle{}
		, m_Path{}
		, m_Table{}
	{ this->Load(path); }

	BaseParser::BaseParser(const BaseParser &other) noexcept
		: m_Handle{}
		, m_Path{}
		, m_Table{}
	{ this->Load(other.m_Path); }

	BaseParser &BaseParser::BaseParser::operator=(const BaseParser &other) noexcept
	{
		if (this != &other) [[likely]]
			this->Load(other.m_Path);
		return *this;
	}

	BaseParser::BaseParser(BaseParser &&other) noexcept
		: m_Handle{std::move(other.m_Handle)}
		, m_Path{std::move(other.m_Path)}
		, m_Table{std::move(other.m_Table)}
	{}

	BaseParser &BaseParser::BaseParser::operator=(BaseParser &&other) noexcept
	{
		this->m_Handle = std::move(other.m_Handle);
		this->m_Path = std::move(other.m_Path);
		this->m_Table = std::move(other.m_Table);

		return *this;
	}

	std::pair<std::string, std::string> BaseParser::__Split(std::string_view str) noexcept
	{
		std::string __str{str.data()};
		std::smatch matches{};
		std::regex_search
		(
			__str,
			matches,
			std::regex{R"((\S+)\.(\S+))"}
		);

		return std::make_pair(matches[1].str(), matches[2].str());
	}

	std::expected<std::monostate, disxx::utility::error::ParserError>
	BaseParser::Load(const std::filesystem::path &path) noexcept
	{
		this->m_Path = path;

		if (std::filesystem::exists(this->m_Path)) [[likely]]
		{
			this->m_Handle.open(this->m_Path, std::fstream::out | std::fstream::in | std::fstream::binary);
			if (!this->m_Handle.is_open()) [[unlikely]]
				return std::unexpected{disxx::utility::error::ParserError{"FileError"}};

			std::string line{}, section{};
			while (std::getline(this->m_Handle, line))
			{
				if (line.starts_with(";")) [[unlikely]]
					continue;

				// Eat all spaces
				line = std::regex_replace
				(
					line,
					std::regex{R"(\s+)"},
					std::string{}
				);

				// Eat all comments
				line = std::regex_replace
				(
					line,
					std::regex{R"(;[\s\S]+$)"},
					std::string{}
				);

				if (std::smatch match{}; std::regex_match(line, match, std::regex{R"(\[(\S+)\])"}))
					section = match[1].str();
				else if (!section.empty())
				{
					std::regex_match
					(
						line,
						match,
						std::regex{R"((\S+)\=(\S+)$)"}
					);

					this->m_Table[section][match[1].str()] = match[2].str();
				}
			}

			this->m_Handle.close();
		}

		return std::monostate{};
	}
} /* disxx::utility::ini */
