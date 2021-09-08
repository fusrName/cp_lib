template<class T=long long>
struct SlopeTrick {
    static constexpr T INF = std::numeric_limits<T>::max() / 3;
    T min_f = 0;

    // acutual value of x in L is ADD_add_L(x) = x + add_L
    T add_L = 0, add_R = 0;
    std::priority_queue<T> L;
    std::priority_queue<T, std::vector<T>, std::greater<>> R;

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
        min_f += std::max<T>(top_L() - a, 0);
        push_R(pushpop_L(a));
    }
    // f(x) += max(a - x, 0)
    void add_left_slope(T a) {
        min_f += std::max<T>(a - top_R(), 0);
        push_L(pushpop_R(a));
    }
    // f(x) += abs(x - a)
    void add_v(T a) { add_right_slope(a); add_left_slope(a); }

    void shift(T a, T b) { 
        assert(a <= b);
        add_L += a;
        add_R += b;
    }
    void shift(T a) { shift(a, a); }
};