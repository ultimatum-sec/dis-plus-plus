module;

#include <disconf.hpp>

#include <filesystem>
#include <numeric>
#include <cstdint>
#include <new>

#include "mapped.h"

export module disxx.utility.file.MappedFile;

export import <bit>;

export namespace disxx::utility::file
{
	class __DISXX_EXPORT__ [[nodiscard]] MappedFile final : private mapped_t
	{
	  private:
		std::filesystem::path m_Path{};

  	  private:
		void ValidateRange(std::uint64_t, std::size_t) const noexcept(false);

  	  public:
		explicit MappedFile(void) noexcept;
		explicit MappedFile(const std::filesystem::path &) noexcept;

		MappedFile(const MappedFile &) noexcept;
		MappedFile &operator=(const MappedFile &) noexcept;

		MappedFile(MappedFile &&) noexcept;
		MappedFile &operator=(MappedFile &&) noexcept;

		~MappedFile(void) noexcept(false);
		
		void Open(const std::filesystem::path &) noexcept(false);
		void Open(void) noexcept(false);

		template <typename T> const T &Read(std::uint64_t offset) const noexcept(false);
	};

	template <typename T> const T &MappedFile::Read(std::uint64_t offset) const noexcept(false)
	{
	    this->ValidateRange(offset, sizeof(T));
	
	    #ifdef __cpp_lib_start_lifetime_as
	        return *std::start_lifetime_as<const T>(this->mptr + offset);
	    #else
	        return *std::bit_cast<const T *>(std::add_sat(std::bit_cast<std::uint64_t>(this->mptr), offset));
	   	#endif
	}
} /* disxx::loader::utility */
