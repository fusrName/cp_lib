struct Vec {
  using etype = long long;  // type of elements
  using ptype = long long;  // type of product results
  etype x = 0, y = 0;
  constexpr Vec operator-() const { return {-x, -y}; }
  friend constexpr Vec operator+(const Vec& a, const Vec& b) { return {a.x + b.x, a.y + b.y}; }
  friend constexpr Vec operator-(const Vec& a, const Vec& b) { return {a.x - b.x, a.y - b.y}; }
  friend constexpr etype operator*(const Vec& a, const Vec& b) { return a.x * b.y - a.y * b.x; }
  template <class T, enable_if_t<is_integral_v<T>>* = nullptr>
  friend constexpr Vec operator*(T c, const Vec& a) { return {c * a.x, c * a.y}; }
  template <class T, enable_if_t<is_integral_v<T>>* = nullptr>
  friend constexpr Vec operator*(const Vec& a, T c) { return c * a; }
  template <class T, enable_if_t<is_integral_v<T>>* = nullptr>
  friend constexpr Vec operator/(const Vec& a, T c) { return {a.x / c, a.y / c}; }
  Vec& operator+=(const Vec& a) { x += a.x; y += a.y; return *this; }
  Vec& operator-=(const Vec& a) { x -= a.x; y -= a.y; return *this; }
  template <class T, enable_if_t<is_integral_v<T>>* = nullptr>
  Vec& operator*=(T c) { x *= c; y *= c; return *this; }
  friend constexpr bool operator==(const Vec& a, const Vec& b) { return a.x == b.x && a.y == b.y; }
  friend constexpr bool operator!=(const Vec& a, const Vec& b) { return !(a == b); }
  constexpr int quadrant() const { return y > 0 ? (x > 0 ? 0 : 1) : y < 0 ? (x < 0 ? 2 : 3) : (x > 0 ? 0 : x < 0 ? 2 : -1); }
  friend constexpr bool operator<(const Vec& a, const Vec& b) {
    int qa = a.quadrant(), qb = b.quadrant();
    if (qa != qb) return qa < qb;
    ptype p1 = (ptype)a.x * b.y, p2 = (ptype)a.y * b.x;
    return p1 > p2 || (p1 == p2 && (a.x != 0 ? abs(a.x) < abs(b.x) : abs(a.y) < abs(b.y)));
  }
  friend constexpr bool operator>(const Vec& a, const Vec& b) { return b < a; }
  friend constexpr bool operator<=(const Vec& a, const Vec& b) { return !(b < a); }
  friend constexpr bool operator>=(const Vec& a, const Vec& b) { return !(a < b); }
  constexpr bool parallel_to(const Vec& a) const { return (ptype)x * a.y == (ptype)y * a.x; }
  constexpr ptype dot(const Vec& a) const { return (ptype)x * a.x + (ptype)y * a.y; }
  constexpr ptype norm2() const { return (ptype)x * x + (ptype)y * y; }
  constexpr bool is_zero() const { return x == 0 && y == 0; }
  constexpr Vec normalize() const {
    if (x == 0) return Vec{0, (y == 0 ? 0 : 1)};
    etype g = gcd(x, y);
    return *this / (x > 0 ? g : -g);
  }
  constexpr Vec rot90() const { return {-y, x}; }
  // exclusive
  constexpr bool in_between(const Vec& a, const Vec& b) const {
    Vec x = a - *this, y = b - *this;
    return x.parallel_to(y) && x.dot(y) < 0;
  }
  friend std::ostream& operator<< (std::ostream& os, const Vec& t) { return os << t.x << ' ' << t.y; }
  friend std::istream& operator>> (std::istream& os, Vec& t) { return os >> t.x >> t.y; }
};