module;

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <optional>
#include <numeric>
#include <cstdint>
#include <memory>
#include <ranges>
#include <vector>
#include <string>

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld.h>
#include <mach-o/fat.h>

// For ntohl(1)
#ifndef _WIN32
#	include <arpa/inet.h>
#else
#	include <winsock2.h>
#endif

module disxx.loader.macho.Loader;

import disxx.loader.utility.Section;

namespace disxx::loader::macho
{
	Loader::Loader(void) noexcept(false)
		: m_pHeader{new mach_header_64}
	{}

	Loader::~Loader(void) noexcept(false)
	{
		std::destroy_at(&this->m_Mapper);
		if (this->m_pHeader) [[likely]]
			delete this->m_pHeader;
	}

	void Loader::LoadFile(const std::filesystem::path &rPath) noexcept(false)
	{
		this->m_Mapper.MapFile(rPath);
		// I really like that 0xCAFEBABE magic number :D
		if (this->m_Mapper.Read<std::uint32_t>(0) == FAT_CIGAM)
		{
			auto hdr{this->m_Mapper.Read<fat_header>(0)};
			std::vector<fat_arch> vec{ntohl(hdr.nfat_arch)};
			for (auto i{sizeof(fat_header)}; i < ntohl(hdr.nfat_arch) * sizeof(fat_arch) + sizeof(fat_header); i += sizeof(fat_arch))
			{
				vec.emplace_back
				(
					[](const auto &arch) -> fat_arch
					{
						// Translating integers to little endian byte order
						return fat_arch
						{
							static_cast<std::int32_t>(ntohl(arch.cputype)),
							static_cast<std::int32_t>(ntohl(arch.cpusubtype)),
							ntohl(arch.offset),
							ntohl(arch.size),
							ntohl(arch.align)
						};
					}(this->m_Mapper.Read<fat_arch>(i))
				);
			}
	
			auto it 
			{
				std::ranges::find_if
				(
					vec,
					[](const auto &arch) -> bool
					{ return arch.cputype == CPU_TYPE_ARM64; }
				)
			};
			
			if (it == vec.end()) [[unlikely]]
				throw std::invalid_argument{"CPUArchError"};
			this->m_Offset = it->offset;
		}
		
		*(this->m_pHeader) = this->m_Mapper.Read<mach_header_64>(this->m_Offset);
		if (this->m_pHeader->magic != MH_MAGIC_64) [[unlikely]]
			throw std::invalid_argument{"FileFormatError"}; // Only 64-bit mach objects!
		else if (this->m_pHeader->cputype != CPU_TYPE_ARM64) [[unlikely]]
			throw std::invalid_argument{"CPUArchError"}; // Can dissasemble only aarch64 instructions!
	}

	std::vector<disxx::loader::utility::Section> Loader::LoadSections(void) const noexcept(false)
	{
		std::vector<disxx::loader::utility::Section> sections;
		auto sectIdx{1ULL};
		load_command loadCmd;
		auto offset{this->m_Offset + sizeof(mach_header_64)};
		for (const auto &_ : std::views::iota(0U, this->m_pHeader->ncmds))
		{
			loadCmd = this->m_Mapper.Read<load_command>(offset);
			if (loadCmd.cmd == LC_SEGMENT_64)
			{
				auto segCmd{this->m_Mapper.Read<segment_command_64>(offset)};
				auto pSects{std::make_unique<section_64[]>(segCmd.nsects)};
				for (const auto &j : std::views::iota(0U, segCmd.nsects))
				{
					auto nsect{this->m_Mapper.Read<section_64>((offset + sizeof(segCmd)) + (j * sizeof(section_64)))};
					if (!nsect.offset) [[unlikely]]
						continue;

					sections.emplace_back
					(
						__uint128_t{nsect.addr},
						__uint128_t{this->m_Offset + nsect.offset},
						__uint128_t{nsect.size},
						std::string{nsect.segname} + std::string{","}
							+ std::string{nsect.sectname},
						sectIdx++
					);
				}
			}
			else if (loadCmd.cmd == LC_SYMTAB)
			{
				auto symtabCmd{this->m_Mapper.Read<symtab_command>(offset)};
				auto pSymbols{std::make_unique<nlist_64[]>(symtabCmd.nsyms)};
				for (const auto &j : std::views::iota(0U, symtabCmd.nsyms))
					pSymbols[j] = this->m_Mapper.Read<nlist_64>(symtabCmd.symoff + j * sizeof(nlist_64) + this->m_Offset);

				auto pStrtab{std::make_unique<char[]>(symtabCmd.strsize)};
				for (const auto j : std::views::iota(0U, symtabCmd.strsize))
					pStrtab[j] = this->m_Mapper.Read<char>(symtabCmd.stroff + j + this->m_Offset);

				for (const auto &j : std::views::iota(0U, symtabCmd.nsyms))
				{
					if (!pSymbols[j].n_un.n_strx)
						continue;
					else if ((pSymbols[j].n_type & N_TYPE) != N_SECT || (pSymbols[j].n_type & N_STAB))
						continue;
					
					auto it
					{
						// Find the section with the same number as an argument
						std::ranges::find_if
						(
							sections,
							[&pSymbols, j](const auto &section) -> bool
							{ return section.GetSectionNumber() == pSymbols[j].n_sect; }
						)
					};
				
					// If not found, just pass it
					if (it == sections.end()) [[unlikely]]
						continue;
	
					auto start{0ULL};
					if (pSymbols[j].n_value >= it->GetStart() && pSymbols[j].n_value < it->GetVirtualStart() + it->GetSize())
						start = it->GetStart() + (pSymbols[j].n_value - it->GetVirtualStart());

					it->GetLabelsAndData().emplace_back
					(
						std::make_pair<disxx::loader::utility::func_t, std::vector<std::uint8_t>>
						(
							disxx::loader::utility::func_t
							{
								&pStrtab[pSymbols[j].n_un.n_strx],
								pSymbols[j].n_value,
								start,
								0 // The size will be calculated (guessed) later
							},
							std::vector<std::uint8_t>{} // Should be filled later
						)
					);
				}

				break;
			}
			
			offset += loadCmd.cmdsize;
		}

		// Sort sections
		std::ranges::sort
		(
			sections,
			[](const auto &a, const auto &b) -> bool
			{ return a.GetStart() < b.GetStart(); }
		);
		
		// Sort labels in each section
		std::ranges::for_each
		(
			sections,
			[](disxx::loader::utility::Section &section) -> void
			{
				std::sort
				(
					section.GetLabelsAndData().begin(),
					section.GetLabelsAndData().end(),
					[](const auto &a, const auto &b) -> bool
					{ return a.first.addr < b.first.addr; }
				);
			}
		);
		
		// Calculating (or guessing) labels sizes
		std::ranges::for_each
		(
			sections,
			[](auto &section) -> void
			{	
				for (const auto &i : std::views::iota(0UL, section.GetLabelsAndData().size()))
				{
					if (i < section.GetLabelsAndData().size() - 1)
						section.GetLabelsAndData().at(i).first.size = section.GetLabelsAndData().at(i + 1).first.addr
							- section.GetLabelsAndData().at(i).first.addr;
					else
					{
						auto addr{section.GetStart() + section.GetSize()};
						section.GetLabelsAndData().at(i).first.size = addr
							- section.GetLabelsAndData().at(i).first.addr;
					}
				}
			}
		);
	
		return sections;
	}
	
	void Loader::LoadSectionData(disxx::loader::utility::Section &section) const noexcept(false)
	{
		for	(auto &[label, data] : section.GetLabelsAndData())
		{
			data.resize(label.size);
			for (const auto &i : std::views::iota(label.addr, label.addr + label.size))
				data.at(i - label.addr) = this->m_Mapper.Read<std::uint8_t>(i);
		}	
	}

	disxx::loader::utility::BinaryInfo Loader::LoadMetadata(void) const noexcept(false)
	{
		disxx::loader::utility::BinaryInfo metadata;

		load_command loadCmd;
		std::uint64_t offset{this->m_Offset + sizeof(mach_header_64)};
		for (const auto &_ : std::views::iota(0U, this->m_pHeader->ncmds))
		{
			loadCmd = this->m_Mapper.Read<load_command>(offset);
			if (loadCmd.cmd == LC_BUILD_VERSION)
			{
				auto cmd{this->m_Mapper.Read<build_version_command>(offset)};
				metadata.SetBuildVersion(cmd);
				
				for (auto index{offset + sizeof(build_version_command)}; index < offset + cmd.cmdsize; index += sizeof(build_tool_version))
					metadata.SetBuildTool(this->m_Mapper.Read<build_tool_version>(index));
				return metadata;
			}

			offset += loadCmd.cmdsize;
		}

		return metadata;
	}

	std::optional<std::uint64_t> Loader::LoadImageBase(void) const noexcept(false)
	{
		load_command loadCmd;
		std::uint64_t offset{this->m_Offset + sizeof(mach_header_64)};
		for (const auto &_ : std::views::iota(0U, this->m_pHeader->ncmds))
		{
			loadCmd = this->m_Mapper.Read<load_command>(offset);
			if (loadCmd.cmd == LC_SEGMENT_64)
				if (auto cmd{this->m_Mapper.Read<segment_command_64>(offset)}; std::string{cmd.segname} == "__TEXT")
					return cmd.vmaddr - cmd.fileoff;
			offset += loadCmd.cmdsize;
		}

		return std::nullopt;
	}

	std::string_view Loader::GetFileType(void) const noexcept(false)
	{
		const static std::unordered_map<std::uint32_t, std::string_view> fileTypeTable = {
			{MH_OBJECT,		"relocatable object file"},
			{MH_EXECUTE,	"demand paged executable file"},
			{MH_CORE,		"core file"},
			{MH_DYLIB,		"dynamically bound shared library"}
		};

		if (!this->m_pHeader) [[unlikely]]
			return "unknown";
		else if (!fileTypeTable.contains(this->m_pHeader->filetype)) [[unlikely]]
			return "unknown";
		else [[likely]]
			return fileTypeTable.at(this->m_pHeader->filetype);
	}
} /* disxx::loader::macho */
