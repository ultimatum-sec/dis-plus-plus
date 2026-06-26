module;

#include <disconf.hpp>

#include <optional>

export module disxx.disasm.operand.Register;

import disxx.disasm.operand.AbstractOperand;

export import <string_view>;
export import <string>;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Register final : public AbstractOperand
	{
	  public:
		enum class Type { TYPE_GPR, TYPE_NEON, TYPE_SYSREG };

	  private:
		std::optional<std::string> m_VectorArrangementSpecifier{};
		Type m_RegType{};
		unsigned short int m_Bits{};
		unsigned short int m_Size{};
		bool m_ExcludeZero{};

	  public:
		explicit Register(void) noexcept;
		explicit Register(Type, unsigned short int, unsigned short int, bool = false) noexcept;
		
		explicit Register(const Register &) noexcept;
		Register &operator=(const Register &) noexcept;

		explicit Register(Register &&) noexcept;
		Register &operator=(Register &&) noexcept;
	
		virtual std::string GetMnemonic(void) const noexcept(false) override;
		virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;

		void SetArrangementSpecifier(const std::string) noexcept;

	  public:
		static inline std::string_view GetArrangementSpecifier(unsigned short int, unsigned short int) noexcept(false);
	};

	inline std::string_view Register::GetArrangementSpecifier(unsigned short size, unsigned short Q) noexcept(false)
    {
        if (size == 0b00 && Q == 0b0)
            return "8b";
        else if (size == 0b00 && Q == 0b1)
            return "16b";
        else if (size == 0b01 && Q == 0b0)
            return "4h";
        else if (size == 0b01 && Q == 0b1)
            return "8h";
        else if (size == 0b10 && Q == 0b0)
            return "2s";
        else if (size == 0b10 && Q == 0b1)
            return "4s";
        else if (size == 0b11 && Q == 0b0)
            throw std::invalid_argument{"ReservedArrangementSpecifierError"};
        return "2d";
	}
} /* operand */
