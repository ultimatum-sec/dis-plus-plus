module disxx.disasm.Address;

namespace disxx::disasm
{
	Address operator++(Address &addr, int) noexcept
	{
		const Address old{addr};
		addr = static_cast<Address>(integer(addr) + 4ULL);
		
		return old;
	}
} /* disxx::disasm */


