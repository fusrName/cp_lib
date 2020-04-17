template<class TI, class TF>
TI bin_search(TI l, TI u, TF f) {
	while(u - l > 1) {
		TI c = (l + u) / 2;
		if (f(c)) u = c;
		else l = c;
	}
	assert(l + 1 == u);
	return u;
}

bool is_prime(int n) {
	if (n < 2) return false;
	int u = bin_search(1, n, [n](int i) {return i * i > n;});
	for(int i = 2; i < u; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}

struct primes_ {
	vector<int> v;
	primes_(int n) {
		for(int i = 2; i <= n; i++) {
			if (is_prime(i)) {
				v.push_back(i);
			}
		}
	}
	auto begin() {return v.begin();}
	auto end() {return v.end();}
} primes(100000);

struct divs_ {
	vector<vector<int>> d;
	divs_(int n): d(n+1) {
		for(int i = 1; i <= n; i++) {
			for(int j = i; j <= n; j += i) {
				d[j].push_back(i);
			}
		}
	}
	vector<int>& operator[](int n) {
		return d[n];
	}
} divs(100000);

