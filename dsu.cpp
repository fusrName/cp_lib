// disjoint-set data structure represented on a single array
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

		DSU& operator=(DSU&& rval) {
			auto t = rval.data;
			rval.data = data;
			data = t;
			return *this;
		}
		
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
		// returns 0 if union not performed
		// note that 'union' is reserved word
		int merge(int i, int j) {
			int ri = find(i), rj = find(j);
			if (ri == rj)
				return 0;
			if (data[ri] > data[rj]) {
				int t = ri;
				ri = rj;
				rj = t;
			}
			data[ri] += data[rj];
			data[rj] = ri;
			return 1;
		}

		int get_size(int i) {
			return -data[find(i)];
		}
};
