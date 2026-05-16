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
		: m_pSubDecoder{nullptr} // This is the reason of absence of the guarantees about m_pSubDecoder
		, m_ProgramCounter{}
		, m_Insn{}
		, m_Pad{}
	{}

	Decoder::Decoder(std::uint32_t insn, std::uint64_t programCounter) noexcept
		: m_pSubDecoder{nullptr}
		, m_ProgramCounter{programCounter}
		, m_Insn{insn}
		, m_Pad{}
	{}

	Decoder::Decoder(std::unique_ptr<SubDecoder> &&pSubDecoder) noexcept
		: m_pSubDecoder{std::move(pSubDecoder)}
		, m_ProgramCounter{}
		, m_Insn{}
		, m_Pad{}
	{}

	/*
	Decoder::Decoder(const Decoder &other) noexcept(false)
		: m_pSubDecoder{std::make_unique<SubDecoder>(*other.m_pSubDecoder)}
		, m_ProgramCounter{other.m_ProgramCounter}
		, m_Insn{other.m_Insn}
		, m_Pad{}
	{}

	Decoder &Decoder::operator=(const Decoder &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pSubDecoder)
				this->m_pSubDecoder.Get().reset();
			this->m_pSubDecoder.Get() = std::make_unique<SubDecoder>(*other.m_pSubDecoder);
			this->m_ProgramCounter = other.m_ProgramCounter;
			this->m_Insn = other.m_Insn;
		}

		return *this;
	}
	*/

	Decoder::Decoder(Decoder &&other) noexcept
		: m_pSubDecoder{std::move(other.m_pSubDecoder.Get())}
		, m_ProgramCounter{std::move(other.m_ProgramCounter)}
		, m_Insn{std::move(other.m_Insn)}
		, m_Pad{}
	{}

	Decoder &Decoder::operator=(Decoder &&other) noexcept
	{
		if (this->m_pSubDecoder)
			this->m_pSubDecoder.Get().reset();
		this->m_pSubDecoder.Get() = std::move(other.m_pSubDecoder.Get());
		this->m_ProgramCounter = std::move(other.m_ProgramCounter);
		this->m_Insn = std::move(other.m_Insn);
	
		return *this;
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
			this->m_pSubDecoder.Get() = std::move(decoder.value());
		else
			return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		return this
			->m_pSubDecoder
			->Decode();
	}
} /* disxx::disasm::decoder::abstract */
