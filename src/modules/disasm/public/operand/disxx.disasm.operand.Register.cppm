module;

#include <disconf.hpp>

export module disxx.disasm.operand.Register;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Register final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		enum class Type { TYPE_GPR, TYPE_NEON, TYPE_SYSREG };

	  public:
		explicit Register(void) noexcept;
		explicit Register(Type, unsigned short int, unsigned short int, bool = false) noexcept;
		
		explicit Register(const Register &) noexcept(false);
		Register &operator=(const Register &) noexcept(false);

		explicit Register(Register &&) noexcept;
		Register &operator=(Register &&) noexcept;
		
		virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;  

		void SetArrangementSpecifier(const std::string) noexcept(false);

	  public:
		static std::string_view GetArrangementSpecifier(unsigned short int size, unsigned short int Q);
	};
} /* operand */
