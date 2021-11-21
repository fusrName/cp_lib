#include <bits/stdc++.h>
using namespace std;

template <class T>
void write_out(const T& v) { cout << v; }

template <class T, class... Args>
void write_out(const T& v, const int& n, const Args&... args) {
  cout << '{';
  for (int i = 0; i < n; i++) {
    write_out(v[i], args...);
    cout << ",}"[i + 1 == n];
  }
}

template<char sep=' ', char end='\n', class T>
void OUT(const vector<T>& d) {
  int n = d.size();
  for(int i = 0; i < n; i++) cout << d[i] << (i + 1 == n ? end : sep);
}

template<class T>
void OUT(const vector<vector<T>>& d) {
  for(auto& di: d) OUT(di);
}

int main() {
  int d[][5][5] = {{{3, 4, 5}, {5, 2}}};
  write_out(d, 1, 5, 5);
  cout << '\n';
  vector<vector<int>> a{{3, 4, 5}, {1, 7, 1, 3, 5, 1}};
  OUT(a);
}