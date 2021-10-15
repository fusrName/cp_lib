
// T has to implement 'operator+' and 'add_parent' method
template<class T>
class RTDP {
	class Node {
		public:
		int k; // k_th node
		RTDP& nodes; // owner object holding all nodes
		const std::vector<int>& children; // children indices
		const int csize;
		std::vector<T> dp; // stores dps of children
		std::vector<T> lmerged;
		std::vector<T> rmerged;

		Node(RTDP& ns, int i) : k(i), nodes(ns), children(ns.v[i]), csize(children.size()), dp(csize), lmerged(csize + 1), rmerged(csize + 1) {}

		T fc(int parent = -1) {
			const int csize = children.size();
			T m;
			for(int i = 0; i < csize; i++) {
				int ci = children[i];
				if (ci == parent) continue;
				dp[i] = nodes.ns[ci].fc(k);
				m = m + dp[i];
			}
			return m.add_parent(k);
		}

		void fp(int parent = -1) {
			const int csize = children.size();
			// assume that dp is already filled in
			// left merge
			for(int i = 0; i < csize; i++) {
				lmerged[i + 1] = lmerged[i] + dp[i];
			}
			// right merge
			for(int i = csize; i > 0; i--) {
				rmerged[i - 1] = rmerged[i] + dp[i - 1];
			}
			for(int i = 0; i < csize; i++) {
				int  ci = children[i];
				if (ci == parent) continue;
				Node& cnode = nodes.ns[ci];
				for(int j = 0; j < cnode.csize; j++) {
					if (cnode.children[j] == k) {
						cnode.dp[j] = (lmerged[i] + rmerged[i + 1]).add_parent(k);
						break;
					}
				}
				cnode.fp(k);
			}
			return;
		}
	};

	std::vector<std::vector<int>>& v;
	int size;
	std::vector<Node> ns;
	std::vector<T> root_dp;

	public:
	RTDP(std::vector<std::vector<int>>& v) : v(v), size(v.size()), root_dp(size) {
		for(int i = 0; i < size; i++) {
			ns.push_back(Node(*this, i));
		}
	}

	void exec_dp(int root = 0) {
		fill_in_children(root);
		fill_in_parent(root);
	}

	void fill_in_children(int root) {
		ns[root].fc();
	}

	void fill_in_parent(int root) {
		ns[root].fp();
	}

	T get_root_dp(int i) {
		Node& r = ns[i];
		return r.lmerged[r.csize].add_parent(i);
	}

};
