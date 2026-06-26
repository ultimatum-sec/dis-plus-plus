module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.MemoryCopyAndMemorySet.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::MemoryCopyAndMemorySet
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +----+---+--+--+---+-+--+---+--+--+--+
        // |size|011|o0|01|op1|0|Rs|op2|01|Rn|Rd|
        // +----+---+--+--+---+-+--+---+--+--+--+

        unsigned short int o0, op1, Rs, op2, Rn, Rd;
        //size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        o0 = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        op1 = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rs = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000000, InstructionID::INSN_CPYFP},
            {0b0010000, InstructionID::INSN_CPYFM},
            {0b0100000, InstructionID::INSN_CPYFE},
            {0b0000001, InstructionID::INSN_CPYFPWT},
            {0b0010001, InstructionID::INSN_CPYFMWT},
            {0b0100001, InstructionID::INSN_CPYFEWT},
            {0b0000010, InstructionID::INSN_CPYFPRT},
            {0b0010010, InstructionID::INSN_CPYFMRT},
            {0b0100010, InstructionID::INSN_CPYFERT},
            {0b0000011, InstructionID::INSN_CPYFPT},
            {0b0010011, InstructionID::INSN_CPYFMT},
            {0b0100011, InstructionID::INSN_CPYFET},
            {0b0000100, InstructionID::INSN_CPYFPWN},
            {0b0010100, InstructionID::INSN_CPYFMWN},
            {0b0100100, InstructionID::INSN_CPYFEWN},
            {0b0000101, InstructionID::INSN_CPYFPWTWN},
            {0b0010101, InstructionID::INSN_CPYFMWTWN},
            {0b0100101, InstructionID::INSN_CPYFEWTWN},
            {0b0000110, InstructionID::INSN_CPYFPRTWN},
            {0b0010110, InstructionID::INSN_CPYFMRTWN},
            {0b0100110, InstructionID::INSN_CPYFERTWN},
            {0b0000111, InstructionID::INSN_CPYFPTWN},
            {0b0010111, InstructionID::INSN_CPYFMTWN},
            {0b0100111, InstructionID::INSN_CPYFETWN},
            {0b0001000, InstructionID::INSN_CPYFPRN},
            {0b0011000, InstructionID::INSN_CPYFMRN},
            {0b0101000, InstructionID::INSN_CPYFERN},
            {0b0001001, InstructionID::INSN_CPYFPWTRN},
            {0b0011001, InstructionID::INSN_CPYFMWTRN},
            {0b0101001, InstructionID::INSN_CPYFEWTRN},
            {0b0001010, InstructionID::INSN_CPYFPRTRN},
            {0b0011010, InstructionID::INSN_CPYFMRTRN},
            {0b0101010, InstructionID::INSN_CPYFERTRN},
            {0b0001011, InstructionID::INSN_CPYFPTRN},
            {0b0011011, InstructionID::INSN_CPYFMTRN},
            {0b0101011, InstructionID::INSN_CPYFETRN},
            {0b0001100, InstructionID::INSN_CPYFPN},
            {0b0011100, InstructionID::INSN_CPYFMN},
            {0b0101100, InstructionID::INSN_CPYFEN},
            {0b0001101, InstructionID::INSN_CPYFPWTN},
            {0b0011101, InstructionID::INSN_CPYFMWTN},
            {0b0101101, InstructionID::INSN_CPYFEWTN},
            {0b0001110, InstructionID::INSN_CPYFPRTN},
            {0b0011110, InstructionID::INSN_CPYFMRTN},
            {0b0101110, InstructionID::INSN_CPYFERTN},
            {0b0001111, InstructionID::INSN_CPYFPTN},
            {0b0011111, InstructionID::INSN_CPYFMTN},
            {0b0101111, InstructionID::INSN_CPYFETN},
            {0b0110000, InstructionID::INSN_SETP},
            {0b0110100, InstructionID::INSN_SETM},
            {0b0111000, InstructionID::INSN_SETE},
            {0b0110001, InstructionID::INSN_SETPT},
            {0b0110101, InstructionID::INSN_SETMT},
            {0b0111001, InstructionID::INSN_SETET},
            {0b0110010, InstructionID::INSN_SETPN},
            {0b0110110, InstructionID::INSN_SETMN},
            {0b0111010, InstructionID::INSN_SETEN},
            {0b0110011, InstructionID::INSN_SETPTN},
            {0b0110111, InstructionID::INSN_SETMTN},
            {0b0111011, InstructionID::INSN_SETETN},
            {0b1000000, InstructionID::INSN_CPYP},
            {0b1010000, InstructionID::INSN_CPYM},
            {0b1100000, InstructionID::INSN_CPYE},
            {0b1000001, InstructionID::INSN_CPYPWT},
            {0b1010001, InstructionID::INSN_CPYMWT},
            {0b1100001, InstructionID::INSN_CPYEWT},
            {0b1000010, InstructionID::INSN_CPYPRT},
            {0b1010010, InstructionID::INSN_CPYMRT},
            {0b1100010, InstructionID::INSN_CPYERT},
            {0b1000011, InstructionID::INSN_CPYPT},
            {0b1010011, InstructionID::INSN_CPYMT},
            {0b1100011, InstructionID::INSN_CPYET},
            {0b1000100, InstructionID::INSN_CPYPWN},
            {0b1010100, InstructionID::INSN_CPYMWN},
            {0b1100100, InstructionID::INSN_CPYEWN},
            {0b1000101, InstructionID::INSN_CPYPWTWN},
            {0b1010101, InstructionID::INSN_CPYMWTWN},
            {0b1100101, InstructionID::INSN_CPYEWTWN},
            {0b1000110, InstructionID::INSN_CPYPRTWN},
            {0b1010110, InstructionID::INSN_CPYMRTWN},
            {0b1100110, InstructionID::INSN_CPYERTWN},
            {0b1000111, InstructionID::INSN_CPYPTWN},
            {0b1010111, InstructionID::INSN_CPYMTWN},
            {0b1100111, InstructionID::INSN_CPYETWN},
            {0b1001000, InstructionID::INSN_CPYPRN},
            {0b1011000, InstructionID::INSN_CPYMRN},
            {0b1101000, InstructionID::INSN_CPYERN},
            {0b1001001, InstructionID::INSN_CPYPWTRN},
            {0b1011001, InstructionID::INSN_CPYMWTRN},
            {0b1101001, InstructionID::INSN_CPYEWTRN},
            {0b1001010, InstructionID::INSN_CPYPRTRN},
            {0b1011010, InstructionID::INSN_CPYMRTRN},
            {0b1001011, InstructionID::INSN_CPYPTRN},
            {0b1011011, InstructionID::INSN_CPYMTRN},
            {0b1101011, InstructionID::INSN_CPYETRN},
            {0b1001100, InstructionID::INSN_CPYPN},
            {0b1011100, InstructionID::INSN_CPYMN},
            {0b1101100, InstructionID::INSN_CPYEN},
            {0b1001101, InstructionID::INSN_CPYPWTN},
            {0b1011101, InstructionID::INSN_CPYMWTN},
            {0b1101101, InstructionID::INSN_CPYEWTN},
            {0b1001110, InstructionID::INSN_CPYPRTN},
            {0b1011110, InstructionID::INSN_CPYMRTN},
            {0b1101110, InstructionID::INSN_CPYERTN},
            {0b1001111, InstructionID::INSN_CPYPTN},
            {0b1011111, InstructionID::INSN_CPYMTN},
            {0b1101111, InstructionID::INSN_CPYETN},
            {0b1110000, InstructionID::INSN_SETGP},
            {0b1110100, InstructionID::INSN_SETGM},
            {0b1111000, InstructionID::INSN_SETGE},
            {0b1110001, InstructionID::INSN_SETGPT},
            {0b1110101, InstructionID::INSN_SETGMT},
            {0b1111001, InstructionID::INSN_SETGET},
            {0b1110010, InstructionID::INSN_SETGPN},
            {0b1110110, InstructionID::INSN_SETGMN},
            {0b1111010, InstructionID::INSN_SETGEN},
            {0b1110011, InstructionID::INSN_SETGPTN},
            {0b1110111, InstructionID::INSN_SETGMTN},
            {0b1111011, InstructionID::INSN_SETGETN}
        };

        const unsigned short int encoding = (o0 << 6) | (op1 << 4) | op2;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));
        if (op1 == 0b11)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rs, 64));
        else
		{
			disxx::disasm::operand::Register other{disxx::disasm::operand::Register::Type::TYPE_GPR, Rs, 64};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(other)));
        }
		this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::MemoryCopyAndMemorySet */
