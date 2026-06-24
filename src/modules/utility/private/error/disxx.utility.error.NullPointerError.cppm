module;

#include <disconf.hpp>

export module disxx.utility.error.NullPointerError;

export import <stdexcept>;
export import <string>;

export namespace disxx::utility::error
{
	class __DISXX_EXPORT__ [[nodiscard]] NullPointerError : public std::runtime_error
	{
	  public:
		explicit NullPointerError(const char *) noexcept;
		explicit NullPointerError(const std::string &) noexcept;

		virtual ~NullPointerError(void) noexcept override = default;
	};
} /* disxx::disasm::util */
