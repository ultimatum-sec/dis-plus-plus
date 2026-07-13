module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <tuple>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTwoRegisterMiscellaneous
{
	using InsnTable = std::unordered_map
    <
        unsigned short int,
        std::tuple
        <
            InstructionID,
            std::function<std::string_view(void)>,
            std::function<std::string_view(void)>
        >
    >;

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
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(other);
		return *this;
	}

	SubDecoder::SubDecoder(SubDecoder &&other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{std::forward<SubDecoder &&>(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +-+-+-+-----+----+-----+------+--+--+--+
        // |0|Q|U|01110|size|10000|opcode|10|Rn|Rd|
        // +-+-+-+-----+----+-----+------+--+--+--+

        unsigned short int Q, U, size, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        // Arrangement specifiers
        static const std::unordered_map<unsigned short int, std::string_view> fullArrangementSpecifiersTable = {
            {0b000, "8b"}, {0b001, "16b"},
            {0b010, "4h"}, {0b011, "8h"},
            {0b100, "2s"}, {0b101, "4s"},
            {0b110, "1d"}, {0b111, "2d"}
        };

        const auto getAllArrangementSpecifiers
        {
            [=](unsigned short int shift = 0, unsigned short int border = 0b11) -> std::string_view
            {
                if (size >= border) [[unlikely]]
                    return "";
                
                const auto it{fullArrangementSpecifiersTable.find(((size + shift) << 1) | Q)};
                if (it == fullArrangementSpecifiersTable.end()) [[unlikely]]
                    return "";
                return it->second;
            }
        };

        const auto getSizeBasedArrangementSpecifier
        {
            [=](unsigned short int shift = 0, unsigned short int border = 0b11) -> std::string_view
            {
                if (size >= border) [[unlikely]]
                    return "";
                
                const auto it{fullArrangementSpecifiersTable.find(((size + shift) << 1) | 0b1)};
                if (it == fullArrangementSpecifiersTable.end()) [[unlikely]]
                    return "";
                return it->second;
            }
        };

        const auto getSzQBasedArrangementSpecifier
        {
            [=](unsigned short int shift = 0, short int border = -1) -> std::string_view
            {
                // Specifier between 4s and 2d is reserved!
                if (shift >= 2 && Q == 0b0) [[unlikely]]
                    return "";
    
                if (const auto sz{size & 0b01}; sz >= border && border != -1) [[unlikely]]
                    return "";
    
                // NOTE: To get arrangement specifier correctly, shift must be multiplied by two
                auto it{fullArrangementSpecifiersTable.find(((size << 1) | Q) + shift * 2)};
                if (it == fullArrangementSpecifiersTable.end()) [[unlikely]]
                    return "";
                return it->second;
            }
        };

        // FIXME: Check all the insns without second specifier
        InsnTable insnTable = {
            // |encoding|insn|functions for arrangement specifiers|
            {
				0b000000,
				{
					InstructionID::INSN_REV64,
					getAllArrangementSpecifiers,
					getAllArrangementSpecifiers
				}
			},
            {
				0b000001,
				{
					InstructionID::INSN_REV16,
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b01); },
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b01); }
				}
			},
            {
				0b000010,
				{
					InstructionID::INSN_SADDLP,
					[&] -> std::string_view { return getAllArrangementSpecifiers(1); },
					getAllArrangementSpecifiers
				}
			},
            {
				0b000011,
				{
					InstructionID::INSN_SUQADD,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b000100,
				{
					InstructionID::INSN_CLS,
					getAllArrangementSpecifiers,
					getAllArrangementSpecifiers
				}
			},
            {
				0b000101,
				{
					InstructionID::INSN_CNT,
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b01); },
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b01); }
				}
			},
            {
				0b000110,
				{
					InstructionID::INSN_SADALP,
					[&] -> std::string_view { return getAllArrangementSpecifiers(1); },
					getAllArrangementSpecifiers
				}
			},
            {
				0b000111,
				{
					InstructionID::INSN_SQABS,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b001000,
				{
					InstructionID::INSN_CMGT,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b001001,
				{
					InstructionID::INSN_CMEQ,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b001010,
				{
					InstructionID::INSN_CMLT,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b001011,
				{
					InstructionID::INSN_ABS,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b010010,
				{
					Q == 0b1 ? InstructionID::INSN_XTN2 : InstructionID::INSN_XTN,
					getAllArrangementSpecifiers,
					[&] -> std::string_view { return getSizeBasedArrangementSpecifier(1); }
				}
			},
            {
				0b010100,
				{
					Q == 0b1 ? InstructionID::INSN_SQXTN2 : InstructionID::INSN_SQXTN,
					getAllArrangementSpecifiers,
					[&] -> std::string_view { return getSizeBasedArrangementSpecifier(1); }
				}
			},
            {
				0b100000,
				{
					InstructionID::INSN_REV32,
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b10); },
					[&] -> std::string_view { return getAllArrangementSpecifiers(0, 0b10); }
				}
			},
            {
				0b100010,
				{
					InstructionID::INSN_UADDLP,
					[&] -> std::string_view { return getAllArrangementSpecifiers(1); },
					getAllArrangementSpecifiers
				}
			},
            {
				0b100011,
				{
					InstructionID::INSN_USQADD,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b100100,
				{
					InstructionID::INSN_CLZ,
					getAllArrangementSpecifiers,
					getAllArrangementSpecifiers
				}
			},
            {
				0b100110,
				{
					InstructionID::INSN_UADALP,
					[&] -> std::string_view { return getAllArrangementSpecifiers(1); },
					getAllArrangementSpecifiers
				}
			},
            {
				0b100111,
				{
					InstructionID::INSN_SQNEG,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b101000,
				{
					InstructionID::INSN_CMGE,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b101001,
				{
					InstructionID::INSN_CMLE,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b101011,
				{
					InstructionID::INSN_NEG,
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); },
					[&] -> std::string_view { return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q); }
				}
			},
            {
				0b110010,
				{
					Q == 0b1 ? InstructionID::INSN_SQXTUN2 : InstructionID::INSN_SQXTUN,
					getAllArrangementSpecifiers,
					[&] -> std::string_view { return getSizeBasedArrangementSpecifier(1); }
				}
			},
            // Shit! Oops, I meant shift! I have to process the shift for this insn
            {
				0b110011,
				{
					Q == 0b1 ? InstructionID::INSN_SHLL2 : InstructionID::INSN_SHLL,
					[&] -> std::string_view { return getSizeBasedArrangementSpecifier(1); },
					getAllArrangementSpecifiers
				}
			},
            {
				0b110100,
				{
					Q == 0b1 ? InstructionID::INSN_UQXTN2 : InstructionID::INSN_UQXTN,
					getAllArrangementSpecifiers,
					[&] -> std::string_view { return getSizeBasedArrangementSpecifier(1); }
				}
			}
        };

        InsnTable insnTableWithSize = {
            // |encoding|insn|functions for arrangement specifiers|
            {
				0b00010110,
				{
					Q == 0b1 ? InstructionID::INSN_FCVTN2 : InstructionID::INSN_FCVTN,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(1); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "2d" : "4s"; }
				}
			},
            {
				0b00010111,
				{
					Q == 0b1 ? InstructionID::INSN_FCVTL2 : InstructionID::INSN_FCVTL,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(1); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "2d" : "4s"; }
				}
			},
            {
				0b00011000,
				{
					InstructionID::INSN_FRINTN,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011001,
				{
					InstructionID::INSN_FRINTM,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011010,
				{
					InstructionID::INSN_FCVTNS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011011,
				{
					InstructionID::INSN_FCVTMS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011100,
				{
					InstructionID::INSN_FCVTAS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011101,
				{
					InstructionID::INSN_SCVTF,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b00011110,
				{
					InstructionID::INSN_FRINT32Z,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b00011111,
				{
					InstructionID::INSN_FRINT64Z,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b01001100,
				{
					InstructionID::INSN_FCMGT,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01001101,
				{
					InstructionID::INSN_FCMEQ,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01001110,
				{
					InstructionID::INSN_FCMLT,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01001111,
				{
					InstructionID::INSN_FABS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01011000,
				{
					InstructionID::INSN_FRINTP,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01011001,
				{
					InstructionID::INSN_FRINTZ,	
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01011010,
				{
					InstructionID::INSN_FCVTPS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01011011,
				{
					InstructionID::INSN_FCVTZS,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01011100,
				{
					InstructionID::INSN_URECPE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2, 1); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2, 1); }
				}
			},
            {
				0b01011101,
				{
					InstructionID::INSN_FRECPE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b01010110,
				{
					Q == 0b1 ? InstructionID::INSN_BFCVTN2 : InstructionID::INSN_BFCVTN,
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; },
					[&] -> std::string_view { return "4s"; }
				}
			},
            {
				0b10011000,
				{
					InstructionID::INSN_FRINTA,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011001,
				{
					InstructionID::INSN_FRINTX,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011010,
				{
					InstructionID::INSN_FCVTNU,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011011,
				{
					InstructionID::INSN_FCVTMU,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011100,
				{
					InstructionID::INSN_FCVTAU,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011101,
				{
					InstructionID::INSN_UCVTF,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[size] -> std::string_view { return (size & 0b01) == 0b1 ? "8h" : "4h"; }
				}
			},
            {
				0b10011110,
				{
					InstructionID::INSN_FRINT32X,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b10011111,
				{
					InstructionID::INSN_FRINT64X,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b10000101,
				{
					InstructionID::INSN_NOT,
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			},
            {
				0b10010111,
				{
					Q == 0b1 ? InstructionID::INSN_F1CVTL2 : InstructionID::INSN_F1CVTL,
					[&] -> std::string_view { return "8h"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			},
            {
				0b10100101,
				{
					InstructionID::INSN_RBIT,
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			},
            {
				0b10110110,
				{
					Q == 0b1 ? InstructionID::INSN_FCVTXN2 : InstructionID::INSN_FCVTXN,
					[Q] -> std::string_view { return Q == 0b1 ? "4s" : "2s"; },
					[&] -> std::string_view { return "2d"; }
				}
			},
			{
				0b10110111,
				{
					Q == 0b1 ? InstructionID::INSN_F2CVTL2 : InstructionID::INSN_F2CVTL,
					[&] -> std::string_view { return "8h"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			},
            {
				0b11001100,
				{
					InstructionID::INSN_FCMGE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11001101,
				{
					InstructionID::INSN_FCMLE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11001111,
				{
					InstructionID::INSN_FNEG,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11011001,
				{
					InstructionID::INSN_FRINTI,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11011010,
				{
					InstructionID::INSN_FCVTPU,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11011011,
				{
					InstructionID::INSN_FCVTZU,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11011100,
				{
					InstructionID::INSN_URSQRTE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2, 1); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2, 1); }
				}
			},
            {
				0b11011101,
				{
					InstructionID::INSN_FRSQRTE,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11011111,
				{
					InstructionID::INSN_FSQRT,
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); },
					[&] -> std::string_view { return getSzQBasedArrangementSpecifier(2); }
				}
			},
            {
				0b11010111,
				{
					Q == 0b1 ? InstructionID::INSN_BF1CVTL2 : InstructionID::INSN_BF1CVTL,
					[&] -> std::string_view { return "8h"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			},
            {
				0b11110111,
				{
					Q == 0b1 ? InstructionID::INSN_BF2CVTL2 : InstructionID::INSN_BF2CVTL,
					[&] -> std::string_view { return "8h"; },
					[Q] -> std::string_view { return Q == 0b1 ? "16b" : "8b"; }
				}
			}
        };

		unsigned short int encoding = (U << 5) | opcode;
		auto it{insnTable.find(encoding)};
		if (it == insnTable.end())
		{
            encoding = (U << 7) | (size << 5) | opcode;
            if (!((U == 0b1 && (opcode == 0b10111 || opcode == 0b00101 || opcode == 0b10110)) || encoding == 0b01010110))
                // Turn off the first bit in the size field
                encoding &= ~(0b1 << 5);
			it = insnTableWithSize.find(encoding); 
			
			if (it == insnTableWithSize.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
		}

		const auto &[insn, func1, func2]{it->second};
        const auto &[spec1, spec2]{std::make_tuple(func1(), func2())};
        if (spec1 == "" || spec2 == "") [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier(spec1.data());
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier(spec2.data());
        
        if ((opcode >= 0b01100 && opcode <= 0b01110) || (opcode >= 0b01000 && opcode <= 0b01010) || opcode == 0b01100 || opcode == 0b01101)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<float, 1>>(0.f));
        else if (encoding == 0b110011)
        {
            if (size >= 0b11) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 1>>(8 << size));
        }

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTwoRegisterMiscellaneous */
