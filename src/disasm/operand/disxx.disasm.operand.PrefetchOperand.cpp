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
		: AbstractOperand{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{}
	{}

	PrefetchOperand::PrefetchOperand(unsigned short int type) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{static_cast<Type>(type)}
	{}

	PrefetchOperand::PrefetchOperand(const PrefetchOperand &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{other.m_Prfop}
	{}

	PrefetchOperand &PrefetchOperand::operator=(const PrefetchOperand &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Prfop = other.m_Prfop;
		return *this;
	}

	PrefetchOperand::PrefetchOperand::PrefetchOperand(PrefetchOperand &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{std::move(other.m_Prfop)}
	{}

	PrefetchOperand &PrefetchOperand::operator=(PrefetchOperand &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Prfop = std::move(other.m_Prfop);
		return *this;
	}

	std::string PrefetchOperand::GetMnemonic(void) const noexcept(false)
	{
		if (s_PrfopTable.contains(this->m_Prfop))
			return s_PrfopTable.at(this->m_Prfop);
		return std::format("#0x{:x}", static_cast<unsigned short int>(this->m_Prfop));
	}

	std::unique_ptr<AbstractOperand> PrefetchOperand::Clone(void) const noexcept
	{ return std::make_unique<PrefetchOperand>(*this); }

	const std::unordered_map<PrefetchOperand::PrefetchOperand::Type, const char *> PrefetchOperand::PrefetchOperand::s_PrfopTable = {
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL1KEEP, "pldl1keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL1STRM, "pldl1strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL2KEEP, "pldl2keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL2STRM, "pldl2strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL3KEEP, "pldl3keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLDL3STRM, "pldl3strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLDSLCKEEP, "pldslckeep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLDSLCSTRM, "pldslcstrm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL1KEEP, "plil1keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL1STRM, "plil1strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL2KEEP, "plil2keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL2STRM, "plil2strm"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL3KEEP, "plil3keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLIL3STRM, "plil3strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PLISLCKEEP, "plislckeep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PLISLCSTRM, "plislcstrm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL1KEEP, "pstl1keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL1STRM, "pstl1strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL2KEEP, "pstl2keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL2STRM, "pstl2strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL3KEEP, "pstl3keep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PSTL3STRM, "pstl3strm"},
        {PrefetchOperand::PrefetchOperand::Type::TYPE_PSTSLCKEEP, "pstslckeep"},
		{PrefetchOperand::PrefetchOperand::Type::TYPE_PSTSLCSTRM, "pstslcstrm"}
	};
} /* disxx::disasm::operand */
