template<class R, R (*zero)(), R (*one)()>
struct Mat: std::vector<std::vector<R>> {
    Mat(int n=0): std::vector<std::vector<R>>::vector(n, vector<R>(n, zero())) {}
    friend Mat operator*(const Mat& lhs, const Mat& rhs) {
        assert(lhs.size() == rhs.size());
        const int n = lhs.size();
        Mat ret(n);
        for(int i = 0; i < n; i++) for(int k = 0; k < n; k++) for(int j = 0; j < n; j++) {
            ret[i][j] += lhs[i][k] * rhs[k][j];
        }
        return ret;
    }
    Mat& operator*=(const Mat& rhs) {
        return (*this) = (*this) * rhs;
    }
    friend Mat operator+(const Mat& lhs, const Mat& rhs) {
        assert(lhs.size() == rhs.size());
        const int n = lhs.size();
        Mat ret(n);
        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) ret[i][j] = lhs[i][j] + rhs[i][j];
        return ret;
    }
    Mat& operator+=(const Mat& rhs) {
        return (*this) = (*this) + rhs;
    }
    Mat pow(unsigned long long n) {
        Mat A = *this;
        Mat res = Mat::I(A.size());
        for(; n; n >>= 1) {
            if (n & 1) res *= A;
            A *= A;
        }
        return res;
    }
    static Mat I(int n) {
        Mat ret(n);
        for(int i = 0; i < n; i++) ret[i][i] = one();
        return ret;
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