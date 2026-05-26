module;

#include <disconf.hpp>

#include <filesystem>
#include <fstream>
#include <map>

export module disxx.utility.ini.Parser;

export namespace disxx::utility::ini
{
	class __DISXX_PRIVATE__ [[nodiscard]] Parser
	{
	  public:
		using SectionsTable = std::map
		<
			// Section's name
			std::string,
			// Section's keys and values
			std::map
			<
				std::string,
				std::string
			>
		>;

	  protected:
		std::fstream m_Handle;
		SectionsTable m_Table;

	  public:
		Parser(void) noexcept = default;
		Pasrser(const std::filesystem::path &) noexcept;
	
		Parser(const Parser &) noexcept;
		Parser &operator=(const Parser &) noexcept;

		Parser(Parser &&) noexcept;
		Parser &operator=(Parser &&) noexcept;
	
		virtual ~Parser(void) noexcept = default;

		std::expected<std::monostate, disxx::utility::error::ParserError>
		Load(const std::filesystem::path &) noexcept;
	};
} /* disxx::utility::ini */
