// all directed edges have to be reversed
std::vector<int> assign_grundy(std::vector<std::vector<int>>& dg) {
	int gsize = dg.size();
	std::vector<int> incomings(gsize);
	for(int i = 0; i < gsize; i++) {
		for(int j: dg[i]) {
			incomings[j]++;
		}
	}
	std::vector<int> vs_ready;
	for(int i = 0; i < gsize; i++) {
		if (incomings[i] == 0) vs_ready.push_back(i);
	}
	std::vector<std::set<int>> prohibited_grundy(gsize);
	std::vector<int> grundy(gsize);
	while(!vs_ready.empty()) {
		int i = vs_ready.back();
		vs_ready.pop_back();
		int g = 0;
		for(int pg: prohibited_grundy[i]) {
			if (g == pg) g++;
			else break;
		}
		grundy[i] = g;
		for(int j: dg[i]) {
			prohibited_grundy[j].insert(g);
			if ((--incomings[j]) == 0) vs_ready.push_back(j);
		}
	}
	return grundy;
}

