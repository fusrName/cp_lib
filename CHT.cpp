template<class Int = int, class Long = long long>
struct CHT {
    const Int IntMax = std::numeric_limits<Int>::max() / 2 - 1;
    const Int IntMin = std::numeric_limits<Int>::min() / 2 + 1;
    using P = std::pair<Int, Int>;
    struct Line { Int a, b; };
    struct Segment { Int b, l, r; };
    std::map<Int, Segment> segs; // maps slopes to segments
    std::map<Int, Line> ends; // maps x-coordinates to lines

    // y = ax+b = cx+d, a<c
    // x = (b-d)/(c-a), y = (bc-ad)/(c-a)
    // ex+f<=y <=> e(b-d)+f(c-a)<=bc-ad

    Int ceiling(Int a, Int b) {
        assert(b > 0);
        return a > 0 ? (a - 1) / b + 1 : a / b;
    }
    // returns ceiling of x
    // y=ax+b=cx+d
    Int intersection_x(Int a, Int b, Int c, Int d) {
        assert(a <= c);
        if (a == c) return b > d ? IntMax : IntMin;
        return ceiling(b - d, c - a);
    }

    // query x must be between [IntMin, IntMax)
    void add_line(Int a, Int b) {
        assert(IntMin <= a && a < IntMax);
        assert(IntMin <= b && b < IntMax);
        Int l = IntMin, r = IntMax;
        auto it = segs.lower_bound(a);
        auto it_x = it == segs.end() ? ends.end() : ends.find(it->second.r);
        while(it != segs.end()) {
            const Int& a_it = it->first;
            auto& [b_it, l_it, r_it] = it->second;
            Int x = intersection_x(a, b, a_it, b_it);
            if (x >= r_it) {
                it = segs.erase(it);
                it_x = ends.erase(it_x);
            } else {
                r = x;
                if (x > l_it) {
                    l_it = x;
                }
                break;
            }
        }
        while(it != segs.begin()) {
            auto pit = std::prev(it);
            auto pit_x = std::prev(it_x);
            const Int& a_it = pit->first;
            auto& [b_it, l_it, r_it] = pit->second;
            Int x = intersection_x(a_it, b_it, a, b);
            if (x <= l_it) {
                segs.erase(pit);
                ends.erase(pit_x);
            } else {
                l = x;
                if (x < r_it) {
                    r_it = x;
                    ends.erase(pit_x);
                    ends.emplace_hint(it_x, r_it, Line{a_it, b_it});
                }
                break;
            }
        }
        if (l < r) {
            segs.emplace_hint(it, a, Segment{b, l, r});
            ends.emplace_hint(it_x, r, Line{a, b});
        }
    }

    Long get_max(Int x) {
        assert(IntMin <= x && x < IntMax);
        assert(!segs.empty());
        auto [a, b] = ends.upper_bound(x)->second;
        return (Long)a * x + b;
    }
};

template<class Int = int, class Long = long long>
struct CHT_min : CHT<Int, Long> {
    void add_line(Int a, Int b) { CHT<Int, Long>::add_line(-a, -b); }
    Long get_max(Int) = delete;
    Long get_min(Int x) { return -CHT<Int, Long>::get_max(x); }
};