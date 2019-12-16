#include <vector>
#include <stdexcept>
#include <string>
#include "skup.h"

namespace automati {

void skup::eq(const skup& b) const {
	if (n != b.n)
		throw std::length_error("operand size mismatch");
}

skup::skup(size_t n) : n(n), a(n, 0) {}

skup::skup(const std::string& s) : n(s.size()), a(n) {
	for (size_t i=0; i<n; i++)
		a[i] = s[i] == '1';
}

unsigned char& skup::operator[] (const size_t x) {
	return a[x];
}

const unsigned char& skup::operator[] (const size_t x) const {
	return a[x];
}

// unija
skup& skup::operator|= (const skup& b) {
	eq(b);
	for (size_t i=0; i<n; i++)
		a[i] |= b[i];
	return *this;
}

skup skup::operator| (const skup& b) const {
	return skup(*this) |= b;
}

// presek
skup& skup::operator&= (const skup& b) {
	eq(b);
	for (size_t i=0; i<n; i++)
		a[i] &= b[i];
	return *this;
}

skup skup::operator& (const skup& b) const {
	return skup(*this) &= b;
}

void skup::negate() {
	for (size_t i=0; i<n; i++)
		a[i] = !a[i];
}

// negacija
skup skup::operator! () const {
	skup t = *this;
	t.negate();
	return t;
}

// relacije poredjenja
bool skup::operator== (const skup& b) const { eq(b); return a == b.a; }
bool skup::operator!= (const skup& b) const { eq(b); return a != b.a; }
bool skup::operator<= (const skup& b) const { eq(b); return a <= b.a; }
bool skup::operator>= (const skup& b) const { eq(b); return a >= b.a; }
bool skup::operator< (const skup& b) const { eq(b); return a < b.a; }
bool skup::operator> (const skup& b) const { eq(b); return a > b.a; }

bool skup::podskup (const skup& b) const {
	eq(b);
	for (size_t i=0; i<n; i++)
		if (a[i] > b[i])
			return false;
	return true;
}

bool skup::nadskup (const skup& b) const {
	return b.podskup(*this);
}

unsigned char skup::operator* (const skup& b) const {
	eq(b);
	for (size_t i=0; i<n; i++)
		if (a[i] && b[i])
			return true;
	return false;
}

skup::operator std::string () const {
	std::string s(n, 0);
	for (size_t i=0; i<n; i++)
		s[i] = a[i] ? '1' : '0';
	return s;
}


}