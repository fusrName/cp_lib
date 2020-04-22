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
