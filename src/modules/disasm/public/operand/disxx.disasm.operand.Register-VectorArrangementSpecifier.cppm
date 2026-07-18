module;

#include <disconf.hpp>

export module disxx.disasm.operand.Register:VectorArrangementSpecifier;

export import <optional>;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ [[nodiscard]] VectorArrangementSpecifier
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_8B	= 0b000,
			ID_B16	= 0b001,
			ID_4H	= 0b010,
			ID_8H	= 0b011,
			ID_2S	= 0b100,
			ID_4S	= 0b101,
			ID_1D	= 0b110,
			ID_2D	= 0b111,

			ID_B	= 0b1000,
			ID_H	= 0b1001,
			ID_S	= 0b1010,
			ID_D	= 0b1011
		};

	  private:
		std::optional<unsigned short int> m_Lanes{};
		Identifier m_Identifier{};

	  public:
		explicit VectorArrangementSpecifier(void) noexcept;
		explicit VectorArrangementSpecifier(unsigned short int) noexcept;
		explicit VectorArrangementSpecifier(unsigned short int, unsigned short int) noexcept;
	
		VectorArrangementSpecifier(const VectorArrangementSpecifier &) noexcept;
		VectorArrangementSpecifier &operator=(const VectorArrangementSpecifier &) noexcept;

		VectorArrangementSpecifier(VectorArrangementSpecifier &&) noexcept;
		VectorArrangementSpecifier &operator=(VectorArrangementSpecifier &&) noexcept;

		inline std::optional<unsigned short int> GetLanes(void) const noexcept;
		inline Identifier GetIdentifier(void) const noexcept;
	};

	VectorArrangementSpecifier::VectorArrangementSpecifier(void) noexcept
		: m_Lanes{}
		, m_Identifier{}
	{}

	VectorArrangementSpecifier::VectorArrangementSpecifier(unsigned short int bits) noexcept
		: m_Lanes{}
		, m_Identifier{static_cast<Identifier>(bits)}
	{}

	VectorArrangementSpecifier::VectorArrangementSpecifier(unsigned short int bits, unsigned short int lanes) noexcept
		: m_Lanes{lanes}
		, m_Identifier{static_cast<Identifier>(bits | 0b1000)}
	{}

	VectorArrangementSpecifier::VectorArrangementSpecifier(const VectorArrangementSpecifier &other) noexcept
		: m_Lanes{other.m_Lanes}
		, m_Identifier{other.m_Identifier}
	{}

	VectorArrangementSpecifier &VectorArrangementSpecifier::operator=(const VectorArrangementSpecifier &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Lanes = other.m_Lanes;
			this->m_Identifier = other.m_Identifier;
		}

		return *this;
	}

	VectorArrangementSpecifier::VectorArrangementSpecifier(VectorArrangementSpecifier &&other) noexcept
		: m_Lanes{std::move(other.m_Lanes)}
		, m_Identifier{std::move(other.m_Identifier)}
	{}

	VectorArrangementSpecifier &VectorArrangementSpecifier::operator=(VectorArrangementSpecifier &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Lanes = std::move(other.m_Lanes);
			this->m_Identifier = std::move(other.m_Identifier);
		}

		return *this;
	}

	inline std::optional<unsigned short int> VectorArrangementSpecifier::GetLanes(void) const noexcept
	{ return this->m_Lanes; }

	inline VectorArrangementSpecifier::Identifier VectorArrangementSpecifier::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* disxx::disasm::operand */
