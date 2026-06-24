module;

#include <disconf.hpp>

#include <exception>
#include <string>

export module disxx.utility.error.ParserError;

export namespace disxx::utility::error
{
	class __DISXX_EXPORT__ [[nodiscard]] ParserError final : public std::exception
	{
	  private:
		std::string m_Error{};

	  public:
		explicit ParserError(void) noexcept;
		explicit ParserError(std::string) noexcept;

		ParserError(const ParserError &) noexcept = default;
		ParserError &operator=(const ParserError &) noexcept = default;

		virtual ~ParserError(void) noexcept override = default;

		virtual const char *what(void) const noexcept override;
	};
} /* disxx::utility::error */
