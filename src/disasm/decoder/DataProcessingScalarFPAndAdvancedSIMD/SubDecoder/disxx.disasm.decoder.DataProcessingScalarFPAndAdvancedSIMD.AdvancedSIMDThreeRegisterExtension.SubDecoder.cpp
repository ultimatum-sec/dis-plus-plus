module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeRegisterExtension.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeRegisterExtension
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
        // +-+-+-+-----+----+-+--+-+------+-+--+--+
        // |0|Q|U|01110|size|0|Rm|1|opcode|1|Rn|Rd|
        // +-+-+-+-----+----+-+--+-+------+-+--+--+

        unsigned short int Q, U, size, Rm, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 11, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000010, InstructionID::INSN_SDOT},
            {0b10000010, InstructionID::INSN_SDOT},
            {0b00001110, InstructionID::INSN_FCVTN},
            {0b10001110, InstructionID::INSN_FCVTN2},
            {0b00001111, InstructionID::INSN_FDOT},
            {0b10001111, InstructionID::INSN_FDOT},
            {0b00011110, InstructionID::INSN_FCVTN},
            {0b10011110, InstructionID::INSN_FCVTN},
            {0b00011111, InstructionID::INSN_FDOT},
            {0b10011111, InstructionID::INSN_FDOT},
            {0b00100011, InstructionID::INSN_USDOT},
            {0b10100011, InstructionID::INSN_USDOT},
            {0b01000000, InstructionID::INSN_SQRDMLAH},
            {0b11000000, InstructionID::INSN_SQRDMLAH},
            {0b01000001, InstructionID::INSN_SQRDMLSH},
            {0b11000001, InstructionID::INSN_SQRDMLSH},
            {0b01000010, InstructionID::INSN_UDOT},
            {0b11000010, InstructionID::INSN_UDOT},
            {0b01001000, InstructionID::INSN_FCMLA},
            {0b11001000, InstructionID::INSN_FCMLA},
            {0b01001100, InstructionID::INSN_FCADD},
            {0b11001100, InstructionID::INSN_FCADD},
            {0b01011111, InstructionID::INSN_BFDOT},
            {0b11011111, InstructionID::INSN_BFDOT},
            {0b01111111, InstructionID::INSN_BFMLALB},
            {0b11111111, InstructionID::INSN_BFMLALT},
            {0b00001000, InstructionID::INSN_FMLALLBB},
            {0b00011000, InstructionID::INSN_FMLALLBT},
            {0b00111111, InstructionID::INSN_FMLALB},
            {0b10001000, InstructionID::INSN_FMLALLTB},
            {0b10011000, InstructionID::INSN_FMLALLTT},
            {0b10100100, InstructionID::INSN_SMMLA},
            {0b10100101, InstructionID::INSN_USMMLA},
            {0b10111111, InstructionID::INSN_FMLALT},
            {0b11001101, InstructionID::INSN_FMMLA},
            {0b11011101, InstructionID::INSN_BFMMLA},
            {0b11100100, InstructionID::INSN_UMMLA},
            {0b11101101, InstructionID::INSN_FMMLA}
        };

        unsigned short int encoding = (Q << 7) | (U << 6) | (size << 4) | opcode;
        if (opcode <= 0b0010)
            encoding &= ~(0b11 << 4);

        if (U == 0b1 && (opcode >> 2) == 0b10)
        {
            encoding &= ~0b11;
            encoding &= ~(0b11 << 4);
        }
        else if (U == 0b1 && (opcode & ~(0b1 << 1)) == 0b1100)
        {
            encoding &= ~(0b1 << 1);
            encoding &= ~(0b11 << 4);
        }

        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
    
        if (opcode == 0b1110)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier(Q == 0b1 ? "16b" : "8b");
            
            const auto Tb{size == 0b00 ? "4s" : (Q == 0b1 ? "8h" : "4h")};
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier(Tb);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier(Tb);

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (U == 0b0 && size <= 0b01 && opcode == 0b1111)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier
            (
                [&size, &Q](void) -> const char *
                {
                    if (size == 0b00)
                        return Q == 0b1 ? "4s" : "2s";
                    return Q == 0b1 ? "8h" : "4h";
                }()
            );

            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier(Q == 0b1 ? "16b" : "8b");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier(Q == 0b1 ? "16b" : "8b");

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if ((opcode >> 2) == 0b01 || opcode == 0b0010 || opcode == 0b0011)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier(Q == 0b1 ? "4s" : "2s");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier(Q == 0b1 ? "16b" : "8b");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier(Q == 0b1 ? "16b" : "8b");
            
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (U == 0b1 && opcode <= 0b0001)
        {
            std::unordered_map<unsigned short int, const char *> specifiersTable = {
                {0b010, "4h"},
                {0b011, "8h"},
                {0b100, "2s"},
                {0b101, "4s"}
            };

            const auto T{specifiersTable.find((size << 1) | Q)};
            if (T == specifiersTable.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            for (auto &pReg : this->m_Operands)
                static_cast<disxx::disasm::operand::Register *>(pReg.get())->SetArrangementSpecifier(T->second);

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (U == 0b1 && (((opcode >> 2) == 0b10) || ((opcode & ~(0b1 << 1)) == 0b1100)))
        {
            std::unordered_map<unsigned short int, const char *> arrangementSpecifierTable = {
                {0b010, "4h"},
                {0b011, "8h"},
                {0b100, "2s"},
                {0b101, "4s"},
                {0b111, "2d"}
            };

            const auto T{arrangementSpecifierTable.find((size << 1) | Q)};
            if (T == arrangementSpecifierTable.end()) [[unlikely]]
                 return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            for (auto &pReg : this->m_Operands)
                static_cast<disxx::disasm::operand::Register *>(pReg.get())->SetArrangementSpecifier(T->second);

            // rot
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>
                (
                    (opcode >> 2) == 0b10
                        ? std::array<unsigned short int, 4>{0, 90, 180, 270}.at(opcode & ~(0b11 << 2))
                        : (bits::extract<unsigned short int, unsigned short int, 1, 1>(opcode) ? 270 : 90)
                )
            );
        }
        else if (U == 0b1 && (size == 0b01 || size == 0b11) && (opcode == 0b1101))
        {
            if (size == 0b01)
            {
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())
                    ->SetArrangementSpecifier(Q == 0b1 ? "4s" : "2s");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())
                    ->SetArrangementSpecifier(Q == 0b1 ? "8h" : "4h");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())
                    ->SetArrangementSpecifier(Q == 0b1 ? "8h" : "4h");

                return std::make_pair(it->second, std::move(this->m_Operands));
            }

            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier("4s");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier("8h");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier("8h");
                
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (opcode == 0b1000)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier("4s");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier("16b");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier("16b");
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (size == 0b11 && opcode == 0b1111)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier("8h");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier("16b");
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier("16b");
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (opcode == 0b1101)
        {
            if (size == 0b00)
            {
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier("8h");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier("16b");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier("16b");
        
                return std::make_pair(it->second, std::move(this->m_Operands));
            }
            else
            {
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier("4s");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier("16b");
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(2).get())->SetArrangementSpecifier("16b");
        
                return std::make_pair(it->second, std::move(this->m_Operands));
            }
        }
        
        return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}}; // Can't define instruction's type
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeRegisterExtension */
