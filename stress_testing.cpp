#include<bits/stdc++.h>
namespace {
#pragma GCC diagnostic ignored "-Wunused-function"
#include<atcoder/all>
#pragma GCC diagnostic warning "-Wunused-function"
using namespace std;
using namespace atcoder;
#define rep(i,n) for(int i = 0; i < (int)(n); i++)
#define rrep(i,n) for(int i = (int)(n) - 1; i >= 0; i--)
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
template<class T> bool chmax(T& a, const T& b) { if (a < b) { a = b; return true; } else return false; }
template<class T> bool chmin(T& a, const T& b) { if (b < a) { a = b; return true; } else return false; }
using ll = long long;
using P = pair<int,int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<ll>;
using VVL = vector<VL>;

#pragma GCC diagnostic ignored "-Wunused-function"
// input
std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
using dist_type = std::uniform_int_distribution<>;
using param_type = dist_type::param_type;

int RI(int L, int R) { assert(L < R); return dist_type(L, R - 1)(gen); }
ll RL(ll L, ll R) { assert(L < R); return std::uniform_int_distribution<ll>(L, R - 1)(gen); }
VI RIS(int n, int L, int R) {
  assert(L < R);
  VI a(n);
  rep(i, n) a[i] = RI(L, R);
  return a;
}
VL RLS(int n, ll L, ll R) {
  assert(L < R);
  VL a(n);
  rep(i, n) a[i] = RL(L, R);
  return a;
}
string RS(int n, const char* char_set = nullptr) {
  string s(n, '.');
  if (char_set) {
    int sz = strlen(char_set);
    rep(i, n) s[i] = char_set[RI(0, sz)];
  } else {
    rep(i, n) s[i] = 'a' + RI(0, 26);
  }
  return s;
}

// output
template<class T, class = void>
struct is_outputable : false_type {};
template<class T>
struct is_outputable<T, void_t<decltype(declval<ostream&>() << declval<T>())>> : true_type {};

template<class T, class = void>
struct is_iterable : false_type {};
template<class T>
struct is_iterable<T, void_t<decltype(begin(declval<T>()) != end(declval<T>()))>> : true_type {};

template<class T>
void print_value(const T& v) {
#ifdef ATCODER_MODINT_HPP
  if constexpr (atcoder::internal::is_modint<T>::value) {
    cout << v.val();
  } else
#endif
  if constexpr (is_outputable<const T&>::value) {
    cout << v;
  } else if constexpr (is_iterable<const T&>::value) {
    bool first = true;
    for(auto&& x: v) {
      if (first) first = false;
      else cout << ' ';
      print_value(x);
    }
  } else {
    static_assert(!is_same_v<T, T>, "cannot print");
  }
}

void print() { cout << endl; }
template <class T, class... Args>
void print(const T& v, const Args&... args) {
  print_value(v);
  if constexpr (sizeof...(args) >= 1) cout << ' ';
  print(args...);
}

#pragma GCC diagnostic warning "-Wunused-function"


auto solve() {
  
}

auto solve_naive() {
  
}


} int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  // { int s; cin >> s; gen.seed(s); }
  int TCNT = 2;
  // cin >> TCNT;
  rep(TC, TCNT) {
    auto res = solve();
    auto res_naive = solve_naive();
    // result check
    bool succeeded = res == res_naive;
    if (!succeeded) {
      print(res);
      print(res_naive);
      exit(0);
    }
  }
  cout << "All tests passed.\n";
}
