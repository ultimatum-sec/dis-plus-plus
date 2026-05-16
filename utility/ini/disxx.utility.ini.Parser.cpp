module;

#include <fstream>
#include <ranges>
#include <format>
#include <regex>

#define CHECKF(f) \
	if (!(f)) [[unlikely]] \
		return std::unexpected{disxx::utility::error::ParserError{"FileError"}}

#define CHECKR(r) \
	if (!(r)) [[unlikely]] \
		return std::unexpected{(r).error()}

#include <print>

module disxx.utility.ini.Parser;

namespace disxx::utility::ini
{
	Parser::Parser(void) noexcept = default;

	Parser::Parser(const Parser &other) noexcept
		: m_Path{other.m_Path}
		, m_Handle{}
	{}

	Parser &Parser::Parser::operator=(const Parser &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Path = other.m_Path;

		return *this;
	}

	std::pair<std::string, std::string> Parser::__SplitSectionWithKey(std::string_view str) noexcept(false)
	{
		std::string __str{str.data()};
		std::smatch matches{};
		std::regex_search
		(
			__str, // This function requires std::string as its first argument
			matches,
			std::regex{R"((\S+)\.(\S+))"}
		);

		return std::make_pair(matches[1].str(), matches[2].str());
	}
	
	std::expected<std::string, disxx::utility::error::ParserError> Parser::__FindValue(std::string_view str) noexcept
	{
		CHECKF(this->m_Handle);

		bool sectionFound{false};
		const auto &[section, key]{Parser::__SplitSectionWithKey(str)};
	
		while (!this->m_Handle.eof())
		{
			std::string line{};
			std::getline(this->m_Handle, line);
			// Check if it's section
			if (line.starts_with(";"))
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
				std::regex{R"(;\S+$)"},
				std::string{}
			);

			if (std::smatch matches{}; std::regex_search(line, matches, std::regex{R"(\[(\S+)\])"}))
				sectionFound = matches[1].str() == section;
			else
			{
				std::smatch other{};
				std::regex_search
				(
					line,
					other,
					std::regex{R"((\S+)=(\S+))"}
				);

				if (other[1].str() == key && sectionFound)
				{
					this->m_Handle.seekg(0);
					return other[2].str();
				}
			}
		}

		this->m_Handle.seekg(0);

		return std::unexpected{disxx::utility::error::ParserError{"ElementError"}};
	}

	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::__AddValue(std::string_view name, std::string_view val) noexcept
	{
		CHECKF(this->m_Handle);

		const auto &[section, key]{Parser::__SplitSectionWithKey(name)};

		auto &&contents
		{
			[this](void) -> std::string
			{
				// Using such kind of reading because
				// std::istream_iterator<char>() eats '\n'

				std::string buff{};
				while (!this->m_Handle.eof())
				{
					std::string line{};
					std::getline(this->m_Handle, line);
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
						std::regex{R"(;\S+$)"},
						std::string{}
					);

					buff += line + '\n';
				}
			
				this->m_Handle.seekg(0);

				return buff;
			}()
		};

		// Check if the key has already been added (or assigned)
		if (const auto &result{Parser::__FindValue(name)})
		{
			std::println("Already got the value!");
			if (result.value() != val)
			{
				contents = std::regex_replace
				(
					contents,
					std::regex{key + std::string{"="} + result.value()},
					std::format("{}={}\n", key, val)
				);
			}
		}
		else if (const auto &formatted{std::format("[{}]", section)}; contents.find(formatted) != std::string::npos)
		{
			std::println("Already got the section!");
			contents = std::regex_replace
			(
				contents,
				std::regex{formatted},
				std::format("{}\n{}={}\n", formatted, key, val)
			);
		}
		else
		{
			std::println("Adding the new section!");
			contents += std::format
			(
				"[{}]\n{}={}\n",
				section,
				key,
				val
			);
		}

		// Truncate the file
		this->m_Handle.close();
		this->m_Handle.open
		(
			this->m_Path,
			std::fstream::out
				| std::fstream::in
				| std::fstream::trunc
				| std::fstream::binary
		);
		CHECKF(this->m_Handle);

		for (const auto ch : contents)
			this->m_Handle.write(&ch, sizeof(ch));

		return std::monostate{};
	}

	std::expected<std::monostate, disxx::utility::error::ParserError>
	Parser::LoadFile(const std::filesystem::path path) noexcept
	{
		this->m_Path = path; 
		this->m_Handle.open
		(
			this->m_Path,
			std::fstream::out
				| std::fstream::in
				| std::fstream::binary
		);
		CHECKF(this->m_Handle);

		return std::monostate{};
	}

	template <>
	std::expected<bool, disxx::utility::error::ParserError> Parser::Get<bool>(std::string_view key) noexcept
	{
		const auto result{Parser::__FindValue(key)};
		CHECKR(result);

		if (const auto &val{result.value()}; val == "true" || val == "false") [[likely]]
			return val == "true";

		return std::unexpected{disxx::utility::error::ParserError{"ValueError"}};
	}

	template <>
	std::expected<std::string, disxx::utility::error::ParserError> Parser::Get<std::string>(std::string_view key) noexcept
	{
		const auto result{Parser::__FindValue(key)};
		CHECKR(result);

		return result.value();
	}

	template <>
	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::Set<bool>(std::string_view key, bool value) noexcept
	{
		CHECKR(Parser::__AddValue(key, value ? "true" : "false"));

		return std::monostate{};
	}
		
	template <>
	std::expected<std::monostate, disxx::utility::error::ParserError> Parser::Set<std::string_view>(std::string_view key, std::string_view value) noexcept
	{
		CHECKR(Parser::__AddValue(key, value));
	
		return std::monostate{};
	}
} /* disxx::utility::ini */
