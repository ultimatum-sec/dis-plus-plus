module;

#include <string_view>
#include <stdexcept>
#include <utility>
#include <memory>
#include <string>

module disxx.disasm.operand.AbstractOperand;

namespace disxx::disasm::operand
{
	/* AbstractOperand */

	AbstractOperand::AbstractOperand(Type type) noexcept
		: m_Type{type}
	{}

	AbstractOperand::AbstractOperand(AbstractOperand &&other) noexcept
        : m_Type{std::move(other.m_Type)}
    {}

	AbstractOperand &AbstractOperand::operator=(AbstractOperand &&other) noexcept
    {
        if (this != &other) [[likely]]
            this->m_Type = std::move(other.m_Type);
        return *this;
    }
} /* operand */
