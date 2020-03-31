#define MOD 1000000007
// #define MOD 998244353

struct mint {
	int i;

	mint() : i(0) {}

	template<class T>
	mint(T x) {
		i = int(x % MOD);
		if (i < 0) i += MOD;
	}

	mint operator+(mint x) {
		return i + x.i;
	}

	mint operator-(mint x) {
		return i - x.i;
	}

	mint operator*(mint x) {
		return (long long)i * x.i;
	}

	mint operator/(mint x) {
		return (long long)i * x.pow(MOD - 2).i;
	}

	template<class T>
	mint pow(T p) {
		int r = 1;
		int t = i;
		for(; p > 0; p >>= 1) {
			if (p & 1) r = r * t % MOD;
			t = t * t % MOD;
		}
		return r;
	}

	mint& operator+=(mint x) {
		i = (i + x.i) % MOD;
		return *this;
	}

	mint& operator-=(mint x) {
		i = i - x.i;
		if (i < 0) i += MOD;
		return *this;
	}

	mint& operator*=(mint x) {
		i = (int)((long long)i * x.i % MOD);
		return *this;
	}

	mint& operator/=(mint x) {
		i = (long long)i * x.pow(MOD - 2).i % MOD;
		return *this;
	}
};

std::ostream& operator<<(std::ostream& os, const mint& m) {
	return os << m.i;
}
