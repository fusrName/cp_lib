struct Tropical {
    static constexpr long long INF = 1002003004005006007;
    Tropical(long long x=-INF): x(x) {}
    long long x;
    static const Tropical zero, one;
    friend Tropical operator+(const Tropical& a, const Tropical& b) {
        return max(a.x, b.x);
    }
    friend Tropical operator*(const Tropical& a, const Tropical& b) {
        return a.x == -INF || b.x == -INF ? -INF : a.x + b.x;
    }
    Tropical& operator+=(const Tropical& rhs) {
        return *this = *this + rhs;
    }
    Tropical& operator*=(const Tropical& rhs) {
        return *this = *this * rhs;
    }
    friend std::ostream& operator<< (std::ostream& os, const Tropical& t) {
        return os << t.x;
    }
};

const Tropical Tropical::zero;
const Tropical Tropical::one(0);