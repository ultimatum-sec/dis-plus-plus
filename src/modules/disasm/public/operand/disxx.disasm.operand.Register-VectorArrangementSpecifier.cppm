module;

#include <disconf.hpp>

export module disxx.disasm.operand.Register:VectorArrangementSpecifier;

export import <optional>;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ [[nodiscard]] VectorArrangementSpecifier
	{
	  public:
		enum class Identifier
		{
			ID_B8, ID_B16,
			ID_H4, ID_H8,
			ID_S2, ID_S4,
			ID_D1, ID_D2,

			ID_B, ID_H,
			ID_S, ID_D
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

	inline std::optional<unsigned short int> VectorArrangementSpecifier::GetLanes(void) const noexcept
	{ return this->m_Lanes; }

	inline VectorArrangementSpecifier::Identifier VectorArrangementSpecifier::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* disxx::disasm::operand */
