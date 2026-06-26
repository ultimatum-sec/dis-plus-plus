module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.PState;

namespace disxx::disasm::operand
{
	PState::PState(void) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{}
	{}

	PState::PState(unsigned short int bits) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{static_cast<Type>(bits)}
	{}

	PState::PState(const PState &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{other.m_PState}
	{}

	PState &PState::operator=(const PState &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_PState = other.m_PState;

		return *this;
	}

	PState::PState(PState &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{std::move(other.m_PState)}
	{}

	PState &PState::operator=(PState &&other) noexcept
	{
		this->m_PState = std::move(other.m_PState);
		return *this;
	}

	std::string PState::GetMnemonic(void) const noexcept(false)
	{
		return s_PStateTable
			.at(this->m_PState);
	}

	std::unique_ptr<AbstractOperand> PState::Clone(void) const noexcept
	{ return std::make_unique<PState>(*this); }

	const std::unordered_map<PState::PState::Type, const char *> PState::PState::s_PStateTable = {
		{Type::TYPE_ZA,			"za"},
		{Type::TYPE_SM,			"sm"},
		{Type::TYPE_UAO,		"uao"},
		{Type::TYPE_PAN,		"pan"},
		{Type::TYPE_SPSEL,		"spsel"},
		{Type::TYPE_SSBS,		"ssbs"},
		{Type::TYPE_DIT,		"dit"},
		{Type::TYPE_TCO,		"tco"},
		{Type::TYPE_DAIFSET,	"daifset"},
		{Type::TYPE_DAIFCLR,	"daifclr"},
		{Type::TYPE_ALLINT,		"allint"},
		{Type::TYPE_PM,			"pm"},
		{Type::TYPE_SVCRSM,		"svcrsm"},
		{Type::TYPE_SVCRZA,		"scvrza"},
		{Type::TYPE_SVCRSMZA,	"svcrsmza"}
	};
} /* operand */
