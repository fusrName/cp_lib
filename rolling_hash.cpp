#include<random>
#include<chrono>

class mint61 {
  using ull = unsigned long long;
  using ui128 = __uint128_t;
 public:
  static constexpr unsigned long long mod = (1ULL << 61) - 1;
  ull v = 0;
  explicit constexpr mint61() {}
  explicit constexpr mint61(ull x) {
    v = (x >> 61) + (x & mod);
    if (v >= mod) v -= mod;
  }
  static constexpr mint61 raw(ull x) {
    mint61 res;
    res.v = x;
    return res;
  }
  friend constexpr mint61 operator+(mint61 lhs, mint61 rhs) {
    auto res = lhs.v + rhs.v;
    return raw(res < mod ? res : res - mod);
  }
  friend constexpr mint61 operator-(mint61 lhs, mint61 rhs) {
    return raw(lhs.v >= rhs.v ? lhs.v - rhs.v : mod + lhs.v - rhs.v);
  }
  static constexpr ull multiply_loose_mod(mint61 lhs, mint61 rhs) {
    // ab = q(m+1)+r = qm+q+r
    // q+r = ab-qm = ab-floor(ab/(m+1))m < ab-(ab/(m+1)-1)m = ab/(m+1)+m <= (m-1)^2/(m+1)+m = 2m-1-2/(m+1)
    auto mul = ui128(lhs.v) * rhs.v;
    return ull(mul >> 61) + ull(mul & mod);
  }
  friend constexpr mint61 operator*(mint61 lhs, mint61 rhs) {
    auto res = multiply_loose_mod(lhs, rhs);
    return raw(res < mod ? res : res - mod);
  }
  mint61& operator+=(mint61 rhs) { return *this = *this + rhs; }
  mint61& operator-=(mint61 rhs) { return *this = *this - rhs; }
  mint61& operator*=(mint61 rhs) { return *this = *this * rhs; }
  friend ostream& operator<<(ostream& os, mint61 x) { return os << x.v; }
};

namespace rh_statics {
auto RNG = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
auto Distribution = std::uniform_int_distribution<unsigned long long>(1, mint61::mod - 1);
template <int id> const mint61 B = mint61::raw(Distribution(RNG));
template <int id> vector<mint61> powB;
template <int id> void extend_powB(int n) {
  int sz = powB<id>.size();
  if (sz >= n) return;
  powB<id>.resize(n);
  if (sz == 0) powB<id>[sz++] = {mint61::raw(1)};
  for (; sz < n; sz++) powB<id>[sz] = powB<id>[sz - 1] * B<id>;
}
template <int id, class T> static mint61 hash(const T& s) {
  mint61 res;
  for (auto a : s) res = mint61(mint61::multiply_loose_mod(res, B<id>) + a);
  return res;
}
}
template <int id>
struct hashbox {
  mint61 hash;
  int len = 0;
  hashbox() {}
  hashbox(mint61 x, int len=1) : hash(x), len(len) {}
  template <class T>
  hashbox(const T& s) : hash(rh_statics::hash(s)), len(size(s)) {}
  friend hashbox operator+(const hashbox& lhs, const hashbox& rhs) {
    rh_statics::extend_powB<id>(rhs.len);
    return {mint61(mint61::multiply_loose_mod(lhs.hash, rh_statics::powB<id>[rhs.len]) + rhs.hash.v), lhs.len + rhs.len};
  }
  friend hashbox operator+(unsigned long long lhs, const hashbox& rhs) {
    return hashbox{mint61(lhs), 1} + rhs;
  }
  friend hashbox operator+(const hashbox& lhs, unsigned long long rhs) {
    return lhs + hashbox{mint61(rhs), 1};
  }
  hashbox& operator+=(const hashbox& rhs) { return *this = *this + rhs; }
  hashbox& operator+=(unsigned long long rhs) { return *this = *this + rhs; }
};
template <int id> class rolling_hash {
  using ull = unsigned long long;
 public:
  int n;
  vector<mint61> hash_table;
  template <class T> explicit rolling_hash(const T& s): n(size(s)), hash_table(build_hash_table(s)) {
    rh_statics::extend_powB<id>(n + 1);
  }
  template <class T> vector<mint61> build_hash_table(const T& s) {
    vector<mint61> hash_table;
    hash_table.reserve(size(s) + 1);
    hash_table.emplace_back(0);
    mint61 cur_hash;
    for (auto a : s) {
      cur_hash = mint61(mint61::multiply_loose_mod(cur_hash, rh_statics::B<id>) + a);
      hash_table.emplace_back(cur_hash);
    }
    return hash_table;
  }
  mint61 get(int i, int j) {
    assert(0 <= i && i <= j && j <= n);
    return mint61(hash_table[j].v + 2 * mint61::mod - mint61::multiply_loose_mod(hash_table[i], rh_statics::powB<id>[j - i]));
  }
  hashbox<id> get_boxed(int i, int j) { return hashbox<id>{get(i, j), j - i}; }
};

// struct multihashbox {
//   // TODO
// };