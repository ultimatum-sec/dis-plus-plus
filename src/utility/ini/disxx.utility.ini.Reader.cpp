module disxx.utility.ini.Reader;

namespace disxx::utility::ini
{
	Reader::Reader(void) noexcept
		: BaseParser{}
	{}

	Reader::Reader(const std::filesystem::path &path) noexcept
		: BaseParser{path}
	{}

	Reader::Reader(const Reader &other) noexcept
		: BaseParser{other}
	{}

	Reader &Reader::operator=(const Reader &other) noexcept
	{
		BaseParser::operator=(other);
		return *this;
	}

	Reader::Reader(Reader &&other) noexcept
		: BaseParser{std::move(other)}
	{}

	Reader &Reader::operator=(Reader &&other) noexcept
	{
		BaseParser::operator=(std::move(other));
		return *this;
	}
} /* disxx::utility::ini */
