module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.LDIAPPSTILP.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::LDIAPPSTILP
{
	SubDecoder::SubDecoder(void) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{}
	{}

	SubDecoder::SubDecoder(std::uint32_t insn, std::uint64_t addr) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{insn, addr}
	{}

	SubDecoder::SubDecoder(const SubDecoder &other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{other}
	{}

	SubDecoder &SubDecoder::operator=(const SubDecoder &other) noexcept
	{
		if (this != &other)
			[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(other)};
		return *this;
	}

	SubDecoder::SubDecoder(SubDecoder &&other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{std::move(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(std::move(other))};
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +----+-------+-+-+---+----+--+--+--+
        // |size|0110010|L|0|Rt2|opc2|10|Rn|Rt|
        // +----+-------+-+-+---+----+--+--+--+

        unsigned short int size, L, Rt2, opc2, Rn, Rt;
        size =  bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        L =  bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rt2 =  bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opc2 =  bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        Rn =  bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt =  bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        if ((size & 0b10) == 0b00) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if ((size & 0b10) == 0b10 && (opc2 & 0b1110) == 0b0010) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if ((size & 0b10) == 0b10 && (opc2 & 0b1100) == 0b0100) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if ((size & 0b10) == 0b10 && (opc2 & 0b1000) == 0b1000) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto insn{L == 0b0 ? InstructionID::INSN_STILP : InstructionID::INSN_LDIAPP};
   
		const disxx::disasm::operand::Register::Type rtype
		{
			size == 0b11
				? disxx::disasm::operand::Register::Type::TYPE_X
				: disxx::disasm::operand::Register::Type::TYPE_W
		};
 
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(rtype, Rt));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(rtype, Rt2));
		this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>
			(
				disxx::disasm::operand::Register
				{
					disxx::disasm::operand::Register::Type::TYPE_X,
					Rn,
					true
				}
			)
		);
        if (opc2 == 0b0000)
        {
            if (L == 0b0)
            {
                static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())->AddImmediatePreIndexedOffset
				(
					disxx::disasm::operand::Immediate<signed short int, 9>{static_cast<signed short int>(size == 0b11 ? -16 : -8)},
					disxx::disasm::operand::LoadsAndStoresAddress::PreIndexedOffsetKind::IDX_REGULAR
				);
                
				return std::make_pair(insn, std::move(this->m_Operands));
            }

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(size == 0b11 ? 8 : 16));
        }

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::LDIAPPSTILP */
