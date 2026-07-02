module;

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <optional>
#include <numeric>
#include <cstdint>
#include <memory>
#include <ranges>
#include <format>
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

import disxx.loader.executable.ExecutableFile;

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

	disxx::loader::executable::ExecutableFile Loader::LoadData(void) const noexcept(false)
	{
		disxx::loader::executable::ExecutableFile exec{};
		exec.SetMagic(this->m_pHeader->magic);

		unsigned short int sectionIndex{1};

		load_command loadCmd;
		auto offset{this->m_Offset + sizeof(mach_header_64)};
		for (const auto _ : std::views::iota(0u, this->m_pHeader->ncmds))
		{
			loadCmd = this->m_Mapper.Read<load_command>(offset);
			if (loadCmd.cmd == LC_SEGMENT_64)
			{
				auto segCmd{this->m_Mapper.Read<segment_command_64>(offset)};
				auto pSects{std::make_unique<section_64[]>(segCmd.nsects)};
				for (const auto j : std::views::iota(0u, segCmd.nsects))
				{
					auto nsect{this->m_Mapper.Read<section_64>((offset + sizeof(segCmd)) + (j * sizeof(section_64)))};
					if (!nsect.offset) [[unlikely]]
					{
						sectionIndex++;
						continue;
					}

					disxx::loader::executable::Section section{};
					section.SetName
					(
						std::format
						(
							"{},{}",
							// Cut null-terminators by wrapping ptr into str
							std::string{nsect.segname},
							// The same thing here
							std::string{nsect.sectname}
						)
					);
					section.SetAddress(nsect.addr);
					section.SetOffset(this->m_Offset + nsect.offset);
					section.SetSize(nsect.size);
					section.SetIndex(sectionIndex++);
					
					exec.AddSection(std::move(section));
				}
			}
			else if (loadCmd.cmd == LC_SYMTAB)
			{
				auto symtabCmd{this->m_Mapper.Read<symtab_command>(offset)};
				auto pSymbols{std::make_unique<nlist_64[]>(symtabCmd.nsyms)};
				for (const auto j : std::views::iota(0u, symtabCmd.nsyms))
					pSymbols[j] = this->m_Mapper.Read<nlist_64>(symtabCmd.symoff + j * sizeof(nlist_64) + this->m_Offset);

				auto pStrtab{std::make_unique<char[]>(symtabCmd.strsize)};
				for (const auto j : std::views::iota(0u, symtabCmd.strsize))
					pStrtab[j] = this->m_Mapper.Read<char>(symtabCmd.stroff + j + this->m_Offset);

				for (const auto j : std::views::iota(0u, symtabCmd.nsyms))
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
							exec.GetSections(),
							[&pSymbols, j](const auto &section) -> bool
							{ return section.GetIndex() == pSymbols[j].n_sect; }
						)
					};
				
					// If not found, just pass it
					if (it == exec.GetSections().end()) [[unlikely]]
						continue;
	
					auto start{0ull};
					if (const auto addr{it->GetAddress()}; pSymbols[j].n_value >= addr && pSymbols[j].n_value < addr + it->GetSize())
						start = it->GetOffset() + (pSymbols[j].n_value - addr);
					
					disxx::loader::executable::Label label{};
					label.SetName(&pStrtab[pSymbols[j].n_un.n_strx]);
					label.SetAddress(pSymbols[j].n_value);
					label.SetOffset(start);

					it->AddLabel(std::move(label));
				}

				break;
			}
			
			offset += loadCmd.cmdsize;
		}

		for (auto &sect : exec.GetSections())
		{
			for (auto it{sect.GetLabels().begin()}; it != sect.GetLabels().end(); ++it)
			{
				std::vector<uint8_t> data{};
				for (const auto i : std::views::iota(it->GetOffset(), std::next(it) != sect.GetLabels().end() ? std::next(it)->GetOffset() : sect.GetSize() + sect.GetOffset()))
					data.emplace_back(this->m_Mapper.Read<std::uint8_t>(i));
				it->SetData(std::move(data));
			}
		}

		return disxx::loader::executable::ExecutableFile{exec};
	}

	disxx::loader::utility::BinaryInfo Loader::LoadMetadata(void) const noexcept(false)
	{
		disxx::loader::utility::BinaryInfo metadata{};

		load_command loadCmd;
		std::uint64_t offset{this->m_Offset + sizeof(mach_header_64)};
		for (const auto _ : std::views::iota(0u, this->m_pHeader->ncmds))
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
		for (const auto _ : std::views::iota(0u, this->m_pHeader->ncmds))
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
