std::vector<std::vector<int>> iig(int vn, int en) {
	std::vector<std::vector<int>> res(vn);
        for(int i = 0; i < en; i++) {
                int s, t;
                std::cin >> s >> t;
                s--; t--;
                res[s].push_back(t);
                res[t].push_back(s);
        }
        return res;
}

void sep_pc(const std::vector<std::vector<int>>& g, std::vector<int>& p, std::vector<std::vector<int>>& c, const int root=0) {
	int n = g.size();
	std::vector<int> pt(n);
	std::vector<std::vector<int>> ct(n);
	std::function<void(int, int)> f = [&](int x, int parent) {
		pt[x] = parent;
		for(int y: g[x]) {
			if (y == parent) continue;
			ct[x].push_back(y);
			f(y, x);
		}
		return;
	};
	f(root, -1);
	p = std::move(pt);
	c = std::move(ct);
	return;
}

void itree(int n, std::vector<int>& p, std::vector<std::vector<int>>& c, const int root=0) {
	sep_pc(iig(n, n - 1), p, c, root);
}

// return vector of size of subtrees
std::vector<int> get_tsize(std::vector<std::vector<int>> &children, int root=0) {
	std::vector<int> res(children.size());
	std::function<int(int)> f = [&](int x) {
		int a = 1;
		for(int y: children[x]) {
			a += f(y);
		}
		res[x] = a;
		return a;
	};
	f(root);
	return res;
}

