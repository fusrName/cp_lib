std::vector<int> z_algorithm(std::string s) {
	int n = s.size();
	std::vector<int> z(n);
	z[0] = n;
	int l = 0, r = 0;
	for(int i = 1; i < n; i++) {
		if (r < i) {
			l = i; r = i;
			for(int j = 0; r < n; j++, r++) {
				if (s[j] != s[r]) break;
			}
			z[i] = r - l;
		} else {
			int p = i - l, q = r - i;
			if (z[p] != q) {
				z[i] = std::min(z[p], q);
			} else {
				l = i;
				for(int j = q; r < n; j++, r++) {
					if (s[j] != s[r]) break;
				}
				z[i] = r - l;
			}
		}
	}
	return z;
}
