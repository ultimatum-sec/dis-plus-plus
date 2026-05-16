module;

#include <string_view>
#include <stdexcept>
#include <utility>
#include <memory>
#include <string>

module disxx.disasm.operand.AbstractOperand;

namespace disxx::disasm::operand
{
	/* AbstractImpl */

	AbstractOperand::AbstractImpl::AbstractImpl(Type type) noexcept
		: m_Type{type}
	{}

	/*
	AbstractOperand::AbstractImpl::AbstractImpl(const AbstractImpl &other) noexcept
		: m_Type{other.m_Type}
	{}

	AbstractOperand::AbstractImpl &AbstractOperand::AbstractImpl::operator=(const AbstractImpl &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Type = other.m_Type;
		return *this;
	}
	*/

	AbstractOperand::AbstractImpl::AbstractImpl(AbstractImpl &&other) noexcept
        : m_Type{std::move(other.m_Type)}
    {}

	AbstractOperand::AbstractImpl &AbstractOperand::AbstractImpl::operator=(AbstractImpl &&other) noexcept
    {
        if (this != &other) [[likely]]
            this->m_Type = std::move(other.m_Type);
        return *this;
    }

	AbstractOperand::Type AbstractOperand::AbstractImpl::GetType(void) const noexcept
	{ return this->m_Type; }

	/* AbstractOperand */

	AbstractOperand::AbstractOperand(void) noexcept
		: m_pImpl{nullptr}
	{}

	AbstractOperand::AbstractOperand(std::unique_ptr<AbstractImpl> &&pImpl) noexcept
		: m_pImpl{std::move(pImpl)}
	{}

	/*
	AbstractOperand::AbstractOperand(const AbstractOperand &other) noexcept(false)
		: m_pImpl{std::make_unique<AbstractImpl>(*other.m_pImpl)}
	{}

	AbstractOperand &AbstractOperand::operator=(const AbstractOperand &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<AbstractImpl>(*other.m_pImpl);
		}
	
		return *this;
	}
	*/

	AbstractOperand::AbstractOperand(AbstractOperand &&other) noexcept
    	: m_pImpl{std::move(other.m_pImpl.Get())}
	{}

    AbstractOperand &AbstractOperand::operator=(AbstractOperand &&other) noexcept
    {
        if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
            this->m_pImpl.Get() = std::move(other.m_pImpl.Get());
        }

		return *this;
    }

	AbstractOperand::Type AbstractOperand::GetType(void) const noexcept(false)
	{ return this->m_pImpl->GetType(); }

	std::string AbstractOperand::GetMnemonic(void) const noexcept(false)
	{ return this->m_pImpl->GetMnemonic(); }
} /* operand */
