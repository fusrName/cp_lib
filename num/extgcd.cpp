template<class T>
struct EGCD {
  T g, x, y;
};

template<class T>
EGCD<T> extgcd(T a, T b) {
  T ax = 1, ay = 0, bx = 0, by = 1;
  while(b) {
    T q = a / b;
    T tmp = a;
    a = b;
    b = tmp % b;
    T tmpx = ax, tmpy = ay;
    ax = bx;
    ay = by;
    bx = tmpx - q * bx;
    by = tmpy - q * by;
  }
  return {a, ax, ay};
}
