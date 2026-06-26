module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.MemoryBarrier;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ MemoryBarrier final : public AbstractOperand
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_OSHLD	= 0b0001,
			TYPE_OSHST	= 0b0010,
			TYPE_OSH	= 0b0011,
			TYPE_NSHLD	= 0b0101,
			TYPE_NSHST	= 0b0110,
			TYPE_NSH	= 0b0111,
			TYPE_ISHLD	= 0b1001,
			TYPE_ISHST	= 0b1010,
			TYPE_ISH	= 0b1011,
			TYPE_LD		= 0b1101,
			TYPE_ST		= 0b1110,
			TYPE_SY		= 0b1111
		};

	  private:
		static const std::unordered_map<Type, const char *> s_BarrierTable;

	  private:
		Type m_Barrier{};
		bool m_NXS{};

	  public:
		explicit MemoryBarrier(void) noexcept;
		explicit MemoryBarrier(unsigned short int, bool) noexcept;

		explicit MemoryBarrier(const MemoryBarrier &) noexcept;
		MemoryBarrier &operator=(const MemoryBarrier &) noexcept;

		explicit MemoryBarrier(MemoryBarrier &&) noexcept;
		MemoryBarrier &operator=(MemoryBarrier &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
