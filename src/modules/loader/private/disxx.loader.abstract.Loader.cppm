export module;

#include <disconf.hpp>

export module disxx.loader.abstract.Loader;

export namespace disxx::loader::abstact
{
	class __DISXX_PRIVATE__ [[nodiscard]] Loader
	{
	  protected:
		MappedFile m_Mapper{};

	  public:
		explicit Loader(void) noexcept(false);

		explicit Loader(const Loader &) noexcept = delete;
		Loader &operator=(const Loader &) noexcept = delete;

		std::expected<std::monostate, std::runtime_error> LoadFile(const std::filesystem::path &) noexcept(false);
	};
} /* disxx::loader::abstact */
