module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.OneHundredAndTwentyEightBitAtomicMemoryOperations.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::OneHundredAndTwentyEightBitAtomicMemoryOperations
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
        // +-+-+------+-+-+-+---+--+---+--+--+--+
        // |0|S|011001|A|R|1|Rt2|o3|opc|00|Rn|Rt|
        // +-+-+------+-+-+-+---+--+---+--+--+--+

        unsigned short int S, A, R, Rt2, o3, opc, Rn, Rt;
        S = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        A = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        R = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rt2 = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        o3 = bits::extract<unsigned short int, std::uint32_t, 15, 15>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 12, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        /*
        if ((opc & 0b100) == 0b100)
            throw DisassemblyError(this->m_Insn);
        else if (S == 0b0 && o3 == 0b0 && (opc & 0b101) == 0b000)
            throw DisassemblyError(this->m_Insn);
        else if (S == 0b1 && o3 == 0b0 && (opc & 0b100) == 0b000)
            throw DisassemblyError(this->m_Insn);
        else if (S == 0b1 && o3 == 0b1 && opc == 0b000)
            throw DisassemblyError(this->m_Insn);
        */

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000001, InstructionID::INSN_LDCLRP},
            {0b0000011, InstructionID::INSN_LDSETP},
            {0b0001000, InstructionID::INSN_SWPP},
            {0b0001001, InstructionID::INSN_RCWCLRP},
            {0b0001010, InstructionID::INSN_RCWSWPP},
            {0b0001011, InstructionID::INSN_RCWSETP},
            {0b0010001, InstructionID::INSN_LDCLRPL},
            {0b0010011, InstructionID::INSN_LDSETPL},
            {0b0011000, InstructionID::INSN_SWPPL},
            {0b0011001, InstructionID::INSN_RCWCLRPL},
            {0b0011010, InstructionID::INSN_RCWSWPPL},
            {0b0011011, InstructionID::INSN_RCWSETPL},
            {0b0100001, InstructionID::INSN_LDCLRPA},
            {0b0100011, InstructionID::INSN_LDSETPA},
            {0b0101000, InstructionID::INSN_SWPPA},
            {0b0101001, InstructionID::INSN_RCWCLRPA},
            {0b0101010, InstructionID::INSN_RCWSWPPA},
            {0b0101011, InstructionID::INSN_RCWSETPA},
            {0b0110001, InstructionID::INSN_LDCLRPAL},
            {0b0110011, InstructionID::INSN_LDSETPAL},
            {0b0111000, InstructionID::INSN_SWPPAL},
            {0b0111001, InstructionID::INSN_RCWCLRPAL},
            {0b0111010, InstructionID::INSN_RCWSWPPAL},
            {0b0111011, InstructionID::INSN_RCWSETPAL},
            {0b1001001, InstructionID::INSN_RCWSCLRP},
            {0b1001010, InstructionID::INSN_RCWSSWPP},
            {0b1001011, InstructionID::INSN_RCWSSETP},
            {0b1011001, InstructionID::INSN_RCWSCLRPL},
            {0b1011010, InstructionID::INSN_RCWSSWPPL},
            {0b1011011, InstructionID::INSN_RCWSSETPL},
            {0b1101001, InstructionID::INSN_RCWSCLRPA},
            {0b1101010, InstructionID::INSN_RCWSSWPPA},
            {0b1101011, InstructionID::INSN_RCWSSETPA},
            {0b1111001, InstructionID::INSN_RCWSCLRPAL},
            {0b1111010, InstructionID::INSN_RCWSSWPPAL},
            {0b1111011, InstructionID::INSN_RCWSSETPAL}
        };

        const unsigned short int encoding = (S << 6) | (A << 5) | (R << 4) | (o3 << 3) | opc;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt2, 64));
        disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true};
		this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::128BitAtomicMemoryOperations */
