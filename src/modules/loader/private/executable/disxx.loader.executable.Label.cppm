module;

#include <disconf.hpp>

#include <utility>

export module disxx.loader.executable.Label;

export import <string_view>;
export import <type_traits>;
export import <algorithm>;
export import <cstdint>;
export import <ranges>;
export import <vector>;
export import <string>;

export namespace disxx::loader::executable
{
	class __DISXX_PRIVATE__ [[nodiscard]] Label
	{
	  public:
		using Bytes = std::vector<std::uint8_t>;

	  private:
		std::string m_Name{};
		std::uint64_t m_Address{};
		std::uint64_t m_Offset{};
		Bytes m_Data{};

	  public:
		explicit Label(void) noexcept;

		Label(const Label &) noexcept;
		Label &operator=(const Label &) noexcept;

		explicit Label(Label &&) noexcept;
		Label &operator=(Label &&) noexcept;

		inline void SetName(std::string) noexcept;
		inline void SetAddress(std::uint64_t) noexcept;
		inline void SetOffset(std::uint64_t) noexcept;
		inline void SetData(Bytes &&) noexcept;

		inline std::string_view GetName(void) const noexcept;
		inline std::uint64_t GetAddress(void) const noexcept;
		inline std::uint64_t GetSize(void) const noexcept;
		inline std::uint64_t GetOffset(void) const noexcept;
		template <typename T> requires std::is_integral<T>::value
		inline std::vector<T> GetData(void) const noexcept;
	};

	inline void Label::SetName(std::string name) noexcept
	{ this->m_Name = name; }

	inline void Label::SetAddress(std::uint64_t addr) noexcept
	{ this->m_Address = addr; }

	inline void Label::SetOffset(std::uint64_t offset) noexcept
	{ this->m_Offset = offset; }

	inline void Label::SetData(Bytes &&data) noexcept
	{ this->m_Data = std::move(data); }

	inline std::string_view Label::GetName(void) const noexcept
	{ return this->m_Name; }

	inline std::uint64_t Label::GetAddress(void) const noexcept
	{ return this->m_Address; }

	inline std::uint64_t Label::GetOffset(void) const noexcept
	{ return this->m_Offset; }

	inline std::uint64_t Label::GetSize(void) const noexcept
	{ return this->m_Data.size(); }

	template <typename T> requires std::is_integral<T>::value
	inline std::vector<T> Label::GetData(void) const noexcept
	{
		if constexpr (std::is_same<T, std::uint8_t>::value)
			return this->m_Data;

		// Copy the vector
		auto copy{this->m_Data};
		if (copy.size() % sizeof(T) != 0) [[unlikely]]
			for (const auto _ : std::views::iota(0ul, sizeof(T) - (copy.size() % sizeof(T))))
				copy.emplace_back(0);
	
		std::vector<T> result{};
		result.resize(copy.size() / sizeof(T), static_cast<T>(0));
		for (auto i{0ul}; i < copy.size(); i += sizeof(T))
			for (auto j : std::views::iota(i, i + sizeof(T)))
				result.at(i / sizeof(T)) |= static_cast<T>(copy.at(j)) << (j - i) * 8;
		return result;
	}
} /* disxx::loader::executable */
