struct primes_ {
	std::vector<int> d;
	primes_(int n): d(n+2) {
		std::vector<bool> s(n+1);
		d[0] = 2; d[1] = 3;
		int sz = 2;
		bool f = false;
		for(int p = 5; p <= n; p += (f = !f) ? 2 : 4) {
			if (!s[p]) {
				d[sz++] = p;
				if ((long long)p * p <= n) {
					for(int x=p*p; x<=n; x+=p) s[x] = true;
				}
			}
		}
		d.resize(sz);
	}
	auto begin() {return d.begin();}
	auto end() {return d.end();}
} primes(1000000);
