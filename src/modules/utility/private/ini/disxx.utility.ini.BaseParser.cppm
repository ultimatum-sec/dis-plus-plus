module;

#include <disconf.hpp>

export module disxx.utility.ini.BaseParser;

export import <filesystem>;
export import <expected>;
export import <variant>;
export import <fstream>;
export import <memory>;
export import <string>;
export import <map>;

export import disxx.utility.error.ParserError;

export namespace disxx::utility::ini
{
	class __DISXX_EXPORT__ [[nodiscard]] BaseParser
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
		std::filesystem::path m_Path;
		SectionsTable m_Table;

	  protected:
		static std::pair<std::string, std::string> __Split(std::string_view) noexcept;

	  public:
		BaseParser(void) noexcept;
		BaseParser(const std::filesystem::path &) noexcept;
	
		BaseParser(const BaseParser &) noexcept;
		BaseParser &operator=(const BaseParser &) noexcept;

		BaseParser(BaseParser &&) noexcept;
		BaseParser &operator=(BaseParser &&) noexcept;
	
		virtual ~BaseParser(void) noexcept = default;

		std::expected<std::monostate, disxx::utility::error::ParserError>
		Load(const std::filesystem::path &) noexcept;
	};
} /* disxx::utility::ini */
