#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <queue>
#include "nfa.h"
#include "sr-cross.h"

namespace automati {

std::string nfa::alfabet() const {
	std::string s;
	for (const auto& p : mp)
		s += p.first;
	return s;
}

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

nfa::operator std::string() const {
	std::string str;
	for (const auto& dx : d) {
		str += (std::string)dx;
		str += '\n';
	}

	str += (std::string)s;
	str += '\n';
	str += (std::string)t;
	str += '\n';
	return str;
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

nfa nfa::faktor(const relacija& r) const {
	auto f = r.prirodno_preslikavanje();
	std::vector<relacija> d2;
	d2.reserve(d.size());
	for (size_t i=0; i<d.size(); i++)
		d2.emplace_back(f.T() * r * d[i] * r * f);
	nfa a2 = *this;
	a2.d = d2;
	a2.s = s * r * f;
	a2.t = r * t * f;
	return a2;
}

nfa nfa::leva_naizmenicna_redukcija() const {
	bool levo = true;
	int fail = 0;
	nfa tmp = *this;
	while (fail < 2) {
		size_t old_sz = tmp.t.size();
		if (levo)
			tmp = tmp.faktor(tmp.slabo_levo_inv_kvur());
		else
			tmp = tmp.faktor(tmp.slabo_desno_inv_kvur());
		
		if (tmp.t.size() == old_sz)
			fail++;
		else
			fail = 0;
		levo ^= 1;
	}
	return tmp;
}

nfa nfa::desna_naizmenicna_redukcija() const {
	bool levo = false;
	int fail = 0;
	nfa tmp = *this;
	while (fail < 2) {
		size_t old_sz = tmp.t.size();
		if (levo)
			tmp = tmp.faktor(tmp.slabo_levo_inv_kvur());
		else
			tmp = tmp.faktor(tmp.slabo_desno_inv_kvur());
		
		if (tmp.t.size() == old_sz)
			fail++;
		else
			fail = 0;
		levo ^= 1;
	}
	return tmp;
}

std::pair<relacija, bool> nfa::forward_simulacija(const nfa& b) const {
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = t % b.t;
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= ((db * r_old.T()) / da).T();
		}
		if (r == r_old)
			break;
	}

	if (s.podskup(b.s * r.T()))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::backward_simulacija(const nfa& b) const {
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = s % b.s;
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= da % (r_old * db);
		}
		if (r == r_old)
			break;
	}

	if (t.podskup(r * b.t))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::forward_bisimulacija(const nfa& b) const {
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = (t % b.t) & (t / b.t);
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= ((db * r_old.T()) / da).T();
			r &= (da * r_old) / db;
		}
		if (r == r_old)
			break;
	}

	if (s.podskup(b.s * r.T()) && b.s.podskup(s * r))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::backward_bisimulacija(const nfa& b) const {
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = (s % b.s) & (s / b.s);
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= da % (r_old * db);
			r &= (db % (r_old.T() * da)).T();
		}
		if (r == r_old)
			break;
	}

	if (t.podskup(r * b.t) && b.t.podskup(r.T() * t))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::forward_backward_bisimulacija(const nfa& b)
	const
{
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = (t % b.t) & (s / b.s);
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= ((db * r_old.T()) / da).T();
			r &= (db % (r_old.T() * da)).T();
		}
		if (r == r_old)
			break;
	}

	if (s.podskup(b.s * r.T()) && b.t.podskup(r.T() * t))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::backward_forward_bisimulacija(const nfa& b)
	const
{
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	relacija r = (s % b.s) & (t / b.t);
	while (1) {
		relacija r_old = r;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i];
			const auto& db = b.d[i];
			r &= (da * r_old) / db;
			r &= da % (r_old * db);
		}
		if (r == r_old)
			break;
	}

	if (t.podskup(r * b.t) && b.s.podskup(s * r))
		return {r, true};
	else
		return {r, false};
}

std::vector<std::pair<skup, skup>> nfa::tau_skup_par(const nfa& b) const {
	if (mp != b.mp)
		throw std::logic_error("incompatible automata");

	std::set<std::pair<skup, skup>> svi;
	std::vector<std::pair<skup, skup>> red;

	size_t kraj = 0;
	svi.insert({t, b.t});
	red.emplace_back(t, b.t);

	while (kraj != red.size()) {
		auto rp = red[kraj++];
		const auto& ta = rp.first;
		const auto& tb = rp.second;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i]; 
			const auto& db = b.d[i]; 
			skup dra = da * ta;
			skup drb = db * tb;
			if (svi.insert({dra, drb}).second)
				red.emplace_back(dra, drb);
		}
	}

	return red;
}

std::vector<std::pair<skup, skup>> nfa::sigma_skup_par(const nfa& b) const {
	std::set<std::pair<skup, skup>> svi;
	std::vector<std::pair<skup, skup>> red;

	size_t kraj = 0;
	svi.insert({s, b.s});
	red.emplace_back(s, b.s);

	while (kraj != red.size()) {
		auto rp = red[kraj++];
		const auto& sa = rp.first;
		const auto& sb = rp.second;
		for (size_t i=0; i<d.size(); i++) {
			const auto& da = d[i]; 
			const auto& db = b.d[i]; 
			skup dra = sa * da;
			skup drb = sb * db;
			if (svi.insert({dra, drb}).second)
				red.emplace_back(dra, drb);
		}
	}

	return red;
}

