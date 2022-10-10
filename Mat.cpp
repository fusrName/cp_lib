struct mintRing : mint {
  using mint::mint;
  mintRing(mint x) : mint::mint(x) {}
  static mintRing one() { return mint::raw(1); }
  static mintRing zero() { return mint(); }
};

template <class R>
struct Mat : vector<vector<R>> {
  int n, m;

  Mat(int n, int m)
      : vector<vector<R>>::vector(n, vector<R>(m, R::zero())), n(n), m(m) {}
  Mat() : n(0), m(0) {}

  friend Mat operator*(const Mat& lhs, const Mat& rhs) {
    assert(lhs.m == rhs.n);
    const int n = lhs.n, s = lhs.m, m = rhs.m;
    Mat res(n, m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < s; j++) {
        for (int k = 0; k < m; k++) {
          res[i][k] += lhs[i][j] * rhs[j][k];
        }
      }
    }
    return res;
  }
  Mat& operator*=(const Mat& rhs) {
    assert(m == rhs.n);
    const int nn = n, s = m, mm = rhs.m;
    static vector<vector<R>> tmp;
    tmp.resize(nn);
    for (int i = 0; i < nn; i++) tmp[i].assign(mm, R::zero());
    for (int i = 0; i < nn; i++) {
      for (int j = 0; j < s; j++) {
        for (int k = 0; k < mm; k++) {
          tmp[i][k] += (*this)[i][j] * rhs[j][k];
        }
      }
    }
    swap(tmp, *this);
    return *this;
  }
  friend Mat operator+(const Mat& lhs, const Mat& rhs) {
    assert(lhs.n == rhs.n && lhs.m == rhs.m);
    const int n = lhs.n, m = lhs.m;
    Mat res(n, m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        res[i][j] = lhs[i][j] + rhs[i][j];
      }
    }
    return res;
  }
  Mat& operator+=(const Mat& rhs) {
    assert(n == rhs.n && m == rhs.m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        (*this)[i][j] += rhs[i][j];
      }
    }
    return *this;
  }
  Mat pow(unsigned long long k) {
    assert(n == m);
    static Mat<R> tmp;
    tmp = *this;
    Mat res = Mat::I(n);
    while (true) {
      if (k & 1) res *= tmp;
      k >>= 1;
      if (!k) break;
      tmp *= tmp;
    }
    return res;
  }
  static Mat I(int n) {
    Mat res(n, n);
    for (int i = 0; i < n; i++) res[i][i] = R::one();
    return res;
  }
};



template<class T, int N>
struct Mat: std::array<std::array<T, N>, N> {
    friend Mat<T, N> operator*(const Mat& A, const Mat& B) {
        Mat<T, N> C = {};
        for(int i = 0; i < N; i++) {
            for(int k = 0; k < N; k++) {
                for(int j = 0; j < N; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }
    friend std::array<T, N> operator*(const Mat& A, const std::array<T, N>& v) {
        std::array<T, N> x = {};
        for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) x[i] += A[i][j] * v[j];
        return x;
    }
    friend Mat<T, N> operator+(const Mat& A, const Mat& B) {
        Mat<T, N> C;
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        return C;
    }
    Mat<T, N>& operator*=(const Mat& A) { return *this = *this * A; }
    Mat<T, N>& operator+=(const Mat& A) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                (*this)[i][j] += A[i][j];
            }
        }
    }
    static Mat<T, N> I() {
        Mat<T, N> X = {};
        for(int i = 0; i < N; i++) X[i][i] = 1;
        return X;
    }
    Mat<T, N> pow(long long k) const {
        assert(k >= 0);
        auto X = *this;
        auto Y = I();
        while(k) {
            if (k & 1) Y *= X;
            k >>= 1;
            if (k) X *= X;
        }
        return Y;
    }
    Mat<T, N> inv() const {
        auto X = *this;
        auto Y = I();
        for(int p = 0; p < N; p++) {
            bool ok = false;
            for(int i = p; i < N; i++) {
                if (X[i][p] != T()) {
                    ok = true;
                    if (i != p) {
                        // std::swap(X[i], X[p]);
                        for(int j = p; j < N; j++) std::swap(X[i][j], X[p][j]);
                        std::swap(Y[i], Y[p]);
                    }
                    break;
                }
            }
            assert(ok);
            T c = 1 / X[p][p];
            for(int j = p; j < N; j++) X[p][j] *= c;
            for(int j = 0; j < N; j++) Y[p][j] *= c;
            for(int i = 0; i < N; i++) if (i != p) {
                T c = X[i][p];
                for(int j = p; j < N; j++) X[i][j] -= c * X[p][j];
                for(int j = 0; j < N; j++) Y[i][j] -= c * Y[p][j];
            }
        }
        return Y;
    }

    T det() const {
        if (N == 0) return T();
        auto X = *this;
        bool flag = false;
        T res = 1;
        bool ok = false;
        for(int p = 0; p < N; p++) {
            for(int i = p; i < N; i++) {
                if (X[i][p] != T()) {
                    ok = true;
                    if (i != p) {
                        flag = !flag;
                        for(int j = p; j < N; j++) std::swap(X[i][j], X[p][j]);
                    }
                    break;
                }
            }
            if (!ok) return T();
            // T = modint
            for(int i = p + 1; i < N; i++) {
                T c0 = 1, c1 = 0, d0 = 0, d1 = 1;
                int a = X[p][p].val(), b = X[i][p].val();
                while(true) {
                    if (b == 0) {
                        break;
                    }
                    int q = a / b;
                    a -= q * b;
                    c0 -= q * d0;
                    c1 -= q * d1;
                    if (a == 0) {
                        flag = !flag;
                        for(int j = p; j < N; j++) std::swap(X[i][j], X[p][j]);
                        std::swap(c0, d1);
                        std::swap(c1, d0);
                        break;
                    }
                    q = b / a;
                    b -= q * a;
                    d0 -= q * c0;
                    d1 -= q * c1;
                }
                for(int j = p; j < N; j++) {
                    T a = X[p][j], b = X[i][j];
                    T x = c0 * a + c1 * b, y = d0 * a + d1 * b;
                    X[p][j] = x;
                    X[i][j] = y;
                }
            }
            res *= X[p][p];
        }
        if (flag) res = -res;
        return res;
    }
    friend std::ostream& operator<<(std::ostream& os, const Mat<T, N>& A) {
        for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) os << A[i][j] << " \n"[j + 1 == N];
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const mint& x) {
    return os << x.val();
}





template<class T, T (*add)(T, T), T (*zero)(), T (*mul)(T, T), T (*one)(), int N>
struct Mat : array<array<T, N>, N> {
  using M = Mat<T, add, zero, mul, one, N>;
  void make_identity() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        (*this)[i][j] = zero();
      }
    }
    for (int i = 0; i < N; i++) {
      (*this)[i][i] = one();
    }
  }
  M& operator+=(const M& rhs) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        (*this)[i][j] = add((*this)[i][j], rhs[i][j]);
      }
    }
    return *this;
  }
  M& operator*=(const M& rhs) {
    static M temp;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        temp[i][j] = zero();
      }
    }
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
          temp[i][k] = add(temp[i][k], mul((*this)[i][j], rhs[j][k]));
        }
      }
    }
    *this = temp;
    return *this;
  }
  template <class I>
  void inplace_pow(I k) {
    assert(k >= 0);
    static M temp;
    temp = *this;
    make_identity();
    while (k) {
      if (k & 1) *this *= temp;
      k >>= 1;
      if (k) temp *= temp;
    }
  }
  friend ostream& operator<<(ostream& os, const M& A) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        os << A[i][j] << " \n"[j + 1 == N];
      }
    }
    return os;
  }
};

// tropical semiring
constexpr ll INF = 1002003004005006007;
ll add(ll x, ll y) { return max(x, y); }
ll zero() { return -INF; }
ll mul(ll x, ll y) { return x + y; }
ll one() { return 0; }

using M = Mat<ll, add, zero, mul, one, 16>;
