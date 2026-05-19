module;

#include <disconf.hpp>

#include <filesystem>
#include <numeric>
#include <cstdint>
#include <new>

export module disxx.loader.utility.MappedFile;

export import <bit>;
import :mapfile_t;

export namespace disxx::loader::utility
{
	class __DISXX_PRIVATE__ MappedFile final : private mapfile_t
	{
  	  private:
		void _ValidateRange(std::uint64_t, std::size_t) const noexcept(false);

  	  public:
		explicit MappedFile(void) noexcept;
		MappedFile(const MappedFile &) = delete;
		MappedFile &operator=(const MappedFile &) = delete;

		~MappedFile(void) noexcept(false); // <-- Destructor can throw exceptions!
		void MapFile(const std::filesystem::path &) noexcept(false);
		template <typename T> const T &Read(std::uint64_t offset) const noexcept(false);
	};

	template <typename T> const T &MappedFile::Read(std::uint64_t offset) const noexcept(false)
	{
	    this->_ValidateRange(offset, sizeof(T));
	
	    #ifdef __cpp_lib_start_lifetime_as
	        return *std::start_lifetime_as<const T>(this->mptr + offset);
	    #else
	        return *std::bit_cast<const T *>(std::add_sat(std::bit_cast<std::uint64_t>(this->mptr), offset));
	   	#endif
	}
} /* disxx::loader::utility */
