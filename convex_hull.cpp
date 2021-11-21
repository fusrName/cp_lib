vector<int> convex_hull(const vector<Vec>& ps) {
  const int n = ps.size();
  if (n <= 1) {
    if (n == 0) return {};
    else return {0};
  }
  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) {
    return ps[i].y < ps[j].y || (ps[i].y == ps[j].y && ps[i].x < ps[j].x);
  });
  vector<int> st1, st2;
  for(int i: ord) {
    int sz = st1.size();
    while(sz >= 2 && (ps[st1[sz - 1]] - ps[st1[sz - 2]]) * (ps[i] - ps[st1[sz - 1]]) <= 0) {
      st1.pop_back();
      sz--;
    }
    st1.push_back(i);
  }
  for(int i: ord) {
    int sz = st2.size();
    while(sz >= 2 && (ps[st2[sz - 1]] - ps[st2[sz - 2]]) * (ps[i] - ps[st2[sz - 1]]) >= 0) {
      st2.pop_back();
      sz--;
    }
    st2.push_back(i);
  }
  assert(st1.front() == st2.front() && st1.back() == st2.back());
  st1.insert(st1.end(), st2.rbegin() + 1, st2.rend() - 1);
  return st1;
}