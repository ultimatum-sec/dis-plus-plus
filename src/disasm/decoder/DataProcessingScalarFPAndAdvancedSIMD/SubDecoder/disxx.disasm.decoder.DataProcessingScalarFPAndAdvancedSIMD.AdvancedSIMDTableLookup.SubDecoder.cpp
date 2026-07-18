module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <ranges>
#include <format>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTableLookup.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTableLookup
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
        // +-+-+------+---+-+--+-+---+--+--+--+--+
        // |0|Q|001110|op2|0|Rm|0|len|op|00|Rn|Rd|
        // +-+-+------+---+-+--+-+---+--+--+--+--+

        unsigned short int Q, op2, Rm, len, op, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        len = bits::extract<unsigned short int, std::uint32_t, 13, 14>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 12, 12>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000, InstructionID::INSN_TBL},
            {0b0001, InstructionID::INSN_TBLX},
            {0b1011, InstructionID::INSN_LUTI4},
            {0b1010, InstructionID::INSN_LUTI4},
            {0b1101, InstructionID::INSN_LUTI2},
            {0b1110, InstructionID::INSN_LUTI2}
        };

        unsigned short int encoding = (Q << 3) | (op2 << 1) | op;
        if (op2 == 0b00)
            encoding &= ~(1 << 3);
        else if (op2 == 0b11)
            encoding &= ~0b1;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rd
			)
		);
        if (op2 == 0b00)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
				->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier(Q));
        
            for (auto Ri : std::views::iota(Rn, static_cast<unsigned short int>(Rn + len + 1)))
            {
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_V,
						Ri
					)
				);
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
					->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b001});
            }
            
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rm
				)
			);
			static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
				->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier(Q));
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (op2 == 0b01)
        {
            if (op == 0b0 && !(len & ~(0b1 << 1))) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            if (op == 0b1)
            {
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
					->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b000});
            	this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_V,
						Rn
					)
				);
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
					->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b000});
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_V,
						Rn + 1
					)
				);
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
					->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b000});
            }
            else
            {
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_V,
						Rn
					)
				);
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
					->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b001});
            }

            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rm
				)
			);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
                ->SetVectorArrangementSpecifier(std::format("[{}]", op == 0b1 ? len : (len >> op))); // TODO: Check op condition

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{op});
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{op});
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rm
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
            ->SetVectorArrangementSpecifier(std::format("[{}]", op2 == 0b10 ? len : (len << 1) | op));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTableLookup */
