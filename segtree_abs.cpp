template<class T>
struct SegTree_A {
	std::vector<T> d;
	int sz;
	int base;
	const T e;
	T merge(const T& i, const T& j){return i+j;}
	SegTree_A(int size): sz(size), e() {
		int k = 1;
		while(sz > k) k <<= 1;
		d = std::vector<T>(2 * k - 1, e);
		base = k - 1;
	}
	void update_upper(int i) {
		while(i > 0) {
			i = (i-1)/2;
			d[i] = merge(d[i*2+1], d[i*2+2]);
		}
	}
	void add(int i, T x) {
		assert(0 <= i && i < sz);
		int t = base + i;
		d[t] = d[t] + x;
		update_upper(t);
	}
	void assign(int i, T x) {
		assert(0 <= i && i < sz);
		int t = base + i;
		d[t] = x;
		update_upper(t);
	}
	T sum(int a, int b) {
		assert(0 <= a && a <= b && b <= sz);
		// sum up data in [a, b)
		return sum_sub(a, b, 0, 0, base+1);
	}
	T sum(int b) {return sum(0, b);}
	T sum_sub(int a, int b, int k, int l, int r) {
		if (b <= l || r <= a) return e;
		if (a <= l && r <= b) return d[k];
		int c = (l + r) / 2;
		return merge(
			sum_sub(a, b, 2*k+1, l, c),
			sum_sub(a, b, 2*k+2, c, r)
		);
	}
	T get(int i) {return d[base+i];}
};
