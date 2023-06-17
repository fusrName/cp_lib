pair<vector<int>, vector<int>> primes_lpf(const int n) {
  vector<int> primes; primes.reserve(n / 10);
  vector<int> lpf(n + 1);
  for (int i = 2; i <= n; i += 2) lpf[i] = 2;
  for (int i = 3; i <= n; i += 6) lpf[i] = 3;
  if (2 <= n) primes.push_back(2);
  if (3 <= n) primes.push_back(3);
  // 5 * x <= n, x <= floor(n / 5)
  const int n5 = n / 5;
  int x = 5;
  char add_next = 2;
  for (; x <= n5; x += add_next, add_next ^= 2 ^ 4) {
    int px = lpf[x];
    if (px == 0) {
      lpf[x] = px = x;
      primes.push_back(x);
    }
    for (int i = 2;; ++i) {
      int q = primes[i];
      int y = q * x;
      if (y > n) break;
      lpf[y] = q;
      if (q == px) break;
    }
  }
  for (; x <= n; x += add_next, add_next ^= 2 ^ 4) {
    if (lpf[x] == 0) {
      lpf[x] = x;
      primes.push_back(x);
    }
  }
  return {move(primes), move(lpf)};
}

constexpr int PSIZE = 1000000;
auto [primes, lpf] = primes_lpf(PSIZE);

vector<pair<int, int>>& factorize_lv(int x) {
  int ps[10], cs[10];
  int sz = 0;
  while (x != 1) {
    int p = lpf[x], c = 0;
    do {x /= p; c++;} while (x % p == 0);
    ps[sz] = p; cs[sz] = c; sz++;
  }
  static vector<pair<int, int>> fs;
  fs.clear();
  for (int i = 0; i < sz; i++) fs.emplace_back(ps[i], cs[i]);
  return fs;
}

vector<pair<int, int>> factorize(int x) {
  int ps[10], cs[10];
  int sz = 0;
  while (x != 1) {
    int p = lpf[x], c = 0;
    do {x /= p; c++;} while (x % p == 0);
    ps[sz] = p; cs[sz] = c; sz++;
  }
  vector<pair<int, int>> fs; fs.reserve(sz);
  for (int i = 0; i < sz; i++) fs.emplace_back(ps[i], cs[i]);
  return fs;
}


vector<int>& divisors_lv(int x) {
  int ps[10], cs[10];
  int sz = 0;
  while (x != 1) {
    int p = lpf[x], c = 0;
    do {x /= p; c++;} while (x % p == 0);
    ps[sz] = p; cs[sz] = c; sz++;
  }
  static vector<int> res;
  res.clear();
  res.emplace_back(1);
  for (int i = 0; i < sz; i++) {
    int p = ps[i], ptr = 0;
    for (int t = cs[i] * res.size(); t > 0; t--)
      res.emplace_back(res[ptr++] * p);
  }
  return res;
}

vector<int> divisors(int x) {
  int ps[10], cs[10];
  int sz = 0, dcnt = 1;
  while (x != 1) {
    int p = lpf[x], c = 0;
    do {x /= p; c++;} while (x % p == 0);
    ps[sz] = p; cs[sz] = c; sz++;
    dcnt *= c + 1;
  }
  vector<int> res; res.reserve(dcnt);
  res.emplace_back(1);
  for (int i = 0; i < sz; i++) {
    int p = ps[i], ptr = 0;
    for (int t = cs[i] * res.size(); t > 0; t--)
      res.emplace_back(res[ptr++] * p);
  }
  return res;
}

vector<int> divisors_sorted(int x) {
  int ps[10], cs[10];
  int sz = 0, dcnt = 1;
  while (x != 1) {
    int p = lpf[x], c = 0;
    do {x /= p; c++;} while (x % p == 0);
    ps[sz] = p; cs[sz] = c; sz++;
    dcnt *= c + 1;
  }

  int n = dcnt;
  vector<int> divs(n);
  vector<unsigned char> rest(n);
  divs[n - 1] = 1;
  int sz_cur = 1;
  // ps must be increasing
  for (int i = 0; i < sz; i++) {
    int p = ps[i], c = cs[i];
    int ptr1 = n - sz_cur;
    sz_cur *= c + 1;
    int ptr2 = n - sz_cur, ptrW = ptr2;
    // process once
    divs[ptrW] = divs[ptr1];
    rest[ptrW] = c;
    ptrW++, ptr1++;
    if (i) [&] {
      while (true) {
        // ptr2 < ptrW holds
        while (!rest[ptr2]) ptr2++;
        int v = divs[ptr2] * p;
        while (divs[ptr1] < v) {
          divs[ptrW] = divs[ptr1];
          rest[ptrW] = c;
          ptrW++, ptr1++;
          if (ptr1 == n) return;
        }
        divs[ptrW] = v;
        rest[ptrW] = rest[ptr2] - 1;
        ptrW++, ptr2++;
      }
    }();
    while (ptrW != n) {
      while (!rest[ptr2]) ptr2++;
      divs[ptrW] = divs[ptr2] * p;
      rest[ptrW] = rest[ptr2] - 1;
      ptrW++, ptr2++;
    }
  }
  return divs;
}
