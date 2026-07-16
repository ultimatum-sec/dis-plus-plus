module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.Extension;

import disxx.disasm.operand.IOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Extension final : public IOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_UXTB, ID_UXTH,
			ID_UXTW, ID_UXTX,
			ID_SXTB, ID_SXTH,
			ID_SXTW, ID_SXTX
		};

	  private:
		Identifier m_Identifier{};
		unsigned short int m_Value{};

	  public:
		explicit Extension(void) noexcept;
		explicit Extension(unsigned short int, unsigned short int) noexcept;

		explicit Extension(const Extension &) noexcept;
		Extension &operator=(const Extension &) noexcept;

		explicit Extension(Extension &&) noexcept;
		Extension &operator=(Extension &&) noexcept;
 
        virtual std::unique_ptr<IOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
	};

	inline Extension::Identifier Extension::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* disxx::disasm::operand */
