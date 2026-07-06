module;

#include <system_error>
#include <filesystem>
#include <stdexcept>
#include <utility>
#include <cstdint>
#include <memory>
#include <limits>
#include <new>

#include "../../modules/utility/public/file/mapped.h"

module disxx.utility.file.MappedFile;

namespace disxx::utility::file
{
	MappedFile::MappedFile(void) noexcept
		: mapped_t{nullptr, 0ull}
		, m_Path{}
	{}

	MappedFile::MappedFile(const MappedFile &other) noexcept
		: mapped_t{nullptr, 0ull}
		, m_Path{other.m_Path}
	{ this->Open(); }

	MappedFile &MappedFile::operator=(const MappedFile &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Path = other.m_Path;
			this->Open();
		}
	
		return *this;
	}

	MappedFile::MappedFile(MappedFile &&other) noexcept
		: mapped_t{std::forward<MappedFile &&>(other)}
		, m_Path{std::move(other.m_Path)}
	{}

	MappedFile &MappedFile::operator=(MappedFile &&other) noexcept
	{
		mapped_t::operator=(std::forward<MappedFile &&>(other));
		this->m_Path = std::move(other.m_Path);

		return *this;
	}

	MappedFile::~MappedFile(void) noexcept(false)
	{
		if (this->mptr)
		{
			// Just throw en exception if ptr won't be freed to
			// avoid a HUGE MEMORY LEAK (depending on file size)!
			if (mclose(static_cast<mapped_t>(*this)) == -1) [[unlikely]]
				throw std::runtime_error{"FileUnmappingError"};
		}
	}
	
	void MappedFile::ValidateRange(std::uint64_t offset, std::size_t size) const noexcept(false)
	{
		if (!size) [[unlikely]]
			throw std::range_error{"ReadZeroSizeError"};
		else if (offset > this->msize || this->msize - offset < size) [[unlikely]]
			throw std::out_of_range{"ReadOutOfBoundsError"};
	}
	
	void MappedFile::Open(const std::filesystem::path &path) noexcept(false)
	{
		// Validating file path
		if (std::error_code errc{}; !std::filesystem::exists(path, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"FileNotFoundError", errc};
		else if (std::filesystem::is_directory(path, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"DirectoryError", errc};
	
		auto pMapped{mopen(path.c_str())};
		if (!pMapped.mptr) [[unlikely]]
			throw std::bad_alloc{};
		this->mptr = pMapped.mptr;
		this->msize = pMapped.msize;
	}

	void MappedFile::Open(void) noexcept(false)
	{
		// Validating file path
		if (std::error_code errc{}; !std::filesystem::exists(this->m_Path, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"FileNotFoundError", errc};
		else if (std::filesystem::is_directory(this->m_Path, errc)) [[unlikely]]
			throw std::filesystem::filesystem_error{"DirectoryError", errc};
	
		auto pMapped{mopen(this->m_Path.c_str())};
		if (!pMapped.mptr) [[unlikely]]
			throw std::bad_alloc{};
		this->mptr = pMapped.mptr;
		this->msize = pMapped.msize;
	}
} /* disxx::loader::utility */
