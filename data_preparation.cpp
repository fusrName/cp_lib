template<class TI, class TF>
TI bin_search(TI l, TI u, TF f) {
	while(u - l > 1) {
		TI c = (l + u) / 2;
		if (f(c)) u = c;
		//if (!f(c)) u = c;
		else l = c;
	}
	assert(l + 1 == u);
	return u;
	//return l;
}

template<class T>
bool is_prime(T n) {
	if (n < 2) return false;
	int u = bin_search((T)1, n, [n](T i) {return n / i / i == 0;});
	for(int i = 2; i < u; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}

struct primes_ {
	vector<int> v;
	primes_(int n): v{2, 3} {
		if (n < 3) {
			if (n == 2) v = vector<int>(1, 2);
			else v.clear();
			return;
		}
		bool f = false;
		int end = 1;
		for(int i = 5; i <= n; i += (f = !f) ? 2 : 4) {
			while(v[end] * v[end] <= i) end++;
			bool isprime = true;
			for(int k = 0; k < end; k++) {
				if (i % v[k] == 0) {
					isprime = false;
					break;
				}
			}
			if (isprime) v.push_back(i);
		}
	}
	auto begin() {return v.begin();}
	auto end() {return v.end();}
} primes(1000000);

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
