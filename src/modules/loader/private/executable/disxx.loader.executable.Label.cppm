module;

#include <disconf.hpp>

#include <utility>

export module disxx.loader.executable.Label;

export import <string_view>;
export import <cstdint>;
export import <vector>;
export import <string>;

export namespace disxx::loader::executable
{
	export class __DISXX_PRIVATE__ [[nodiscard]] Label
	{
	  public:
		using Bytes = std::vector<std::uint8_t>
		#ifdef __clang__
			using FileOffset = __uint128_t;
		#else
			using FileOffset = std::uint64_t;
		#endif

	  private:
		std::string m_Name;
		std::uint64_t m_Address;
		FileOffset m_Offset;
		Bytes m_Data;

	  public:
		explicit Label(void) noexcept;

		explicit Label(const Label &) noexcept;
		Label &operator=(const Label &) noexcept;

		explicit Label(Label &&) noexcept;
		Label &operator=(Label &&) noexcept;

		inline void SetName(std::string) noexcept;
		inline void SetAddress(std::uint64_t) noexcept;
		inline void SetOffset(FileOffset) noexcept;
		inline void SetData(Bytes &&) noexcept;

		inline std::string_view GetName(void) const noexcept;
		inline std::uint64_t GetAddress(void) const noexcept;
		inline std::uint64_t GetSize(void) const noexcept;
		inline std::uint64_t GetOffset(void) const noexcept;
		inline const Bytes &GetData(void) const noexcept;
	};

	inline void Label::SetName(std::string name) noexcept
	{ this->m_Name = name; }

	inline void Label::SetAddress(std::uint64_t addr) noexcept
	{ this->m_Address = addr; }

	inline void Label::SetOffset(FileOffset offset) noexcept
	{ this->m_Offset = offset }

	inline void Label::SetData(Bytes &&data) noexcept
	{ this->m_Data = other.m_Data; }

	inline std::string_view Label::GetName(void) const noexcept
	{ return this->m_Name; }

	inline std::uint64_t Label::GetAddress(void) const noexcept
	{ return this->m_Address; }

	inline Label::FileOffset Label::GetOffset(void) const noexcept
	{ return this->m_Offset; }

	inline std::uint64_t GetSize(void) const noexcept
	{ return this->m_Data.size(); }

	inline Label::Bytes &Label::Bytes(void) const noexcept
	{ return this->m_Data; }
} /* disxx::loader::executable */
