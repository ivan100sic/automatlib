#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include "relacija.h"

namespace automati {

class relacija;

class skup {
	size_t n;
	std::vector<unsigned char> a;

	void eq(const skup& b) const;

public:

	size_t size() const { return n; }

	explicit skup(size_t n);

	skup(const std::string& s);

	unsigned char& operator[] (const size_t x);

	const unsigned char& operator[] (const size_t x) const;

	// unija
	skup& operator|= (const skup& b);

	skup operator| (const skup& b) const;

	// presek
	skup& operator&= (const skup& b);

	skup operator& (const skup& b) const;

	void negate();

	// negacija
	skup operator! () const;

	// relacije poredjenja
	bool operator== (const skup& b) const;
	bool operator!= (const skup& b) const;
	bool operator<= (const skup& b) const;
	bool operator>= (const skup& b) const;
	bool operator< (const skup& b) const;
	bool operator> (const skup& b) const;

	bool podskup (const skup& b) const;

	bool nadskup (const skup& b) const;
	
	unsigned char operator* (const skup& b) const;

	explicit operator std::string () const;
};

}