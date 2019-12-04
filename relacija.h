#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include "skup.h"

namespace automati {

class skup;

class relacija {
	size_t n, m;
	std::vector<std::vector<unsigned char>> a;

	void eq(const relacija& b) const;

	void nkv() const;

public:

	size_t rows() const { return n; }
	size_t cols() const { return m; }

	relacija(size_t n, size_t m);

	relacija(const std::string& s);

	std::vector<unsigned char>& operator[] (const size_t x);

	const std::vector<unsigned char>& operator[] (const size_t x) const;

	// unija
	relacija& operator|= (const relacija& b);

	relacija operator| (const relacija& b) const;

	// presek
	relacija& operator&= (const relacija& b);

	relacija operator& (const relacija& b) const;

	// transpozicija
	relacija T() const;

	// kompozicija
	relacija operator* (const relacija& b) const;

	relacija& operator*= (const relacija& b);

	void negate();

	// negacija
	relacija operator! () const;

	// levi rezidual
	relacija operator/ (const relacija& b) const;

	// desni rezidual
	relacija operator% (const relacija& b) const;

	// relacije poredjenja
	bool operator== (const relacija& b) const;
	bool operator!= (const relacija& b) const;
	bool operator<= (const relacija& b) const;
	bool operator>= (const relacija& b) const;
	bool operator< (const relacija& b) const;
	bool operator> (const relacija& b) const;

	bool podskup (const relacija& b) const;

	bool nadskup (const relacija& b) const;

	operator std::string () const;

	bool je_refleksivna() const;

	bool je_simetricna() const;
	
	bool je_antisimetricna() const;

	bool je_tranzitivna() const;

	bool je_kvaziuredjenje() const;

	bool je_ekvivalencija() const;

};

}