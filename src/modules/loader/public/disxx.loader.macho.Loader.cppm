module;

#include <disconf.hpp>

#include <filesystem>
#include <optional>
#include <cstdint>
#include <vector>

#include <mach-o/loader.h>
#include <mach-o/nlist.h>

export module disxx.loader.macho.Loader;

export import disxx.loader.executable.ExecutableFile;
export import disxx.loader.utility.BinaryInfo;
import disxx.utility.file.MappedFile;

export namespace disxx::loader::macho
{
	class __DISXX_EXPORT__ [[nodiscard]] Loader
	{
	  private:
		disxx::utility::file::MappedFile m_Mapper{};
		mach_header_64 *m_pHeader{nullptr};
		std::uint64_t m_Offset{0};

	  public:
		explicit Loader(void) noexcept;
		
		Loader(const Loader &) noexcept;
		Loader &operator=(const Loader &) noexcept;

		Loader(Loader &&) noexcept;
		Loader &operator=(Loader &&) noexcept;

		~Loader(void) noexcept;
		
		void LoadFile(const std::filesystem::path &) noexcept(false);
		disxx::loader::executable::ExecutableFile LoadData(void) const noexcept(false);
		disxx::loader::utility::BinaryInfo LoadMetadata(void) const noexcept(false);
		std::optional<std::uint64_t> LoadImageBase(void) const noexcept(false);

		std::string_view GetFileType(void) const noexcept(false);
	};
} /* disxx::loader::macho */
