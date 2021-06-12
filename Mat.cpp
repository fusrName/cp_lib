template<class R>
struct Mat: std::vector<std::vector<R>> {
    Mat(int n=0): std::vector<std::vector<R>>::vector(n, vector<R>(n)) {}
    friend Mat operator*(const Mat& lhs, const Mat& rhs) {
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
    Mat pow(unsigned long long n) {
        Mat<R> A = *this;
        Mat<R> res = Mat<R>::I(A.size());
        for(; n; n >>= 1) {
            if (n & 1) res *= A;
            A *= A;
        }
        return res;
    }
    static Mat I(int n) {
        Mat ret(n);
        for(int i = 0; i < n; i++) ret[i][i] = R::one;
        return ret;
    }
};