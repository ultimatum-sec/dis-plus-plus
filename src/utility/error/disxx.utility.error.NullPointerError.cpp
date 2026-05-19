module disxx.utility.error.NullPointerError;

namespace disxx::utility::error
{
	NullPointerError::NullPointerError(const char *str) noexcept
		: std::runtime_error{str}
	{}

	NullPointerError::NullPointerError(const std::string &str) noexcept
		: std::runtime_error{str}
	{}
} /* disxx::util::error */
