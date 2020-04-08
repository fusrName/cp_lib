#define MOD 1000000007
// #define MOD 998244353

class mint {
	public:
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

		mint inv() {
			return pow(MOD - 2);
		}

		template<class T>
			mint pow(T p) {
				long long r = 1;
				long long t = i;
				for(; p > 0; p >>= 1) {
					if (p & 1) r = r * t % MOD;
					t = t * t % MOD;
				}
				return r;
			}

		template<class T1, class T2>
			static mint pow(T1 a, T2 b) {
				long long r = 1;
				long long t = (long long)(a % MOD);
				for(; b > 0; b >>= 1) {
					if (b & 1) r = r * t % MOD;
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
