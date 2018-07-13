#include "Polynomial.h"
#include <sstream>

using namespace std;

//////////////////////////////
// Author: Matthew Leandres //
//////////////////////////////

Polynomial::Polynomial(int cofficient, int exponent)
	: capacity{5}, coeffs{new int[capacity]}, max_exp{exponent} {
		// need to make sure capacity is big enough
		while(max_exp >= capacity){
			delete [] coeffs;
			capacity *= 2;
			coeffs = new int[capacity];
		}
		for (int i = 0; i < capacity; ++i) {
			coeffs[i] = 0;
		}
		coeffs[exponent] = cofficient;
	}

int xPos(const string &s);
void parseTerm(int &c, int &e, const string &s);

// will use above and +, -, and = operators to do this
Polynomial::Polynomial(const string &str) :
	capacity{5}, coeffs{new int[capacity]}, max_exp{0} {

		// parse string using iss and oss
		istringstream iss{str};

		// gets each term and puts them in the right spot
		bool neg = false;
		string s;

		while (iss >> s) {

			// if s is '-' then next coeff is negative
			if (s[0] == '-') {
				neg = true;
			}

			// if s is '+' then next coeff is positive
			else if (s[0] == '+') {
				neg = false;
			}

			// otherwise it is a term.
			// term can be exp = 0, exp = 1, exp > 1
			else {
				int c = 0;
				int e = 0;
				// parses term
				parseTerm(c, e, s);

				Polynomial t{c, e}; // makes new polynomial on stack (with heap alloced coeffs)

				// adds or subtracts term to polynomial with + or - operator
				if (neg) *this = *this - t;
				else *this = *this + t;
			}
		}
	}

// takes two references to ints and changes them to the coeff and exp of
//  a polynomial term taken from the referenced string s
void parseTerm(int &c, int &e, const string &s) {
	int len = s.length();

	// find pos of x
	int xp = xPos(s);

	// case when exp = 0
	if (xp == -1) {
		istringstream num{s};
		num >> c;
		e = 0;
		return;
	}

	// case when exp = 1
	else if (xp == len - 1) {
		e = 1;
		// case when coeff = 1
		if (xp == 0) {
			c = 1;
			return;
		}

		// coeff > 1
		else {
			istringstream num{s.substr(0, len - 1)};
			num >> c;
			return;
		}
	}

	// case when exp > 1
	else {

		// case when exp > 1, coeff = 1
		if (xp == 0) {
			c = 1;
			istringstream num{s.substr(2, len - 2)};
			num >> e;
		}

		// case when exp > 1, coeff > 1
		else {
			istringstream numC{s.substr(0, xp)};
			istringstream numE{s.substr(xp + 2, len - xp - 2)};

			numC >> c;
			numE >> e;
		}
	}
}



// function that finds the position of char x in a string, returns -1 if x
// 	is not there
int xPos(const string &s) {
	int len = s.length();
	int xp = -1;
	for (int i = 0; i < len; ++i) {
		if (s[i] == 'x') {
			xp = i;
			break;
		}
	}
	return xp;
}

// Dtor, need to delete heap allocated array
Polynomial::~Polynomial() {
	delete [] coeffs;
}

// Copy Ctor
Polynomial::Polynomial(const Polynomial &other)
	: capacity{other.capacity}, coeffs{new int[other.capacity]}, max_exp{other.max_exp} {
		// need to init new array with copied values.
		for (int i = 0; i < capacity; ++i) {
			coeffs[i] = other.coeffs[i];
		}
	}

// Move Ctor
Polynomial::Polynomial(Polynomial &&other)
	: capacity{other.capacity}, coeffs{other.coeffs},  max_exp{other.max_exp} {
		other.coeffs = nullptr;
	}

// swap function to use in Copy Assignment operator
void swapPoly(Polynomial &p, Polynomial &other) {
	int tmpCap = p.capacity;
	int tmpMax = p.max_exp;
	int *tmpCoeffs = p.coeffs;

	p.capacity = other.capacity;
	p.max_exp = other.max_exp;
	p.coeffs = other.coeffs;

	other.capacity = tmpCap;
	other.max_exp = tmpMax;
	other.coeffs = tmpCoeffs;
}

// Copy Assignment
Polynomial &Polynomial::operator=(const Polynomial &other) {
	if (this == &other) return *this;
	// call copy ctor
	Polynomial tmp = other;
	swapPoly(*this, tmp);
	// this is now other, tmp is old this
	return *this;
	// tmp destroyed using dtor when out of scope
}

// Move Assignment
Polynomial &Polynomial::operator=(Polynomial &&other) {
	swapPoly(*this, other);
	return *this;
}

bool Polynomial::operator==(const Polynomial &rhs) const {
	if (max_exp != rhs.max_exp) return false;
	else {
		for (int i = 0; i <= max_exp; ++i) {
			if (coeffs[i] != rhs.coeffs[i]) return false;
		}
	}
	return true;
}