std::pair<relacija, bool> nfa::slaba_forward_simulacija(const nfa& b) const {
	auto ts = tau_skup_par(b);
	auto r = relacija(t.size(), b.t.size());
	r.negate();
	for (const auto& tu : ts)
		r &= tu.first % tu.second;
	if (s.podskup(b.s * r.T()))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::slaba_backward_simulacija(const nfa& b) const {
	auto ss = sigma_skup_par(b);
	auto r = relacija(t.size(), b.t.size());
	r.negate();
	for (const auto& su : ss)
		r &= su.first % su.second;
	if (t.podskup(r * b.t))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::slaba_forward_bisimulacija(const nfa& b) const
{
	auto ts = tau_skup_par(b);
	auto r = relacija(t.size(), b.t.size());
	r.negate();
	for (const auto& tu : ts)
		r &= (tu.first % tu.second) & (tu.first / tu.second);
	if (s.podskup(b.s * r.T()) && b.s.podskup(s * r))
		return {r, true};
	else
		return {r, false};
}

std::pair<relacija, bool> nfa::slaba_backward_bisimulacija(const nfa& b) const
{
	auto ss = sigma_skup_par(b);
	auto r = relacija(t.size(), b.t.size());
	r.negate();
	for (const auto& su : ss)
		r &= (su.first % su.second) & (su.first / su.second);
	if (t.podskup(r * b.t) && b.t.podskup(r.T() * t))
		return {r, true};
	else
		return {r, false};
}

nfa nfa::podskup_konstrukcija() const {
	size_t n = t.size();
	size_t N = size_t(1) << n;
	size_t m = d.size();
	std::vector<relacija> d2(m, relacija(N, N));
	skup s2(N), t2(N);
	for (size_t mask = 0; mask < N; mask++) {
		skup x(n);
		for (size_t i = 0; i < n; i++)
			x[i] = (mask >> i) & 1;
		if (x == s)
			s2[mask] = 1;
		t2[mask] = x * t;
		for (size_t j = 0; j < m; j++) {
			skup y = x * d[j];
			size_t mask_y = 0;
			for (size_t i = 0; i < n; i++)
				mask_y |= size_t(y[i]) << i;
			d2[j][mask][mask_y] = 1;
		}
	}

	return nfa(d2, alfabet(), s2, t2);
}

nfa nfa::nerodov_automat() const {
	auto ss = sigma_skup();
	size_t N = ss.size();
	size_t m = d.size();
	std::vector<relacija> d2(m, relacija(N, N));
	skup s2(N), t2(N);

	std::map<skup, size_t> f;
	for (size_t i = 0; i < N; i++)
		f[ss[i]] = i;

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < m; j++) {
			auto sd = ss[i] * d[j];
			d2[j][i][f[sd]] = 1;
		}
		t2[i] = ss[i] * t;
	}

	s2[0] = 1;

	return nfa(d2, alfabet(), s2, t2);
}

nfa nfa::reverzni_automat() const {
	nfa t = *this;
	std::swap(t.s, t.t);
	for (auto& dx : t.d)
		dx = dx.T();
	return t;
}

nfa nfa::reverzni_nerodov_automat() const {
	auto ts = tau_skup();
	size_t N = ts.size();
	size_t m = d.size();
	std::vector<relacija> d2(m, relacija(N, N));
	skup s2(N), t2(N);

	std::map<skup, size_t> f;
	for (size_t i = 0; i < N; i++)
		f[ts[i]] = i;

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < m; j++) {
			auto td = d[j] * ts[i];
			d2[j][i][f[td]] = 1;
		}
		t2[i] = ts[i] * s;
	}

	s2[0] = 1;

	return nfa(d2, alfabet(), s2, t2);
}

std::vector<skup> nfa::sigma_skup(const relacija& r) const {
	std::set<skup> svi;
	std::vector<skup> red;

	size_t kraj = 0;
	svi.insert(s * r);
	red.emplace_back(s * r);

	while (kraj != red.size()) {
		skup ru = red[kraj++];
		for (const auto& dx : d) {
			skup rux = ru * dx * r;
			if (svi.insert(rux).second)
				red.emplace_back(std::move(rux));
		}
	}

	return red;
}

std::vector<skup> nfa::tau_skup(const relacija& r) const {
	std::set<skup> svi;
	std::vector<skup> red;

	size_t kraj = 0;
	svi.insert(r * t);
	red.emplace_back(r * t);

	while (kraj != red.size()) {
		skup su = red[kraj++];
		for (const auto& dx : d) {
			skup sxu = r * dx * su;
			if (svi.insert(sxu).second)
				red.emplace_back(std::move(sxu));
		}
	}

	return red;
}

nfa nfa::determinizacija_sdiku(const relacija& r) const {
	auto ss = sigma_skup(r);
	size_t N = ss.size();
	size_t m = d.size();
	std::vector<relacija> d2(m, relacija(N, N));
	skup s2(N), t2(N);

	std::map<skup, size_t> f;
	for (size_t i = 0; i < N; i++)
		f[ss[i]] = i;

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < m; j++) {
			auto sd = ss[i] * d[j] * r;
			d2[j][i][f[sd]] = 1;
		}
		t2[i] = ss[i] * t;
	}

	s2[0] = 1;

	return nfa(d2, alfabet(), s2, t2);
}

nfa nfa::determinizacija_sliku(const relacija& r) const {
	auto ts = tau_skup(r);
	size_t N = ts.size();
	size_t m = d.size();
	std::vector<relacija> d2(m, relacija(N, N));
	skup s2(N), t2(N);

	std::map<skup, size_t> f;
	for (size_t i = 0; i < N; i++)
		f[ts[i]] = i;

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < m; j++) {
			auto td = r * d[j] * ts[i];
			d2[j][i][f[td]] = 1;
		}
		t2[i] = ts[i] * s;
	}

	s2[0] = 1;

	return nfa(d2, alfabet(), s2, t2);
}

}