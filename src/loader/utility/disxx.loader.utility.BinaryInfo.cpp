module;

#include <mach-o/loader.h>

#include <unordered_map>
#include <string_view>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <format>
#include <string>
#include <tuple>

module disxx.loader.utility.BinaryInfo;

namespace disxx::loader::utility
{
	const std::unordered_map<std::uint32_t, std::string_view> BinaryInfo::s_PlatformNameTable
	{
		{PLATFORM_UNKNOWN, 				"unknown"},
		{PLATFORM_ANY, 					"any"},
		{PLATFORM_MACOS,				"MacOS"},
		{PLATFORM_IOS, 					"IOS"},
		{PLATFORM_TVOS, 				"TVOS"},
		{PLATFORM_WATCHOS, 				"WatchOS"},
		{PLATFORM_BRIDGEOS, 			"BridgeOS"},
		{PLATFORM_MACCATALYST, 			"Mac Catalyst"},
		{PLATFORM_IOSSIMULATOR, 		"IOS (simulator)"},
		{PLATFORM_TVOSSIMULATOR, 		"TVOS (simulator)"},
		{PLATFORM_WATCHOSSIMULATOR, 	"WatchOS (simulator)"},
		{PLATFORM_DRIVERKIT, 			"Driver kit"},
		{PLATFORM_VISIONOS,				"VisionOS"},
		{PLATFORM_VISIONOSSIMULATOR, 	"VisionOS (simulator)"},
		{PLATFORM_FIRMWARE, 			"firmware"},
		{PLATFORM_SEPOS, 				"SepOS"},
		{PLATFORM_MACOS_EXCLAVECORE, 	"MacOS exclave core"},
		{PLATFORM_MACOS_EXCLAVEKIT,		"MacOS exclave kit"},
		{PLATFORM_IOS_EXCLAVECORE, 		"IOS exclave core"},
		{PLATFORM_IOS_EXCLAVEKIT, 		"IOS exclave kit"},
		{PLATFORM_TVOS_EXCLAVECORE, 	"TVOS exlcave core"},
		{PLATFORM_TVOS_EXCLAVEKIT, 		"TVOS exclave kit"},
		{PLATFORM_WATCHOS_EXCLAVECORE, 	"WatchOS exclave core"},
		{PLATFORM_WATCHOS_EXCLAVEKIT, 	"WatchOS exclave kit"},
		{PLATFORM_VISIONOS_EXCLAVECORE, "VisionOS exclave core"},
		{PLATFORM_VISIONOS_EXCLAVEKIT, 	"VisionOS exclave kit"}
	};

	const std::unordered_map<std::uint32_t, std::string_view> BinaryInfo::s_BuildToolNameTable
	{
		{TOOL_CLANG, 	"clang"},
		{TOOL_SWIFT,	"swift"},
		{TOOL_LD,		"ld"},
		{TOOL_LLD, 		"lld"}
	};

	BinaryInfo::BinaryInfo(void) noexcept
		: m_PlatformInfo{std::make_pair(0, 0)}
		, m_BuildTools{}
		, m_SDKVersion{0}
	{}

	void BinaryInfo::SetBuildVersion(const build_version_command &cmd) noexcept
	{
		this->m_PlatformInfo = std::make_pair(cmd.platform, cmd.minos);
		this->m_SDKVersion = cmd.sdk;
	}

	void BinaryInfo::SetBuildTool(const build_tool_version &tool) noexcept
	{
		this->m_BuildTools.emplace_back
		(
			std::make_pair
			(
				tool.tool,
				tool.version
			)
		);
	}

	std::string_view BinaryInfo::GetPlatformName(void) const noexcept
	{
		auto it{s_PlatformNameTable.find(this->m_PlatformInfo.first)};
		if (it == s_PlatformNameTable.end()) [[unlikely]]
			return s_PlatformNameTable.at(PLATFORM_UNKNOWN);
		return it->second;
	}

	std::string BinaryInfo::GetPlatformMinVersion(void) const noexcept
	{
		const auto &[major, minor, patchlevel] = __DecodeVersion(this->m_PlatformInfo.second);
		// It would be nice to return std::stirng_view,
		// but std::format returns std::string, so
		// it probably cause use after scope :(
		return std::format
		(
			"{}.{}.{}",
			major,
			minor,
			patchlevel
		);
	}

	std::string BinaryInfo::GetSDKVersion(void) const noexcept
	{
		const auto &[major, minor, patchlevel] = __DecodeVersion(this->m_SDKVersion);
		return std::format
		(
			"{}.{}.{}",
			major,
			minor,
			patchlevel
		);
	}

	std::vector<std::pair<std::string_view, std::string>> BinaryInfo::GetBuildTools(void) const noexcept
	{
		std::vector<std::pair<std::string_view, std::string>> out;
		for (const auto &info : this->m_BuildTools)
		{
			std::string_view name{"unknown"};
			if(auto it{s_BuildToolNameTable.find(info.first)}; it != s_BuildToolNameTable.end())
				name = it->second;
			const auto &[major, minor, patchlevel] = __DecodeVersion(info.second);
			out.emplace_back
			(
				std::make_pair
				(
					name,
					std::format
					(
						"{}.{}.{}",
						major,
						minor,
						patchlevel
					)
				)
			);
		}

		return out;
	}
} /* disxx::loaders::utils */


