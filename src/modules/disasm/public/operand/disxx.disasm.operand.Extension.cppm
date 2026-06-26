module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.Extension;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Extension final : public AbstractOperand
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_UXTB,
			TYPE_UXTH,
			TYPE_UXTW,
			TYPE_UXTX,
			TYPE_SXTB,
			TYPE_SXTH,
			TYPE_SXTW,
			TYPE_SXTX
		};

	  private:
		static const std::unordered_map<Type, const char *> s_ExtensionTable;

	  private:
		Type m_ExtensionType{};
		unsigned short int m_Val{};

	  public:
		explicit Extension(void) noexcept;
		explicit Extension(unsigned short int, unsigned short int) noexcept;

		explicit Extension(const Extension &) noexcept;
		Extension &operator=(const Extension &) noexcept;

		explicit Extension(Extension &&) noexcept;
		Extension &operator=(Extension &&) noexcept;
 
		virtual std::string GetMnemonic(void) const noexcept(false)  override;
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
