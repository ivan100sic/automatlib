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

	nfa(const std::vector<relacija>& d, const std::string& l,
		const skup& s, const skup& t);

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

	// nfa faktor(const relacija& r) const;

};

}