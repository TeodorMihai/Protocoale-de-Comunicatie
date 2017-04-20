#include <bits/stdc++.h>

using namespace std;

multimap<int, int> mul;

int main() {

	fun();
	//exit(0);
	more();

	mul.insert({5, 2});
	mul.insert({5, 3});
	const auto eq = mul.equal_range(5);
	for(auto x = eq.first ; x != eq.second; ++x)
		cout << x->first << ' ' << x->second << '\n';
	return 0;
}