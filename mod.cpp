#define MOD 1000000007

// avoid using this function unless you are certain of the necessity of this process
// so as not to overlook errors in code accidentally
inline int to_mod(long long n) {
	return n >= 0 ? n % MOD : MOD + n % MOD;
}

// all the arguments for the functions below are assumed between 0 and 1000000006
inline int madd(int a, int b) {
	return (a + b) % MOD;
}

inline int msub(int a, int b) {
	int t = a - b;
	return t >= 0 ? t : MOD + t;
}

inline int mmul(int a, int b) {
	return (int)((long long)a * b % MOD);
}

inline int mpow(int a, int b) {
	int r = 1;
	for(; b != 0; b >>= 1) {
		if (b & 1) r = mmul(r, a);
		a = mmul(a, a);
	}
	return r;
}

inline int minv(int a) {
	return mpow(a, MOD - 2);
}

inline int mdiv(int a, int b) {
	return mmul(a, minv(b));
}


// combinatorics
inline int mfact(int n) {
	int r = 1;
	for(; n > 0; n--) {
		r = mmul(r, n);
	}
	return r;
}

inline int mperm(int n, int m) {
	int r = 1;
	for(; m > 0; m--) {
		r = mmul(r, m);
		n--;
	}
	return r;
}

inline int mcomb(int n, int m) {
	int rn = 1, rm = 1;
	while(m > 0) {
		rn = mmul(rn, n);
		rm = mmul(rm, m);
		n--;
		m--;
	}
	return mdiv(rn, rm);
}