// function that finds new max_exp of a polynomial
int newMax(Polynomial &p) {
	for (int i = p.max_exp; i >= 0; --i) {
		if (p.coeffs[i] != 0) {
			return i;
		}
	}
	return 0;
}

Polynomial Polynomial::operator+(const Polynomial &rhs) const {
	// larger max_exp
	int max = max_exp > rhs.max_exp ? max_exp : rhs.max_exp;
	// smaller max_exp
	int min = max_exp < rhs.max_exp ? max_exp : rhs.max_exp;

	Polynomial ret{0, max};

	// add values up until smaller max_exp
	for (int i = 0; i <= min; ++i) {
		ret.coeffs[i] = coeffs[i] + rhs.coeffs[i];
	}

	// assign for rest of values for whichever is bigger
	if (max_exp > rhs.max_exp) {
		for (int i = min+1; i <= max; ++i) {
			ret.coeffs[i] = coeffs[i];
		}
	}
	else {
		for (int i = min+1; i <= max; ++i) {
			ret.coeffs[i] = rhs.coeffs[i];
		}
	}

	// if last coeff got cancelled, need to adjust max_exp
	if (ret.coeffs[ret.max_exp] == 0) {
		ret.max_exp = newMax(ret);
	}

	return ret;
}

Polynomial Polynomial::operator-(const Polynomial &rhs) const {
	// larger max_exp
	int max = max_exp > rhs.max_exp ? max_exp : rhs.max_exp;
	// smaller max_exp
	int min = max_exp < rhs.max_exp ? max_exp : rhs.max_exp;

	Polynomial ret{0, max};

	// subtract values up until smaller max_exp
	for (int i = 0; i <= min; ++i) {
		ret.coeffs[i] = coeffs[i] - rhs.coeffs[i];
	}

	// assign for rest of values for whichever is bigger
	if (max_exp > rhs.max_exp) {
		for (int i = min+1; i <= max; ++i) {
			ret.coeffs[i] = coeffs[i];
		}
	}
	else {
		for (int i = min+1; i <= max; ++i) {
			ret.coeffs[i] = 0 - rhs.coeffs[i];
		}
	}

	// if last coeff got cancelled, need to adjust max_exp
	if (ret.coeffs[ret.max_exp] == 0) {
		ret.max_exp = newMax(ret);
	}

	return ret;
}

// go through each and multiply with thing in other, add answer to new poly
Polynomial Polynomial::operator*(const Polynomial &rhs) const {
	Polynomial ret{0, 0};

	int maxL = max_exp;
	int maxR = rhs.max_exp;

	// for each term mult, make new polynomial of one term and add to ret
	for (int i = 0; i <= maxL; ++i) {
		for (int j = 0; j <= maxR; ++j) {
			int c = coeffs[i] * rhs.coeffs[j];
			int e = i + j;
			Polynomial t{c, e};
			ret = ret + t;
		}
	}

	// shouldnt need to do this but JUST IN CASE.
	if (ret.coeffs[ret.max_exp] == 0) {
		ret.max_exp = newMax(ret);
	}

	return ret;
}


int Polynomial::operator()(int x) const {
	int ret = coeffs[0];
	if (x == 0) return ret;

	// go through each
	for (int i = 1; i <= max_exp; ++i) {
		// exponent part
		int y = 1;
		for (int j = 0; j < i; ++j) {
			y *= x;
		}
		y *= coeffs[i];
		ret += y;
	}
	return ret;
}

int Polynomial::operator[](int exponent) const {
	if (exponent < 0) return 0;
	return coeffs[exponent];
}

std::ostream& operator<<(std::ostream& out, const Polynomial &poly) {
	ostringstream oss;
	int c = poly.coeffs[0];
	int max = poly.max_exp;
	bool isFirst = true;

	// other methods guarantee max_exp is non-zero except for zero poly
	if (poly.coeffs[max] == 0) oss << 0;

	else {
		// go through all
		for (int i = 0; i <= max; ++i) {
			c = poly.coeffs[i];
			if (c != 0) {

				// first check if coeff is neg
				if (poly.coeffs[i] < 0) {
					if (isFirst) {
						oss << "- ";
						isFirst = false;
					}
					else oss << " - ";
				}
				// poly is positive
				else {
					if (isFirst) {
						isFirst = false;
					}
					else {
						oss << " + ";
					}
				}

				// c is neg and not -1
				if ((c < 0) && (c != -1 || i == 0))  {
					oss << -1*c;
					// exp = 0
				}
				// c is positive and not 1
				else if ((c > 0) && (c != 1 || i == 0)) {
					oss << c;
				}

				// need x^ here
				if (i > 0) oss << "x";
				if (i > 1) oss << "^" << i;
			}
		}
	}
	out << oss.str();
	return out;
}

std::istream& operator>>(std::istream& in, Polynomial &poly) {
	string s;
	getline(in, s);
	poly = Polynomial{s};
	return in;
}


