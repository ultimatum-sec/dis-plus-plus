module;

#include <system_error>
#include <filesystem>
#include <stdexcept>
#include <cstdint>
#include <memory>
#include <new>

module disxx.loader.utility.MappedFile;

namespace disxx::loader::utility
{
	MappedFile::MappedFile(void) noexcept
		: mapfile_t{nullptr, 0}
	{}

	MappedFile::~MappedFile(void) noexcept(false)
	{
		if (this->mptr)
		{
			// Just throw en exception if ptr won't be freed to
			// avoid a HUGE MEMORY LEAK (depending on file size)!
			if (mclose() == -1) [[unlikely]]
				throw std::runtime_error{"FileUnmappingError"};
		}
	}
	
	void MappedFile::_ValidateRange(std::uint64_t offset, std::size_t size) const noexcept(false)
	{
		if (!size) [[unlikely]]
			throw std::range_error{"ZeroReadSizeError"};
		else if (offset > this->msize || this->msize - offset < size) [[unlikely]]
			throw std::out_of_range{"FileOutOfRangeError"};
	}
	
	void MappedFile::MapFile(const std::filesystem::path &rPath) noexcept(false)
	{
		// Validating file path
		if (std::error_code errc{}; !std::filesystem::exists(rPath, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"FileNotFoundError", errc};
		else if (std::filesystem::is_directory(rPath, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"DirectoryError", errc};
	
		auto pMapfile{mopen(const_cast<char *>(rPath.c_str()))};
		if (!pMapfile) [[unlikely]]
			throw std::bad_alloc{};
		this->mptr = pMapfile->mptr;
		this->msize = pMapfile->msize;
	}
} /* disxx::loader::utility */
