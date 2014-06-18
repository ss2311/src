#pragma once

#include<cmath>

namespace tools {

///////////////////////////////////////////////////////////////////////////////
//
template<typename T>
inline constexpr T power(T num_, unsigned exp_)
{
    return exp_ == 0 ? 1 : num_ * pow(num_, exp_ - 1);
}

/////////////////////////////////////////////////////////////////////////////////
////
//double ceil(double val_, unsigned dp_)
//{
//    val_ *= power(10, dp_);
//    val_ = std::ceil(val_);
//    val_ /= power(10, dp_);
//    return val_;
//}
//
/////////////////////////////////////////////////////////////////////////////////
////
//double floor(double val_, unsigned dp_)
//{
//    val_ *= power(10, dp_);
//    val_ = std::floor(val_);
//    val_ /= power(10, dp_);
//    return val_;
//}
//
/////////////////////////////////////////////////////////////////////////////////
////
//double round(double val_, unsigned dp_)
//{
//    val_ *= power(10, dp_);
//    val_ = std::round(val_);
//    val_ /= power(10, dp_);
//    return val_;
//}
//
/////////////////////////////////////////////////////////////////////////////////
////
//double trunc(double val_, unsigned dp_)
//{
//    val_ *= power(10, dp_);
//    val_ = std::trunc(val_);
//    val_ /= power(10, dp_);
//    return val_;
//}

enum class RoundType
{
    Round,  // rounds to closest value +/-1.5 => +/-2.0, +/-1.4 => +/-1.0
    Up,     // rounds up +/- 1.5
    Down,   // rounds down
    Trunc   // truncates +/-1.5 => 1.0, +/-1.4 => +/-1.0
};

///////////////////////////////////////////////////////////////////////////////
//
template<unsigned Decimals> // Number of decimal places
class Double
{
public:
    typedef int64_t Type;
    
    // ctors
    Double() : m_value(0) {}
    Double(double val_, RoundType type_= RoundType::Round)
    {
        switch(type_)
        {
            case RoundType::Up     :
                m_value = roundUp(val_); break;
            case RoundType::Down   :
                m_value = roundDn(val_); break;
            case RoundType::Trunc  :
                m_value = this->trunc(val_); break;
            default                :
            case RoundType::Round  :
                m_value = this->round(val_); break;
            
        }
    }
    // copy ctor
    Double(const Double& rhs_) : m_value(rhs_.m_value) {}

    // getter
    double asDouble() const
    {
        return (static_cast<double>(m_value)) / getScalingFactor();
    }
    
    // Logical operators
    bool operator < (const Double& rhs_) const{ return m_value < rhs_.m_value; }
    bool operator > (const Double& rhs_) const{ return m_value > rhs_.m_value; }
    bool operator <= (const Double& rhs_) const{ return m_value <= rhs_.m_value; }
    bool operator >= (const Double& rhs_) const{ return m_value >= rhs_.m_value; }
    bool operator == (const Double& rhs_) const { return m_value == rhs_.m_value; }
    
    // Arithmetic Operators
    Double operator+(const Double& rhs_) const { return Double(m_value + rhs_.m_value, true); }
    Double operator-(const Double& rhs_) const { return Double(m_value - rhs_.m_value, true); }
    Double operator*(const Double& rhs_) const { return Double(m_value * rhs_.m_value, true); }
    Double operator/(const Double& rhs_) const { return Double(m_value / rhs_.m_value, true); }
    
    // Getters
    unsigned getDecimalPlaces() const { return Decimals; }
    uint64_t getScalingFactor() const { return power(10, Decimals); }
    Type getRawValue() const { return m_value; }

private:
    ///////////////////////////////////////////////////////////////////////////////
    //
    Type roundUp(double val_)
    {
        val_ *= getScalingFactor();
        val_ = std::ceil(val_);
        return val_;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    Type roundDn(double val_)
    {
        val_ *= getScalingFactor();
        val_ = std::floor(val_);
        return val_;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    Type round(double val_)
    {
//        double delta = val_ >= 0 ? 0.5 : -0.5;
//        return (val_ * getScalingFactor()) + delta;
        
        val_ *= getScalingFactor();
        val_ = std::round(val_);
        return val_;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    Type trunc(double val_)
    {
        return val_ * getScalingFactor();
//        val_ *= getScalingFactor();
//        val_ = std::trunc(val_);
//        return val_;
    }
    
    // Only used internally
    Double(Type val_, bool) : m_value(val_) {}
    
    Type    m_value;
};

}