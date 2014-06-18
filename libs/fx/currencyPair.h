#pragma once


///////////////////////////////////////////////////////////////////
class Instrument
{
public:
	typedef uint32_t Id;
	struct Config
	{
		Id			m_id;
		std::string	m_name;

		friend std::ostream& operator<<(std::ostream& os_, const Config& cfg_)
		{ return os_ << '{' << cfg_.m_name << ",id="<< cfg_.m_id << '}'; }
	};
	Instrument() = default;
	Instrument(const Config& cfg_) : m_cfg(cfg_) {}
	const Config& getConfig() const { return m_cfg; }
	friend std::ostream& operator<<(std::ostream& os_, const Instrument& inst_)
	{ return os_ << inst_.getConfig(); }
private:
	Config m_cfg;
};

///////////////////////////////////////////////////////////////////
class Currency : public Instrument
{
public:
	struct Config : public Instrument::Config
	{
	};
	Currency(const Config& cfg_) : Instrument(cfg_) {}
	friend std::ostream& operator<<(std::ostream& os_, const Currency& ccy_)
	{ return os_ << static_cast<const Instrument&>(ccy_); }
};

///////////////////////////////////////////////////////////////////
class CurrencyPair : public Instrument
{
public:
	struct Config : public Instrument::Config
	{
		double m_onePip;		// 0.0001 or 0.01 (USDJPY)
		double m_pipPrecision;	// how many pips precision is required
	};

	CurrencyPair(const Config& cfg_, const Currency& ccy1_, const Currency& ccy2_) : 
		Instrument(cfg_),
		m_ccy1(ccy1_), 
		m_ccy2(ccy2_) 
	{}
    const Currency& getCcy1() const { return m_ccy1;}	
    const Currency& getCcy2() const { return m_ccy2;}	
	friend std::ostream& operator<<(std::ostream& os_, const CurrencyPair& cp_)
	{ return os_ << '{' << static_cast<const Instrument&>(cp_) << cp_.m_ccy1 << cp_.m_ccy2 << '}'; }

private:
	Config			m_cfg;
	const Currency&	m_ccy1;
	const Currency&	m_ccy2;
};

///////////////////////////////////////////////////////////////////
#include <unordered_map>

///////////////////////////////////////////////////////////////////
class InstrumentManager
{
public:
	const Instrument& getInstrument(const std::string& name_)
	{
        return *m_name2Inst[name_];
	}
	const Instrument& getInstrument(Instrument::Id id_)
	{
        return *m_Id2Inst[id_];
	}
	bool insertInstrument(const Instrument& inst_)
	{
		auto ret1 = m_name2Inst.emplace(inst_.getConfig().m_name, &inst_);
		auto ret2 = m_Id2Inst.emplace(inst_.getConfig().m_id, &inst_);
        return ret1.second && ret2.second;
	}
private:
	std::unordered_map<std::string, const Instrument*> m_name2Inst;
	std::unordered_map<Instrument::Id, const Instrument*> m_Id2Inst;
};

