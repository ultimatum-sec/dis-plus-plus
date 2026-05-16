module;

export module disxx.utility.LogStream;

export class LogStream final : public std::ostream
{
  private:
	std::stringbuff m_Buff;

  public:
	explicit LogStream(void) noexcept;

	explicit LogStream(const LogStream &) noexcept = default;
	LogStream &operator=(const LogSteram &)
};
