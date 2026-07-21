module;

#include <unordered_map>
#include <optional>
#include <utility>
#include <cstdint>
#include <vector>
#include <format>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarXIndexedElement.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarXIndexedElement
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
        // +--+-+-----+----+-+-+--+------+-+-+--+--+
        // |01|U|11111|size|L|M|Rm|opcode|H|0|Rn|Rd|
        // +--+-+-----+----+-+-+--+------+-+-+--+--+
 
        unsigned short int U, size, L, M, Rm, opcode, H, Rn, Rd;
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        M = bits::extract<unsigned short int, std::uint32_t, 20, 20>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 19>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        H = bits::extract<unsigned short int, std::uint32_t, 11, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        // Lambda to calculate index for instructions in insnTable
        const auto calcIndexInsnTable
        {
            [=] -> std::optional<unsigned short int>
            {
                switch (size)
                {
                  case 0b01:
                    return (H << 2) | (L << 1) | M;
    
                  case 0b10:
                    return (H << 1) | L;
    
                  default:
                    return std::nullopt;
                }
            }
        };

        // Lambda to calculate index for some instructions in insnTableWithSize
        const auto calcIndexInsnTableWithSize
        {
            [=] -> std::optional<unsigned short int>
            {
                switch (((size & 0b01) << 1) | L)
                {
                  case 0b00: case 0b01:
                    return (H << 1) | (L);

                  case 0b10:
                    return H;
    
                  default:
                    return std::nullopt;
                }
            }
        };

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<unsigned short int>>> insnTable = {
            {0b00011, {InstructionID::INSN_SQDMLAL, calcIndexInsnTable()}},
            {0b00111, {InstructionID::INSN_SQDMLSL, calcIndexInsnTable()}},
            {0b01011, {InstructionID::INSN_SQDMULL, calcIndexInsnTable()}},
            {0b01100, {InstructionID::INSN_SQDMULH, calcIndexInsnTable()}},
            {0b01101, {InstructionID::INSN_SQRDMULH, calcIndexInsnTable()}},
            {0b11101, {InstructionID::INSN_SQRDMLAH, calcIndexInsnTable()}},
            {0b11111, {InstructionID::INSN_SQRDMLSH, calcIndexInsnTable()}}
        };

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<unsigned short int>>> insnTableWithSize = {
            {0b0000001, {InstructionID::INSN_FMLA, (H << 2) | (L << 1) | M}},
            {0b0000101, {InstructionID::INSN_FMLS, (H << 2) | (L << 1) | M}},
            {0b0001001, {InstructionID::INSN_FMUL, (H << 2) | (L << 1) | M}},
            {0b0100001, {InstructionID::INSN_FMLA, calcIndexInsnTableWithSize()}},
            {0b0100101, {InstructionID::INSN_FMLS, calcIndexInsnTableWithSize()}},
            {0b0101001, {InstructionID::INSN_FMUL, calcIndexInsnTableWithSize()}},
            {0b1001001, {InstructionID::INSN_FMULX, (H << 2) | (L << 1) | M}},
            {0b1101001, {InstructionID::INSN_FMULX, calcIndexInsnTableWithSize()}}
        };

        unsigned short int Ts{};
        
        unsigned short int encoding = (U << 4) | opcode;
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end())
        {
            encoding = (U << 6) | (size << 4) | opcode;
            if (size >> 1)
                encoding &= ~(0b01 << 4);
            
            it = insnTableWithSize.find(encoding);
            if (it == insnTableWithSize.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            const auto rsize
			{
				size == 0b00
					? disxx::disasm::operand::Register::Type::TYPE_H
					: (
						(size & 0b01) == 0b1
							? disxx::disasm::operand::Register::Type::TYPE_D
							: disxx::disasm::operand::Register::Type::TYPE_S
					)
			};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(rsize, Rd));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(rsize, Rn));
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					size == 0b00 ? Rm : (M << 4) | Rm
				)
			);
            
            Ts = (size & 0b01) == 0b1 ? 0b1011 : 0b1010;
        }
        else
        {
			const auto [VaSize, VbSize]
			{
				[size] -> std::pair<disxx::disasm::operand::Register::Type, disxx::disasm::operand::Register::Type>
				{
					if (size == 0b01)
					{
						return std::make_pair
						(
							disxx::disasm::operand::Register::Type::TYPE_S,
							disxx::disasm::operand::Register::Type::TYPE_H
						);
					}

					return std::make_pair
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						disxx::disasm::operand::Register::Type::TYPE_D
					);
				}()
			};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(VaSize, Rd));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(VbSize, Rn));

            const auto Rmhi{size == 0b01 ? 0b0 : M};
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					(Rmhi << 4) | Rm
				)
			);
            
            Ts = size == 0b01 ? 0b1001 : 0b1010;
        }

        const auto &[insn, index]{it->second};
        if (!index) [[unlikely]]
        	return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
		static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
            ->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{Ts, *index});
        
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarXIndexedElement */
