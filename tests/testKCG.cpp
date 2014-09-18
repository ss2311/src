#include <boost/test/unit_test.hpp>
#include <iostream>		// cout
#include <stdexcept>	// runtime_error
#include <deque>		// deque<Location>
#include <stack>		// stack<Location>

//////////////////////////////////////////////////////////////////////
// NOTE:
// o Used std::deque instead of std::stack to represent stack - as its 
//   it is not possible (easily) to dump std::stack's contents

using namespace std;


//////////////////////////////////////////////////////////////////////
// Yes the author on purpose did not check for 'U' or lower case letters
inline bool isVowel(char ch_)  
{
	return (ch_ == 'A' || ch_ == 'E' || ch_ == 'I' || ch_ == 'O'); 
}

//////////////////////////////////////////////////////////////////////
// C++ representation of a location on the board/ keypad
struct Location
{
	unsigned m_row;
	unsigned m_col;

	Location(unsigned row_, unsigned col_) : m_row(row_), m_col(col_) {}
	string toString() const 
	{
		ostringstream oss;
		oss << *this;
		return oss.str();
	}
	friend ostream& operator << (ostream& os_, const Location& loc_)
	{ return os_ << '[' << loc_.m_row << ',' << loc_.m_col << ']' << endl; }
	friend bool operator==(const Location& lhs_, const Location& rhs_)
	{
		return (lhs_.m_row == rhs_.m_row) && (lhs_.m_col == rhs_.m_col);
	}
	static const char INVALID = '*';
};



//////////////////////////////////////////////////////////////////////
// C++ representation of the playing board / keypad
class Board
{
public:
	Board() { setup(); }

	static constexpr unsigned getNumRows() { return NUMROWS; }
	static constexpr unsigned getNumCols() { return NUMCOLS; }

	char at(const Location& loc_) const 
	{ 
		return isValidLocation(loc_) ? m_array[loc_.m_row][loc_.m_col] : Location::INVALID;
	}

	bool isValidLocation(const Location& loc_) const
	{
		//if(loc_.m_row >= getNumRows() || (loc_.m_col >= getNumCols()))
		if(loc_.m_row >= NUMROWS || (loc_.m_col >= NUMCOLS))
			return false;

		if(3 == loc_.m_row && (0 == loc_.m_col || 4 == loc_.m_col))
		   return false;

		return true;
	}

private:
	void setup()
	{
		m_array[0][0] = 'A'; m_array[0][1] = 'B'; m_array[0][2] = 'C'; m_array[0][3] = 'D'; m_array[0][4] = 'E';
		m_array[1][0] = 'F'; m_array[1][1] = 'G'; m_array[1][2] = 'H'; m_array[1][3] = 'I'; m_array[1][4] = 'J';
		m_array[2][0] = 'K'; m_array[2][1] = 'L'; m_array[2][2] = 'M'; m_array[2][3] = 'N'; m_array[2][4] = 'O';
		m_array[3][0] = '*'; m_array[3][1] = '1'; m_array[3][2] = '2'; m_array[3][3] = '3'; m_array[3][4] = '*';
	}

	static const unsigned NUMROWS = 4;
	static const unsigned NUMCOLS = 5;

	char m_array[NUMROWS][NUMCOLS];
};

struct LocationInfo
{
	Location m_loc;
	char m_value;
	bool m_isVowel;

	LocationInfo(const Board& board_, const Location& loc_) :
		m_loc(loc_),
		m_value(board_.at(m_loc)),
		m_isVowel(isVowel(m_value))
	{
	}
	friend ostream& operator << (ostream& os_, const LocationInfo& val_)
	{ return os_ << val_.m_loc << "=>" << val_.m_value; } 
};

//////////////////////////////////////////////////////////////////////
// C++ representation of Knight
class Knight
{
public:
	// ctor
	Knight(const Board& board_, unsigned vowelCountMAX_) : 
		m_board(board_), m_vowelCount(0), m_vowelCountMAX(vowelCountMAX_)
	{
	}

	// is location valid for knight -- board & vowel limitation
	bool isValidLocation(const Location& loc_) const
	{
		char ch = m_board.at(loc_);
		if(Location::INVALID != ch)
		{
			if(isVowel(ch) && m_vowelCount >= m_vowelCountMAX)
/				return false;
			return true;
		}
		return false;
	}

	// returns deque of moves that can be made from current location
	deque<Location> possibleMoves() const
	{
		/*
		static deque<Location> deqs[128];
		char ch = m_board.at(location());
		auto& deq = deqs[static_cast<unsigned>(ch)];
		if(false == deq.empty())
			return deq;
		*/
		deque<Location> deq;
		Location loc(0,0);

		// 2 steps horizontally and 1 step vertically
		loc = moveLeftUp();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveLeftDn();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveRightUp();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveRightDn();
		if(isValidLocation(loc))
			deq.push_back(loc);

		// 2 steps vertically and 1 step horizontally

		loc = moveUpLeft();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveUpRight();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveDnLeft();
		if(isValidLocation(loc))
			deq.push_back(loc);

		loc = moveDnRight();
		if(isValidLocation(loc))
			deq.push_back(loc);

		/*
		cout << ch << " => ";
		for(auto loc : deq)
			cout << m_board.at(loc) << " ";
		cout << endl;
		*/
		return deq;
	}

	// returns number of moves made so far
	unsigned getMoveCount() const
	{
		return m_moves.size();
	}

	// prints all the moves made so far
	void printMoves() const
	{
		cout << "Knight: ";
		for(auto loc : m_moves)
			cout << loc.m_value << " ";
		cout << endl;
	}

	// moves back to previous location -- back tracks
	void moveBack()
	{
		// if the square that we back tracked from was a vowel then decr the count
		if(m_moves.back().m_isVowel)
			m_vowelCount--; 
		m_moves.pop_back();
	}
	
	// move to new location 
	void moveTo(const LocationInfo& locInfo_)
	{
		m_moves.push_back(locInfo_);
		if(locInfo_.m_isVowel)
			m_vowelCount++;
	}

	//////////////////////////////////////////////////////////////////////
	const Location& location() const { return m_moves.back().m_loc; }
private:

	//////////////////////////////////////////////////////
	// move routines

	// 2 steps vertically and 1 step horizontally
	Location moveUpLeft()  const { return move(-2, -1); }
	Location moveUpRight() const { return move(-2, +1); }
	Location moveDnLeft()  const { return move(+2, -1); }
	Location moveDnRight() const { return move(+2, +1); }

	// 2 steps horizontally and 1 step vertically
	Location moveLeftUp()  const { return move(-1, -2); }
	Location moveLeftDn()  const { return move(+1, -2); }
	Location moveRightUp() const { return move(-1, +2); }
	Location moveRightDn() const { return move(+1, +2); }
	//////////////////////////////////////////////////////

	// generic move routine -- takes offset as arguments
	Location move(int rowOffset_, int colOffset_) const
	{
		/*
		if(2 != abs(rowOffset_) && 1 != abs(rowOffset_))
			throw runtime_error("Invalid row offset");

		if(2 != abs(colOffset_) && 1 != abs(colOffset_))
			throw runtime_error("Invalid col offset");
		*/
		const Location& currLoc = location();
		return Location(currLoc.m_row + rowOffset_, currLoc.m_col + colOffset_);
	}
	const Board& m_board;	// Board on which this Knight is on
	unsigned m_vowelCount;	// number of vowels seen so far
	unsigned m_vowelCountMAX;
	deque<LocationInfo> m_moves;	// moves this knight has made so far
};


//////////////////////////////////////////////////////////////////////
// C++ representation of Game Logic
class Game
{
public:

	Game(unsigned maxMoves_, unsigned maxVowels_) : 
		m_maxMoves(maxMoves_),
		m_maxVowels(maxVowels_),
		m_numSequences(0)	
	{}
	
	//////////////////////////////////////////////////////////////////////
	void move(Knight& knight_)
	{
		while(false == m_moves.empty())
		{
			Location newLoc = m_moves.back();
			LocationInfo locInfo(m_board, newLoc);
			{
				knight_.moveTo(locInfo);
				if(knight_.getMoveCount() >= m_maxMoves) // TODO  move count++ inside knight
				{
					//printMoves(); //:-- for debugging only
					//knight_.printMoves();
					m_numSequences++;
					knight_.moveBack();
					m_moves.pop_back();
					while(false == m_moves.empty() && knight_.location() == m_moves.back())
					{
						knight_.moveBack();
						m_moves.pop_back();
					}
					continue;
				}
				//printMoves(knight_); // for debugging only
				for(auto& loc : knight_.possibleMoves())
					m_moves.push_back(loc);
			}
		}
	}

	void printMoves(const Knight& knight_) const
	{
		cout << "possibleMoves: " << m_board.at(knight_.location()) << " => ";
		for(auto& loc : knight_.possibleMoves())
			cout << m_board.at(loc) << " ";
		cout << endl;
	}

	// prints all the moves made so far
	void printMoves() const
	{
		cout << "Board: ";
		for(auto& loc : m_moves)
			cout << m_board.at(loc) << " ";
		cout << endl;
	}

	//////////////////////////////////////////////////////////////////////
	void startFrom(const Location& loc_)
	{
		if(false == m_board.isValidLocation(loc_))
			return;
		Knight knight(m_board, m_maxVowels); 
		m_moves.push_back(loc_);
		move(knight);
	}

	//////////////////////////////////////////////////////////////////////
	void run()
	{
		for(unsigned r = 0; r < m_board.getNumRows(); ++r)
			for(unsigned c = 0; c < m_board.getNumCols(); ++c)
				startFrom(Location(r,c));
		cout << m_numSequences << endl;
	}
	
	unsigned getNumSequences() const { return m_numSequences; }
private:
	Board  m_board;				// the board / keypad
	const unsigned m_maxMoves;	// max num of moves allowed
	const unsigned m_maxVowels;	// max num of vowels allowed
	deque<Location> m_moves;	// stack for DFS
	unsigned m_numSequences;	// num sequences
};

/////////////////////////////////////////////////////////////////////////////
struct PosInfo
{
	char m_char;
	bool m_isVowel;
	const char* m_moves;
};

//////////////////////////////////////////////////////////////////////
struct PosMgr
{
	PosInfo	m_posInfo[128];

	PosMgr()
	{
		m_posInfo['1'] = { '1', false, "NFH"};
		m_posInfo['2'] = { '2', false, "KOGI"};
		m_posInfo['3'] = { '3', false, "LHJ"};
		m_posInfo['A'] = { 'A', true, "HL"};
		m_posInfo['B'] = { 'B', false, "IKM"};
		m_posInfo['C'] = { 'C', false, "FJLN"};
		m_posInfo['D'] = { 'D', false, "GMO"};
		m_posInfo['E'] = { 'E', true, "HN"};
		m_posInfo['F'] = { 'F', false, "CM1"};
		m_posInfo['G'] = { 'G', false, "DN2"};
		m_posInfo['H'] = { 'H', false, "AKEO13"};
		m_posInfo['I'] = { 'I', true, "BL2"};
		m_posInfo['J'] = { 'J', false, "CM3"};
		m_posInfo['K'] = { 'K', false, "H2B"};
		m_posInfo['L'] = { 'L', false, "I3AC"};
		m_posInfo['M'] = { 'M', false, "FJBD"};
		m_posInfo['N'] = { 'N', false, "G1CE"};
		m_posInfo['O'] = { 'O', true, "H2D"};
	}
};

//////////////////////////////////////////////////////////////////////
// C++ representation of Game Logic
BOOST_AUTO_TEST_CASE(testKCG)
{
	Game game(10, 2);
	game.run();
	BOOST_CHECK_EQUAL(game.getNumSequences(), 1013398);
}
