#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <queue>
#include "nfa.h"
#include "sr-cross.h"

namespace automati {

nfa::nfa(const std::vector<relacija>& d, const std::string& l,
	const skup& s, const skup& t) : d(d), s(s), t(t)
{
	if (!d.size() || !d[0].rows())
		throw std::length_error("empty automata are not allowed");
	for (size_t i=0; i<d.size(); i++)
		if (d[i].rows() != d[0].rows() || d[i].rows() != d[i].cols())
			throw std::length_error("relation size mismatch");
	if (s.size() != d[0].rows() || t.size() != d[0].rows())
		throw std::length_error("relation-set size mismatch");
	if (l.size() != d.size())
		throw std::length_error("relation-alphabet size mismatch");
	for (size_t i=0; i<l.size(); i++) {
		if (mp.count(l[i]))
			throw std::length_error("duplicate symbol in alphabet");
		mp[l[i]] = i;
	}
}

skup nfa::pokreni(const std::string& b) const {
	if (b.size() == 0)
		return s;
	char mn = *std::min_element(b.begin(), b.end());
	skup u = s;
	for (char x : b)
		u = u * d[mp.at(x)];
	return u;
}

bool nfa::raspoznaje(const std::string& b) const {
	return pokreni(b) * t;
}

relacija nfa::desno_inv_kvur() const {
	relacija r = t / t;
	while (1) {
		relacija r_old = r;
		for (const auto& dx : d) {
			relacija tmp = dx * r_old;
			r &= tmp / tmp;
		}
		if (r == r_old)
			return r;
	}
}

relacija nfa::levo_inv_kvur() const {
	relacija r = s % s;
	while (1) {
		relacija r_old = r;
		for (const auto& dx : d) {
			relacija tmp = r_old * dx;
			r &= tmp % tmp;
		}
		if (r == r_old)
			return r;
	}
}

relacija nfa::desno_inv_ekv() const {
	relacija tmp = t / t;
	relacija r = tmp & tmp.T();
	while (1) {
		relacija r_old = r;
		for (const auto& dx : d) {
			tmp = dx * r_old;
			tmp = tmp / tmp;
			tmp = tmp & tmp.T();
			r &= tmp;
		}
		if (r == r_old)
			return r;
	}
}

relacija nfa::levo_inv_ekv() const {
	relacija tmp = s % s;
	relacija r = tmp & tmp.T();
	while (1) {
		relacija r_old = r;
		for (const auto& dx : d) {
			tmp = r_old * dx;
			tmp = tmp % tmp;
			tmp = tmp & tmp.T();
			r &= tmp;
		}
		if (r == r_old)
			return r;
	}
}

std::vector<skup> nfa::tau_skup() const {
	std::set<skup> svi;
	std::vector<skup> red;

	size_t kraj = 0;
	svi.insert(t);
	red.emplace_back(t);

	while (kraj != red.size()) {
		skup r = red[kraj++];
		for (const auto& dx : d) {
			skup dr = dx * r;
			if (svi.insert(dr).second)
				red.emplace_back(std::move(dr));
		}
	}

	return red;
}

std::vector<skup> nfa::sigma_skup() const {
	std::set<skup> svi;
	std::vector<skup> red;

	size_t kraj = 0;
	svi.insert(s);
	red.emplace_back(s);

	while (kraj != red.size()) {
		skup r = red[kraj++];
		for (const auto& dx : d) {
			skup dr = r * dx;
			if (svi.insert(dr).second)
				red.emplace_back(std::move(dr));
		}
	}

	return red;
}

relacija nfa::slabo_desno_inv_kvur() const {
	auto ts = tau_skup();
	auto q = relacija(t.size(), t.size());
	q.negate();
	for (const auto& tu : ts)
		q &= tu / tu;
	return q;
}

relacija nfa::slabo_levo_inv_kvur() const {
	auto ss = sigma_skup();
	auto q = relacija(t.size(), t.size());
	q.negate();
	for (const auto& su : ss)
		q &= su % su;
	return q;
}

relacija nfa::slabo_desno_inv_ekv() const {
	auto q = slabo_desno_inv_kvur();
	return q & q.T();
}

relacija nfa::slabo_levo_inv_ekv() const {
	auto q = slabo_levo_inv_kvur();
	return q & q.T();
}

// nfa nfa::faktor(const relacija& r) const {
// 	static_assert(false);
// }

}