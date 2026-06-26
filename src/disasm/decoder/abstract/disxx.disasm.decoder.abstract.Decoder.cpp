module;

#include <system_error>
#include <functional>
#include <stdexcept>
#include <exception>
#include <charconv>
#include <optional>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

module disxx.disasm.decoder.abstract.Decoder;

import disxx.disasm.operand.Immediate;

namespace disxx::disasm::decoder::abstract
{
	Decoder::Decoder(void) noexcept
		: m_pSubDecoder{}
		, m_ProgramCounter{}
		, m_Insn{}
	{}

	Decoder::Decoder(std::uint32_t insn, std::uint64_t programCounter) noexcept
		: m_pSubDecoder{}
		, m_ProgramCounter{programCounter}
		, m_Insn{insn}
	{}

	Decoder::Decoder(Decoder &&other) noexcept
		: m_pSubDecoder{std::move(other.m_pSubDecoder)}
		, m_ProgramCounter{std::move(other.m_ProgramCounter)}
		, m_Insn{std::move(other.m_Insn)}
	{}

	Decoder &Decoder::operator=(Decoder &&other) noexcept
	{
		if (this->m_pSubDecoder) [[likely]]
			this->m_pSubDecoder.Delete();
		this->m_pSubDecoder = std::move(other.m_pSubDecoder);
		this->m_ProgramCounter = std::move(other.m_ProgramCounter);
		this->m_Insn = std::move(other.m_Insn);
	
		return *this;
	}

	Decoder::~Decoder(void) noexcept
	{
		if (this->m_pSubDecoder) [[likely]]
			this->m_pSubDecoder.Delete();
	}

	bool Decoder::IsProgramCounterRelevant(void) const noexcept(false)
	{
		return this
			->m_pSubDecoder
			->GetProgramCounterRelevantAddress()
			.has_value();
	}

	DisassemblyResult Decoder::Decode(void) noexcept(false)
	{
		if (auto decoder{this->__GetDecoder()}) [[likely]]
			this->m_pSubDecoder = decoder.value().release();
		else
			return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		return this
			->m_pSubDecoder
			->Decode();
	}
} /* disxx::disasm::decoder::abstract */
