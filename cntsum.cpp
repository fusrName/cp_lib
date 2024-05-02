template <class T>
struct cntsum {
  T a, b;  // a+bx mod x^2
  friend cntsum operator+(const cntsum& lhs, const cntsum& rhs) { return {lhs.a + rhs.a, lhs.b + rhs.b}; }
  friend cntsum operator-(const cntsum& lhs, const cntsum& rhs) { return {lhs.a - rhs.a, lhs.b - rhs.b}; }
  friend cntsum operator*(const cntsum& lhs, const cntsum& rhs) { return {lhs.a * rhs.a, lhs.a * rhs.b + lhs.b * rhs.a}; }
  friend cntsum operator*(const T& c, const cntsum& x) { return {x.a, x.b + c * x.a}; }
  cntsum& operator+=(const cntsum& x) { return *this = *this + x; }
  cntsum& operator-=(const cntsum& x) { return *this = *this - x; }
  cntsum& operator*=(const cntsum& x) { return *this = *this * x; }
};
