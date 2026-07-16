module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.MemoryBarrier;

import disxx.disasm.operand.IOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ MemoryBarrier final : public IOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_OSHLD	= 0b0001,
			ID_OSHST	= 0b0010,
			ID_OSH		= 0b0011,
			ID_NSHLD	= 0b0101,
			ID_NSHST	= 0b0110,
			ID_NSH		= 0b0111,
			ID_ISHLD	= 0b1001,
			ID_ISHST	= 0b1010,
			ID_ISH		= 0b1011,
			ID_LD		= 0b1101,
			ID_ST		= 0b1110,
			ID_SY		= 0b1111
		};

	  private:
		Identifier m_Identifier{};
		bool m_Nxs{};

	  public:
		explicit MemoryBarrier(void) noexcept;
		explicit MemoryBarrier(unsigned short int, bool) noexcept;

		MemoryBarrier(const MemoryBarrier &) noexcept;
		MemoryBarrier &operator=(const MemoryBarrier &) noexcept;

		MemoryBarrier(MemoryBarrier &&) noexcept;
		MemoryBarrier &operator=(MemoryBarrier &&) noexcept;

        virtual std::unique_ptr<IOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
		inline bool Nxs(void) const noexcept;
	};

	inline MemoryBarrier::Identifier MemoryBarrier::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }

	inline bool MemoryBarrier::Nxs(void) const noexcept
	{ return this->m_Nxs; }
} /* disxx::disasm::operand */
