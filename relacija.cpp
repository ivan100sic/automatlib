#include <vector>
#include <stdexcept>
#include <sstream>
#include "relacija.h"

namespace automati {

void relacija::eq(const relacija& b) const {
	if (n != b.n || m != b.m)
		throw std::length_error("operand size mismatch");
}

void relacija::nkv() const {
	if (n != m)
		throw std::length_error("relation matrix must be square");
}

relacija::relacija(size_t n, size_t m) : n(n), m(m),
	a(n, std::vector<unsigned char>(m, 0))
{
	if (!n || !m)
		throw std::length_error("empty relations are not allowed");
}

relacija::relacija(const std::string& s) {
	std::istringstream iss(s);
	std::vector<std::string> vs;
	std::string t;
	while (iss >> t)
		vs.emplace_back(move(t));
	for (size_t i=1; i<vs.size(); i++)
		if (vs[i].size() != vs[0].size())
			throw std::length_error("row size mismatch");
	n = vs.size();
	if (!n)
		throw std::length_error("empty relations are not allowed");
	m = vs[0].size();
	a.assign(n, std::vector<unsigned char>(m, 0));
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			a[i][j] = vs[i][j] == '1';
}


std::vector<unsigned char>& relacija::operator[] (const size_t x) {
	return a[x];
}

const std::vector<unsigned char>& relacija::operator[] (const size_t x) const {
	return a[x];
}

// unija
relacija& relacija::operator|= (const relacija& b) {
	eq(b);
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			a[i][j] |= b[i][j];
	return *this;
}

relacija relacija::operator| (const relacija& b) const {
	return relacija(*this) |= b;
}

// presek
relacija& relacija::operator&= (const relacija& b) {
	eq(b);
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			a[i][j] &= b[i][j];
	return *this;
}

relacija relacija::operator& (const relacija& b) const {
	return relacija(*this) &= b;
}

// transpozicija
relacija relacija::T() const {
	relacija t(m, n);
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			t[j][i] = a[i][j];
	return t;
}

// kompozicija
relacija relacija::operator* (const relacija& b) const {
	if (m != b.n)
		throw std::length_error("operand size mismatch");
	relacija t(n, b.m);
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			for (size_t k=0; k<b.m; k++)
				t[i][k] |= a[i][j] && b[j][k];
	return t;
}

relacija& relacija::operator*= (const relacija& b) {
	return *this = *this * b;
}

void relacija::negate() {
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			a[i][j] = !a[i][j];
}

// negacija
relacija relacija::operator! () const {
	relacija t = *this;
	t.negate();
	return t;
}

// levi rezidual
relacija relacija::operator/ (const relacija& b) const {
	if (m != b.m)
		throw std::length_error("operand size mismatch");
	relacija t(n, b.n);
	t.negate();
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<b.n; j++)
			for (size_t k=0; k<m; k++)
				t[i][j] &= b[j][k] <= a[i][k];
	return t;
}

// desni rezidual
relacija relacija::operator% (const relacija& b) const {
	if (n != b.n)
		throw std::length_error("operand size mismatch");
	relacija t(m, b.m);
	t.negate();
	for (size_t i=0; i<m; i++)
		for (size_t j=0; j<b.m; j++)
			for (size_t k=0; k<n; k++)
				t[i][j] &= a[k][i] <= b[k][j];
	return t;
}

// relacije poredjenja
bool relacija::operator== (const relacija& b) const { eq(b); return a == b.a; }
bool relacija::operator!= (const relacija& b) const { eq(b); return a != b.a; }
bool relacija::operator<= (const relacija& b) const { eq(b); return a <= b.a; }
bool relacija::operator>= (const relacija& b) const { eq(b); return a >= b.a; }
bool relacija::operator< (const relacija& b) const { eq(b); return a < b.a; }
bool relacija::operator> (const relacija& b) const { eq(b); return a > b.a; }

bool relacija::podskup (const relacija& b) const {
	eq(b);
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<m; j++)
			if (a[i][j] > b[i][j])
				return false;
	return true;
}

bool relacija::nadskup (const relacija& b) const {
	return b.podskup(*this);
}

relacija::operator std::string () const {
	std::string s(n*(m+1), 0);
	size_t k = 0;
	for (size_t i=0; i<n; i++) {
		for (size_t j=0; j<m; j++) {
			s[k++] = a[i][j] ? '1' : '0';
		}
		s[k++] = '\n';
	}
	return s;
}

bool relacija::je_refleksivna() const {
	nkv();
	for (size_t i=0; i<n; i++)
		if (!a[i][i])
			return false;
	return true;
}

bool relacija::je_simetricna() const {
	nkv();
	for (size_t i=0; i<n; i++)
		for (size_t j=i+1; j<n; j++)
			if (a[i][j] != a[j][i])
				return false;
	return true;
}

bool relacija::je_antisimetricna() const {
	nkv();
	for (size_t i=0; i<n; i++)
		for (size_t j=i+1; j<n; j++)
			if (a[i][j] && a[j][i])
				return false;
	return true;
}

bool relacija::je_tranzitivna() const {
	nkv();
	for (size_t i=0; i<n; i++)
		for (size_t j=0; j<n; j++)
			for (size_t k=0; k<n; k++)
				if (a[i][j] && a[j][k] && !a[i][k])
					return false;
	return true;
}

bool relacija::je_kvaziuredjenje() const {
	return je_refleksivna() && je_tranzitivna();
}

bool relacija::je_ekvivalencija() const {
	return je_kvaziuredjenje() && je_simetricna();
}

}