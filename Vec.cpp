struct Vec {
    long long x = 0, y = 0;

    friend Vec operator+(const Vec& a, const Vec& b) {
        return {a.x + b.x, a.y + b.y};
    }
    friend Vec operator-(const Vec& a, const Vec& b) {
        return {a.x - b.x, a.y - b.y};
    }
    friend long long operator*(const Vec& a, const Vec& b) {
        return a.x * b.y - a.y * b.x;
    }
    friend Vec operator*(long long c, const Vec& a) {
        return {c * a.x, c * a.y};
    }
    friend Vec operator*(const Vec& a, long long c) {
        return c * a;
    }
    friend Vec operator/(const Vec& a, long long c) {
        return {a.x / c, a.y / c};
    }
    long long norm2() {
        return x * x + y * y;
    }
    bool is_parallel(const Vec& a) {
        // return __int128_t(x) * a.y == __int128_t(y) * a.x;
        return (*this) * a == 0;
    }
    long long dot(const Vec& a) {
        return x * a.x + y * a.y;
    }
    Vec& operator+=(const Vec& rhs) {
        return *this = *this + rhs;
    }
    Vec& operator-=(const Vec& rhs) {
        return *this = *this - rhs;
    }
    Vec& operator*=(long long c) {
        x *= c;
        y *= c;
        return *this;
    }
    friend bool operator==(const Vec& lhs, const Vec& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    friend bool operator!=(const Vec& lhs, const Vec& rhs) {
        return lhs.x != rhs.x || lhs.y != rhs.y;
    }
    friend bool operator<(const Vec& a, const Vec& b) {
        if (a.x == 0 && a.y == 0) return !(b.x == 0 && b.y == 0);
        if (b.x == 0 && b.y == 0) return false;
        int qa = 0 < a.x && 0 <= a.y ? 0:
                 a.x <= 0 && 0 < a.y ? 1:
                 a.x < 0             ? 2:
                                       3;
        int qb = 0 < b.x && 0 <= b.y ? 0:
                 b.x <= 0 && 0 < b.y ? 1:
                 b.x < 0             ? 2:
                                       3;
        if (qa != qb) return qa < qb;
        // auto t1 = __int128_t(a.x) * b.y, t2 = __int128_t(a.y) * b.x;
        auto t1 = a.x * b.y, t2 = a.y * b.x;
        return t1 > t2 || (t1 == t2 && (std::abs(a.x) < std::abs(b.x) || std::abs(a.y) < std::abs(b.y)));
    }
    friend std::ostream& operator<< (std::ostream& os, const Vec& t) {
        return os << t.x << ' ' << t.y;
    }
    friend std::istream& operator>> (std::istream& os, Vec& t) {
        return os >> t.x >> t.y;
    }
};