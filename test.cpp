#include <bits/stdc++.h>
#include "automati.h"
using namespace std;
using namespace automati;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

// nesto random
void test1() {
	vector<relacija> d = {
		relacija("0010 0010 0000 0010"),
		relacija("0100 0001 0000 0001")
	};

	skup s("1000"), t("0101");

	cout << "radi " << ((s/t) == (t%s).T()) << '\n';

	nfa a(d, "xy", s, t);

	cout << a.raspoznaje("") << '\n';
	cout << a.raspoznaje("x") << '\n';
	cout << a.raspoznaje("y") << '\n';
	cout << a.raspoznaje("yy") << '\n';
	cout << a.raspoznaje("yyy") << '\n';
	cout << a.raspoznaje("xxyyxy") << '\n';
	cout << a.raspoznaje("yyxy") << '\n';
	cout << a.raspoznaje("xyx") << '\n';

	cout << (string)(d[1] * d[1]);
	cout << (d[1]*d[1]*d[1] == d[1]*d[1]) << '\n';
}

nfa pr436() {
	vector<relacija> d = {
		relacija("0100 0001 0001 0001"),
		relacija("0001 1010 1010 0001")
	};

	skup s("1000");
	skup t("0001");

	return nfa(d, "xy", s, t);
}

nfa pr438() {
	vector<relacija> d = {
		relacija("100 000 000"),
		relacija("100 110 100")
	};

	skup s("111");
	skup t("111");

	return nfa(d, "xy", s, t);
}

nfa pr4315() {
	vector<relacija> d = {
		relacija("010 010 000"),
		relacija("001 000 001")
	};

	skup s("100");
	skup t("101");

	return nfa(d, "xy", s, t);
}

nfa pr4319() {
	vector<relacija> d = {
		relacija("100 000 000"),
		relacija("010 111 100")
	};

	skup s("100");
	skup t("011");

	return nfa(d, "xy", s, t);
}

// primer 4.3.6 iz knjige
void test2() {
	cout << (string)pr436().desno_inv_kvur() << '\n';
}

// primer 4.3.7 iz knjige
void test3() {
	cout << (string)pr436().levo_inv_kvur() << '\n';
}

// primer 4.3.8 iz knjige
void test4() {
	cout << (string)pr438().desno_inv_kvur() << '\n';
	cout << (string)pr438().desno_inv_ekv() << '\n';
}

// primer 4.3.14 iz knjige
void test5() {
	auto a = pr436();
	for (auto te : a.tau_skup())
		cout << (string)te << '\n';
	cout << '\n';
	cout << (string)a.slabo_desno_inv_kvur() << '\n';
}

// primer 4.3.15 iz knjige
void test6() {
	auto a = pr4315();
	for (auto te : a.tau_skup())
		cout << (string)te << '\n';
	cout << '\n';
	cout << (string)a.slabo_desno_inv_kvur() << '\n';
	cout << (string)a.desno_inv_kvur() << '\n';
}

// moj gadan primer automata
void test7(size_t n) {
	relacija dx(n, n), dy(n, n);
	for (size_t i=0; i<n; i++)
		dy[i][i] = i > 1, dx[i][(i+1)%n] = 1;
	skup t(n);
	t.negate();
	skup s = t;

	nfa a({dx, dy}, "xy", s, t);
	cout << (string)a.slabo_desno_inv_kvur() << '\n';
}

void test8() {
	cout << (string)pr4319().slabo_desno_inv_kvur() << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	test8();
}