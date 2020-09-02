struct SegTree {
	std::vector<int> d;
	int sz;
	int base;
	const int e = 0;
	int merge(const int& i, const int& j){return i+j;}
	SegTree(int size): sz(size) {
		int k = 1;
		while(sz > k) k <<= 1;
		d = std::vector<int>(2 * k - 1, e);
		base = k - 1;
	}
	void update_upper(int i) {
		while(i > 0) {
			i = (i-1)/2;
			d[i] = merge(d[i*2+1], d[i*2+2]);
		}
	}
	void add(int i, int x) {
		assert(0 <= i && i < sz);
		int t = base + i;
		d[t] += x;
		update_upper(t);
	}
	void set(int i, int x) {
		assert(0 <= i && i < sz);
		int t = base + i;
		d[t] = x;
		update_upper(t);
	}
	int sum(int a, int b) {
		assert(0 <= a && a <= b && b <= sz);
		// sum up data in [a, b)
		return sum_sub(a, b, 0, 0, base+1);
	}
	int sum(int b) {return sum(0, b);}
	int sum_sub(int a, int b, int k, int l, int r) {
		if (b <= l || r <= a) return e;
		if (a <= l && r <= b) return d[k];
		int c = (l + r) / 2;
		return merge(
			sum_sub(a, b, 2*k+1, l, c),
			sum_sub(a, b, 2*k+2, c, r)
		);
	}
	int operator[](int i) {return d[base+i];}
};