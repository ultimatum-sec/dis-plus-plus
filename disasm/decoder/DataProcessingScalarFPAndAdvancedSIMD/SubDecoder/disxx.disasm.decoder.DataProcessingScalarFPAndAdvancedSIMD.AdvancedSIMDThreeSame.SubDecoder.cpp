module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSame.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeSame
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
        // +-+-+-+-----+----+-+--+------+-+--+--+
        // |0|Q|U|01110|size|1|Rm|opcode|1|Rn|Rd|
        // +-+-+-+-----+----+-+--+------+-+--+--+

		// Unable to compile this code due to libstdc++/115939 STL bug
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		/*
        unsigned short int Q, U, size, Rm, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 11, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const auto getArrangementSpecifierBorder
        {
            [=, this](std::initializer_list<unsigned short int> borders = {}) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError>
            {
                for (const auto &border : borders)
                    if (size == border) [[unlikely]]
                        return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                return disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q);
            }
        };

        const auto getSzBasedArrangementSpecifier
        {
            [=, this](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError>
            {
                const auto sz{size & 0b01};
                if (sz == 0b1 && Q == 0b0) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
                return disxx::disasm::operand::Register::GetArrangementSpecifier(sz | 0b10, Q);
            }
        };
        
        std::unordered_map<unsigned short int, std::pair<InstructionID, std::function<std::expected<std::string_view, disxx::utility::error::DisassemblyError>(void)>>> insnTable = {
            {
				0b000000,
				{
					InstructionID::INSN_SHADD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b000001,
				{
					InstructionID::INSN_SQADD,
					getArrangementSpecifierBorder
				}
			},
            {
				0b000010,
				{
					InstructionID::INSN_SRHADD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b000100,
				{
					InstructionID::INSN_SHSUB, [&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11});  }
				}
			},
            {
				0b000101,
				{
					InstructionID::INSN_SQSUB,
					getArrangementSpecifierBorder
				}
			},
            {
				0b000110,
				{
					InstructionID::INSN_CMGT,
					getArrangementSpecifierBorder
				}
			},
            {
				0b000111,
				{
					InstructionID::INSN_CMGE,
					getArrangementSpecifierBorder
				}
			},
            {
				0b001000,
				{
					InstructionID::INSN_SSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b001001,
				{
					InstructionID::INSN_SQSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b001010,
				{
					InstructionID::INSN_SRSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b001011,
				{
					InstructionID::INSN_SQRSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b001100,
				{
					InstructionID::INSN_SMAX,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b001101,
				{
					InstructionID::INSN_SMIN,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b001110,
				{
					InstructionID::INSN_SABD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b001111,
				{
					InstructionID::INSN_SABA,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b010000,
				{
					InstructionID::INSN_ADD,
					getArrangementSpecifierBorder
				}
			},
            {
				0b010001,
				{
					InstructionID::INSN_CMTST,
					getArrangementSpecifierBorder
				}
			},
            {
				0b010010,
				{
					InstructionID::INSN_MLA,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b010011,
				{
					InstructionID::INSN_MUL,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b010100,
				{
					InstructionID::INSN_SMAXP,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b010101,
				{
					InstructionID::INSN_SMINP,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b010110,
				{
					InstructionID::INSN_SQDMULH,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b00, 0b11}); }
				}
			},
            {
				0b010111,
				{
					InstructionID::INSN_ADDP,
					getArrangementSpecifierBorder
				}
			},
            {
				0b100000,
				{
					InstructionID::INSN_UHADD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b100001,
				{
					InstructionID::INSN_UQADD,
					getArrangementSpecifierBorder
				}
			},
            {
				0b100010,
				{
					InstructionID::INSN_URHADD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b100100,
				{
					InstructionID::INSN_UHSUB,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b100101,
				{
					InstructionID::INSN_UQSUB,
					getArrangementSpecifierBorder
				}
			},
            {
				0b100110,
				{
					InstructionID::INSN_CMHI,
					getArrangementSpecifierBorder
				}
			},
            {
				0b100111,
				{
					InstructionID::INSN_CMHS,
					getArrangementSpecifierBorder
				}
			},
            {
				0b101000,
				{
					InstructionID::INSN_USHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b101001,
				{
					InstructionID::INSN_UQSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b101010,
				{
					InstructionID::INSN_URSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b101011,
				{
					InstructionID::INSN_UQRSHL,
					getArrangementSpecifierBorder
				}
			},
            {
				0b101100,
				{
					InstructionID::INSN_UMAX,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b101101,
				{
					InstructionID::INSN_UMIN,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b101110,
				{
					InstructionID::INSN_UABD,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b101111,
				{
					InstructionID::INSN_UABA,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b110000,
				{
					InstructionID::INSN_SUB,
					getArrangementSpecifierBorder
				}
			},
            {
				0b110001,
				{
					InstructionID::INSN_CMEQ,
					getArrangementSpecifierBorder
				}
			},
            {
				0b110010,
				{
					InstructionID::INSN_MLS,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b110011,
				{
					InstructionID::INSN_PMUL,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b01, 0b10, 0b11}); }
				}
			},
            {
				0b110100,
				{
					InstructionID::INSN_UMAXP,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b110101,
				{
					InstructionID::INSN_UMINP,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b11}); }
				}
			},
            {
				0b110110,
				{
					InstructionID::INSN_SQRDMULH,
					[&getArrangementSpecifierBorder](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return getArrangementSpecifierBorder({0b00, 0b11}); }
				}
			}
        };

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::function<std::expected<std::string_view, disxx::utility::error::DisassemblyError>(void)>>> insnTableWithSize = {
            {0b00011000, {InstructionID::INSN_FMAXNM, getSzBasedArrangementSpecifier}},
            {0b00011001, {InstructionID::INSN_FMLA, getSzBasedArrangementSpecifier}},
            {0b00011010, {InstructionID::INSN_FADD, getSzBasedArrangementSpecifier}},
            {0b00011011, {InstructionID::INSN_FMULX, getSzBasedArrangementSpecifier}},
            {0b00011100, {InstructionID::INSN_FCMEQ, getSzBasedArrangementSpecifier}},
            {0b00011110, {InstructionID::INSN_FMAX, getSzBasedArrangementSpecifier}},
            {0b00011111, {InstructionID::INSN_FRECPS, getSzBasedArrangementSpecifier}},
            {0b00000011, {InstructionID::INSN_AND, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            // Handles in other way - it has different arrangement specifiers
            {0b00011101, {InstructionID::INSN_FMLAL, [](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return ""; }}},
            {0b00100011, {InstructionID::INSN_BIC, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            {0b01011000, {InstructionID::INSN_FMINNM, getSzBasedArrangementSpecifier}},
            {0b01011001, {InstructionID::INSN_FMLS, getSzBasedArrangementSpecifier}},
            {0b01011010, {InstructionID::INSN_FSUB, getSzBasedArrangementSpecifier}},
            {0b01011011, {InstructionID::INSN_FAMAX, getSzBasedArrangementSpecifier}},
            {0b01011110, {InstructionID::INSN_FMIN, getSzBasedArrangementSpecifier}},
            {0b01011111, {InstructionID::INSN_FRSQRTS, getSzBasedArrangementSpecifier}},
            {0b01000011, {InstructionID::INSN_ORR, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}}, // Alias when Rm == Rn
            // The same thing again... handles in other way
            {0b01011101, {InstructionID::INSN_FMLSL, [](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return ""; }}},
            {0b01100011, {InstructionID::INSN_ORN, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            {0b10011000, {InstructionID::INSN_FMAXNMP, getSzBasedArrangementSpecifier}},
            {0b10011010, {InstructionID::INSN_FADDP, getSzBasedArrangementSpecifier}},
            {0b10011011, {InstructionID::INSN_FMUL, getSzBasedArrangementSpecifier}},
            {0b10011100, {InstructionID::INSN_FCMGE, getSzBasedArrangementSpecifier}},
            {0b10011101, {InstructionID::INSN_FACGE, getSzBasedArrangementSpecifier}},
            {0b10011110, {InstructionID::INSN_FMAXP, getSzBasedArrangementSpecifier}},
            {0b10011111, {InstructionID::INSN_FDIV, getSzBasedArrangementSpecifier}},
            {0b10000011, {InstructionID::INSN_EOR, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            // Still handles in othe way
            {0b10011001, {InstructionID::INSN_FMLAL2, [](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return ""; }}},
            {0b10100011, {InstructionID::INSN_BSL, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            {0b11011000, {InstructionID::INSN_FMINNMP, getSzBasedArrangementSpecifier}},
            {0b11011010, {InstructionID::INSN_FABD, getSzBasedArrangementSpecifier}},
            {0b11011011, {InstructionID::INSN_FAMIN, getSzBasedArrangementSpecifier}},
            {0b11011100, {InstructionID::INSN_FCMGT, getSzBasedArrangementSpecifier}},
            {0b11011101, {InstructionID::INSN_FACGT, getSzBasedArrangementSpecifier}},
            {0b11011110, {InstructionID::INSN_FMINP, getSzBasedArrangementSpecifier}},
            {0b11011111, {InstructionID::INSN_FSCALE, getSzBasedArrangementSpecifier}},
            {0b11000011, {InstructionID::INSN_BIT, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}},
            // No comments
            {0b11011001, {InstructionID::INSN_FMLSL2, [](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return ""; }}},
            {0b11100011, {InstructionID::INSN_BIF, [Q](void) -> std::expected<std::string_view, disxx::utility::error::DisassemblyError> { return Q == 0b1 ? "16b" : "8b"; }}}
        };

        unsigned short int encoding = (U << 5) | opcode;
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end())
        {
            encoding = (U << 7) | (size << 5) | opcode;
            if (opcode != 0b00011 && opcode != 0b11101)
                encoding &= ~(0b01 << 5);
            it = insnTableWithSize.find(opcode);
            if (it == insnTableWithSize.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        }

        auto [insn, specifierFunc]{it->second};
        const auto &spec{specifierFunc()};
        if (!spec) [[unlikely]]
            return std::unexpected{spec.error()};
        if (encoding == 0b01000011 && Rm == Rn)
            insn = InstructionID::INSN_MOV;

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
    
        const auto specArray
        {
            [&opcode, &Q](std::string_view spec) -> std::array<std::string, 3>
            {
                if (opcode == 0b11001)
                {
                    return
                    {
                        Q == 0b1 ? "4s" : "2s",
                        Q == 0b1 ? "4h" : "2h",
                        Q == 0b1 ? "4h" : "2h"
                    };
                }

                const auto T{spec.data()};
                return {T, T, T};
            }(spec.value())
        };

        for (auto i{0}; const auto &T : specArray)
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(i++).get())->SetArrangementSpecifier(T);

        return std::make_pair(insn, std::move(this->m_Operands));
		*/
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeSame */
