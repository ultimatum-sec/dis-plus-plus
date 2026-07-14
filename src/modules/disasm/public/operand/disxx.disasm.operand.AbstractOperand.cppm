module;

#include <disconf.hpp>

export module disxx.disasm.operand.AbstractOperand;

export import <memory>;
export import <string>;

export import disxx.utility.trait.Cloneable;

export namespace disxx::disasm::operand
{
    class __DISXX_EXPORT__ [[nodiscard]] IOperand : public disxx::utility::trait::Cloneable<AbstractOperand>
    {
      public:
		explicit AbstractOperand(void) noexcept = default;

		AbstractOperand(const AbstractOperand &) noexcept = default;
		AbstractOperand &operator=(const AbstractOperand &) noexcept = default;

		virtual ~AbstractOperand(void) noexcept override = default;
	};
} /* operand */
