#include "relacija.h"
#include "skup.h"

namespace automati {

skup operator* (const skup& a, const relacija& b) {
	if (a.size() != b.rows())
		throw std::length_error("operand size mismatch");
	skup t(b.cols());
	for (size_t i=0; i<b.rows(); i++)
		for (size_t j=0; j<b.cols(); j++)
			t[j] |= a[i] && b[i][j];
	return t;
}

skup operator* (const relacija& a, const skup& b) {
	if (a.cols() != b.size())
		throw std::length_error("operand size mismatch");
	skup t(a.rows());
	for (size_t i=0; i<a.rows(); i++)
		for (size_t j=0; j<a.cols(); j++)
			t[i] |= a[i][j] && b[j];
	return t;
}

// levi rezidual
relacija operator/ (const skup& b, const skup& a) {
	relacija c(b.size(), a.size());
	for (size_t i=0; i<a.size(); i++)
		for (size_t j=0; j<b.size(); j++)
			c[j][i] = a[i] <= b[j];
	return c;
}

// desni rezidual
relacija operator% (const skup& a, const skup& b) {
	relacija c(a.size(), b.size());
	for (size_t i=0; i<a.size(); i++)
		for (size_t j=0; j<b.size(); j++)
			c[i][j] = a[i] <= b[j];
	return c;
}

}