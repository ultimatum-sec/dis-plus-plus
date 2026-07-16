module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <format>
#include <array>

module disxx.disasm.operand.PrefetchOperand;

namespace disxx::disasm::operand
{
	PrefetchOperand::PrefetchOperand(void) noexcept
		: IOperand{}
		, m_Identifier{}
	{}

	PrefetchOperand::PrefetchOperand(unsigned short int id) noexcept
		: IOperand{}
		, m_Identifier{static_cast<Identifier>(id)}
	{}

	PrefetchOperand::PrefetchOperand(const PrefetchOperand &other) noexcept
		: IOperand{}
		, m_Identifier{other.m_Identifier}
	{}

	PrefetchOperand &PrefetchOperand::operator=(const PrefetchOperand &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = other.m_Identifier;
		return *this;
	}

	PrefetchOperand::PrefetchOperand::PrefetchOperand(PrefetchOperand &&other) noexcept
		: IOperand{}
		, m_Identifier{std::move(other.m_Identifier)}
	{}

	PrefetchOperand &PrefetchOperand::operator=(PrefetchOperand &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = std::move(other.m_Identifier);
		return *this;
	}

	std::unique_ptr<IOperand> PrefetchOperand::Clone(void) const noexcept
	{ return std::make_unique<PrefetchOperand>(*this); }
} /* disxx::disasm::operand */
