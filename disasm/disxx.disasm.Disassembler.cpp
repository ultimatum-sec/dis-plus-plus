module;

#include <algorithm>
#include <optional>
#include <charconv>
#include <utility>
#include <ranges>
#include <string>
#include <regex>

module disxx.disasm.Disassembler;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.decoder.DecoderFactory;
import disxx.disasm.Address;

namespace disxx::disasm
{
	Disassembler::Disassembler(bool fatalErrors) noexcept
		: m_FatalErrors{fatalErrors}
	{}

	Disassembler &&Disassembler::FatalErrors(bool fatalErrors) noexcept
	{
		this->m_FatalErrors = fatalErrors;
		return std::move(*this);
	}

	std::expected<Instruction, disxx::utility::error::DisassemblyError> Disassembler::DisassembleSingle(Bytes word, Address addr) const noexcept(false)
	{
		const auto &obj
		{
			decoder::DecoderFactory::GetDecoder
			(
				 integer(word),
                 integer(addr)
			)
		};

		auto &&result{obj->Decode()};
		if (!result) [[unlikely]]
			return std::unexpected{result.error()};
		auto &[id, oprs]{result.value()};

		auto programCounterRelevantAddress
		{
			[&obj, &oprs](void) -> std::optional<signed long long int>
			{
				if (!obj->IsProgramCounterRelevant())
					return std::nullopt;

				const auto &it
            	{
					std::ranges::find_if
                	(
                	    oprs,
                	    [](const auto &ptr) -> bool
                	    { return ptr->GetType() == operand::AbstractOperand::Type::TYPE_IMMEDIATE; }
                	)
            	};

            	if (it == oprs.end()) [[unlikely]]
            		return std::nullopt;
               
            	auto str{(*it)->GetMnemonic()};
            	// Is this value non-hex?
            	if (!str.starts_with("#0x")) [[unlikely]]
            		return std::nullopt;

            	// Remove the "#0x" substring
            	str = std::regex_replace(str, std::regex{R"(#0x)"}, std::string{""});
            	
				if (signed long long int val{}; std::from_chars(str.data(), str.data() + str.size(), val, 16)) [[likely]]
					return val;
				return std::nullopt;
			}()
		};

		return std::expected<Instruction, disxx::utility::error::DisassemblyError> 
		{	
			Instruction{}
				.SetBytes(std::move(word))
				.SetAddress(std::move(addr))
				.SetInstructionID(std::move(id))
				.SetOperands(std::move(oprs))
				.SetProgramCounterRelevantAddress(std::move(programCounterRelevantAddress))
		};
	}
} /* disxx::disasm */


