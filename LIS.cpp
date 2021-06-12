int LIS(std::vector<int>& x) {
    int n = x.size();
    std::vector<int> d;
    d.reserve(n);
    for(int xi: x) {
        auto it = std::lower_bound(d.begin(), d.end(), xi);
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return d.size();
}

int LNDS(std::vector<int>& x) {
    int n = x.size();
    std::vector<int> d;
    d.reserve(n);
    for(int xi: x) {
        auto it = std::upper_bound(d.begin(), d.end(), xi);
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return d.size();
}

// takes iterators [first, last) as arguments
template<class T>
int LIS(T first, T last) {
    std::vector<typename iterator_traits<T>::value_type> d;
    d.reserve(last - first);
    for(; first != last; first++) {
        auto xi = *first;
        auto it = std::lower_bound(d.begin(), d.end(), xi);
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return d.size();
}


// LIS at each end point
template<class T>
std::vector<int> LIS(T first, T last) {
    std::vector<typename iterator_traits<T>::value_type> d;
    d.reserve(last - first);
    std::vector<int> ret;
    ret.reserve(last - first);
    for(; first != last; first++) {
        auto xi = *first;
        auto it = std::lower_bound(d.begin(), d.end(), xi);
        ret.push_back(it - d.begin());
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return ret;
}