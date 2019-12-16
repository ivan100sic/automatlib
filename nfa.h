#pragma once
#include <vector>
#include <map>
#include "relacija.h"
#include "skup.h"
#include "sr-cross.h"

namespace automati {

class nfa {
	std::vector<relacija> d;
	skup s, t;
	std::map<char, size_t> mp;

public:

	const std::vector<relacija>& prelazi() const { return d; }

	const skup& sigma() const { return s; }

	const skup& tau() const { return t; }

	std::string alfabet() const;

	nfa(const std::vector<relacija>& d, const std::string& l,
		const skup& s, const skup& t);
	
	operator std::string () const;

	skup pokreni(const std::string& s) const;

	bool raspoznaje(const std::string& s) const;

	relacija desno_inv_kvur() const;

	relacija levo_inv_kvur() const;

	relacija desno_inv_ekv() const;
	
	relacija levo_inv_ekv() const;

	std::vector<skup> tau_skup() const;

	std::vector<skup> sigma_skup() const;

	relacija slabo_desno_inv_kvur() const;

	relacija slabo_levo_inv_kvur() const;

	relacija slabo_desno_inv_ekv() const;

	relacija slabo_levo_inv_ekv() const;

	nfa faktor(const relacija& r) const;

	nfa leva_naizmenicna_redukcija() const;

	nfa desna_naizmenicna_redukcija() const;

	std::pair<relacija, bool> forward_simulacija(const nfa& b) const;

	std::pair<relacija, bool> backward_simulacija(const nfa& b) const;

	std::pair<relacija, bool> forward_bisimulacija(const nfa& b) const;

	std::pair<relacija, bool> backward_bisimulacija(const nfa& b) const;

	std::pair<relacija, bool> forward_backward_bisimulacija(const nfa& b)
		const;

	std::pair<relacija, bool> backward_forward_bisimulacija(const nfa& b)
		const;

	std::vector<std::pair<skup, skup>> tau_skup_par(const nfa& b) const;

	std::vector<std::pair<skup, skup>> sigma_skup_par(const nfa& b) const;

	std::pair<relacija, bool> slaba_forward_simulacija(const nfa& b)
		const;

	std::pair<relacija, bool> slaba_backward_simulacija(const nfa& b)
		const;

	std::pair<relacija, bool> slaba_forward_bisimulacija(const nfa& b)
		const;

	std::pair<relacija, bool> slaba_backward_bisimulacija(const nfa& b)
		const;

	nfa podskup_konstrukcija() const;

	nfa nerodov_automat() const;

	nfa reverzni_automat() const;

	nfa reverzni_nerodov_automat() const;

	std::vector<skup> sigma_skup(const relacija& r) const;

	std::vector<skup> tau_skup(const relacija& r) const;

	nfa determinizacija_sdiku(const relacija& r) const;

	nfa determinizacija_sliku(const relacija& r) const;

	nfa deciji_automat(const relacija& r) const;

};

}