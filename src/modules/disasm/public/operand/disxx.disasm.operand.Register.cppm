module;

#include <disconf.hpp>

#include <optional>

export module disxx.disasm.operand.Register;

import disxx.disasm.operand.AbstractOperand;
export import :VectorArrangementSpecifier;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ [[nodiscard]] Register final : public AbstractOperand
	{
	  public:
		enum class Type { TYPE_GPR, TYPE_NEON, TYPE_SYSREG };

	  private:
		std::optional<VectorArrangementSpecifier> m_VectorArrangementSpecifier{};
		Type m_RegisterType{};
		unsigned short int m_Bits{};
		unsigned short int m_Size{};
		bool m_ExcludeZero{};

	  public:
		explicit Register(void) noexcept;
		explicit Register(Type, unsigned short int, unsigned short int, bool = false) noexcept;
		
		Register(const Register &) noexcept;
		Register &operator=(const Register &) noexcept;

		Register(Register &&) noexcept;
		Register &operator=(Register &&) noexcept;
	
		virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;

		inline void SetVectorArrangementSpecifier(VectorArrangementSpecifier) noexcept;

		inline vRegister
		inline std::optional<VectorArrangementSpecifier> GetVectorArrangementSpecifier(void) const noexcept;
	};

	inline void Register::SetVectorArrangementSpecifier(VectorArrangementSpecifier spec) noexcept
	{ this->m_Specifier = spec; }

	inline vstd::optional<VectorArrangementSpecifier> GetVectorArrangementSpecifier(void) const noexcept
	{ return this->m_VectorArrangementSpecifier; }
} /* operand */
