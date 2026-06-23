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
import disxx.loader.utility.MappedFile;

export namespace disxx::loader::macho
{
	class __DISXX_EXPORT__ [[nodiscard]] Loader
	{
	  private:
		// This fields ALWAYS must be initialised, or UB will be occrued!
		disxx::loader::utility::MappedFile m_Mapper{};
		mach_header_64 *m_pHeader{nullptr};
		std::uint32_t m_Offset{0};

	  public:
		explicit Loader(void) noexcept(false);
		
		explicit Loader(const Loader &) = delete;
		Loader &operator=(const Loader &) = delete;

		~Loader(void) noexcept(false);
		
		void LoadFile(const std::filesystem::path &) noexcept(false);
		disxx::loader::executable::ExecutableFile LoadData(void) const noexcept(false);
		disxx::loader::utility::BinaryInfo LoadMetadata(void) const noexcept(false);
		std::optional<std::uint64_t> LoadImageBase(void) const noexcept(false);

		std::string_view GetFileType(void) const noexcept(false);
	};
} /* disxx::loader::macho */
