std::vector<int> compute_grundy(std::vector<std::vector<int>>& to) {
	int sz = to.size();
	std::vector<int> vsorted(sz);
	std::vector<int> indeg(sz);
	for(int i = 0; i < sz; i++) for(int j: to[i]) indeg[j]++;
	std::vector<int> avail;
	for(int i = 0; i < sz; i++) if (indeg[i] == 0) avail.push_back(i);
	int nxt_idx = 0;
	while(!avail.empty()) {
		int u = avail.back(); avail.pop_back();
		vsorted[nxt_idx++] = u;
		for(int v: to[u]) if (--indeg[v] == 0) avail.push_back(v);
	}
	std::vector<int> grundy(sz), used(sz, -1);
	for(int i = sz - 1; i >= 0; i--) {
		int u = vsorted[i];
		for(int v: to[u]) used[grundy[v]] = u;
		int g = 0;
		while(used[g] == u) g++;
		grundy[u] = g;
	}
	return grundy;
}
