template <class T, class Comp = less<T>>
struct erasable_priority_queue {
  priority_queue<T, vector<T>, Comp> q, q_erase;
  void push(T x) { q.push(move(x)); }
  template <class... Args>
  void emplace(Args&&... args) {
    q.emplace(forward<Args>(args)...);
  }
  void erase(T x) { q_erase.push(move(x)); }
  decltype(auto) top() {
    assert(!empty());
    expose_top();
    return q.top();
  }
  void pop() {
    assert(!empty());
    expose_top();
    q.pop();
  }
  int size() { return int(q.size()) - int(q_erase.size()); }
  bool empty() { return !size(); }
  void expose_top() {
    while (!q_erase.empty() && q_erase.top() == q.top()) {
      q_erase.pop();
      q.pop();
    }
  }
};

template <class T = long long, class PQMax = priority_queue<T>,
          class PQMin = priority_queue<T, vector<T>, greater<T>>>
struct SlopeTrick {
  static constexpr T INF = numeric_limits<T>::max() / 3;
  T min_f = 0;

  // acutual value of x in L is ADD_add_L(x) = x + add_L
  T add_L = 0, add_R = 0;
  PQMax L;
  PQMin R;

  T top_L() { return L.empty() ? -INF : L.top() + add_L; }
  T top_R() { return R.empty() ? +INF : R.top() + add_R; }
  void push_L(T x) { L.push(x - add_L); }
  void push_R(T x) { R.push(x - add_R); }
  T pushpop_L(T x) {
    T L_max = top_L();
    if (x >= L_max) return x;
    L.pop();
    push_L(x);
    return L_max;
  }
  T pushpop_R(T x) {
    T R_min = top_R();
    if (x <= R_min) return x;
    R.pop();
    push_R(x);
    return R_min;
  }

  // f(x) += c
  void add_constant(T c) { min_f += c; }
  // f(x) += max(x - a, 0)
  void add_right_slope(T a) {
    min_f += max<T>(top_L() - a, 0);
    push_R(pushpop_L(a));
  }
  // f(x) += max(a - x, 0)
  void add_left_slope(T a) {
    min_f += max<T>(a - top_R(), 0);
    push_L(pushpop_R(a));
  }
  // f(x) += abs(x - a)
  void add_v(T a) {
    add_right_slope(a);
    add_left_slope(a);
  }

  void shift(T a, T b) {
    assert(a <= b);
    add_L += a;
    add_R += b;
  }
  void shift(T a) { shift(a, a); }

  // f(x) -= max(x - a, 0)
  void erase_right_slope(T a) {
    if (top_R() <= a) {
      R.erase(a - add_R);
    } else {
      min_f -= top_R() - a;
      L.erase(a - add_L);
      push_L(top_R());
      R.pop();
    }
  }
  // f(x) -= max(a - x, 0)
  void erase_left_slope(T a) {
    if (a <= top_L()) {
      L.erase(a - add_L);
    } else {
      min_f -= a - top_L();
      R.erase(a - add_R);
      push_R(top_L());
      L.pop();
    }
  }
  // f(x) -= abs(x - a)
  void erase_v(T a) {
    erase_right_slope(a);
    erase_left_slope(a);
  }
};

template <class T>
using ErasableSlopeTrick = SlopeTrick < T,
      erasable_priority_queue<T>, erasable_priority_queue<T, greater<T>>>;
