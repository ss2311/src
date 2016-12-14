#include <stdexcept>
#include <sstream>
#include <string>

struct Exception : public std::runtime_error
{
	Exception(const std::ostream& os_) : 
		std::runtime_error(static_cast<const std::ostringstream&>(os_).str())
	{ }

	friend std::ostream& operator << (std::ostream& os_, const Exception& exp_)
	{ return os_ << exp_.what(); }
};

#define EXCEPTION(x) Exception(std::ostringstream() << "Exception: " << x << " (" << __FILE__ << ',' << __LINE__ << ')')

