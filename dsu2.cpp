// disjoint-set data structure is represented on a single array
// each element points to parent element in the same set and forms tree,
// whose root indicates tree size multipled by -1

class DSU {
	private:
		int *data;
	public:
		DSU(int n) {
			data = new int[n];
			for(int i = 0; i < n; i++) {
				data[i] = -1;
			}
		}

		~DSU() {delete[] data;}

		// path compression without recursion
		int find(int i) {
			int parent;
			int back_to = -1;
			for(; (parent = data[i]) >= 0; i = parent) {
				data[i] = back_to;
				back_to = i;
			}
			int root = i;
			while(back_to != -1) {
				i = back_to;
				back_to = data[i];
				data[i] = root;
			}
			return root;
		}

		// union by size
		// 'union' is reserved word
		void merge(int i, int j) {
			int ri = find(i), rj = find(j);
			if (ri == rj)
				return;
			if (data[ri] > data[rj]) {
				int t = ri;
				ri = rj;
				rj = t;
			}
			data[ri] = data[ri] + data[rj];
			data[rj] = ri;
		}

		int get_size(int i) {
			return -data[find(i)];
		}
};
