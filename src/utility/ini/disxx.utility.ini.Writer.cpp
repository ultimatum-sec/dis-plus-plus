module disxx.utility.ini.Writer;

namespace disxx::utility::ini
{
	Writer::Writer(void) noexcept
		: BaseParser{}
	{}

	Writer::Writer(const std::filesystem::path &path) noexcept
		: BaseParser{path}
	{}

	Writer::Writer(const Writer &other) noexcept
		: BaseParser{other}
	{}

	Writer &Writer::operator=(const Writer &other) noexcept
	{
		BaseParser::operator=(other);
		return *this;
	}

	Writer::Writer(Writer &&other) noexcept
		: BaseParser{std::move(other)}
	{}

	Writer &Writer::operator=(Writer &&other) noexcept
	{
		BaseParser::operator=(std::move(other));
		return *this;
	}
} /* disxx::utility::ini */
