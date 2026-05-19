module;

#include <mach-o/loader.h>

#include <unordered_map>
#include <string_view>
#include <cstdint>
#include <vector>
#include <tuple>

export module disxx.loader.utility.BinaryInfo;

export namespace disxx::loader::utility
{
	/*
	 * This class contains build tools names, platform names and
	 * its version, for different executable formats (in future)
	 */
	class BinaryInfo
	{
	  private:
		const static std::unordered_map<std::uint32_t, std::string_view> s_PlatformNameTable;
		const static std::unordered_map<std::uint32_t, std::string_view> s_BuildToolNameTable;
	
		// OS name, OS version
		std::pair<std::uint32_t, std::uint32_t> m_PlatformInfo;
		// Build tool name, build tool version
		std::vector<std::pair<std::uint32_t, std::uint32_t>> m_BuildTools;
		std::uint32_t m_SDKVersion;

	  private:
		static inline std::tuple<std::uint16_t, std::uint8_t, std::uint8_t> __DecodeVersion(std::uint32_t) noexcept;

	  public:
		explicit BinaryInfo(void) noexcept;
		BinaryInfo(const BinaryInfo &) noexcept				= default;
		BinaryInfo &operator=(const BinaryInfo &) noexcept	= default;
		~BinaryInfo(void) noexcept							= default;

		void SetBuildVersion(const build_version_command &)									noexcept;
		void SetBuildTool(const build_tool_version &)										noexcept;

		std::string_view GetPlatformName(void)												const noexcept;
		std::string GetPlatformMinVersion(void)												const noexcept;
		std::vector<std::pair<std::string_view, std::string>> GetBuildTools(void)			const noexcept;
		std::string GetSDKVersion(void)														const noexcept;
	};

	inline std::tuple<std::uint16_t, std::uint8_t, std::uint8_t> BinaryInfo::__DecodeVersion(std::uint32_t version) noexcept
    {
        return std::make_tuple
        (
            (version >> 16) & 0xFFFF,
            (version >> 8) & 0xFF,
            version & 0xFF
        );
    }
} /* disxx::loader::utility */
