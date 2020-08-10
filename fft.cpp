std::vector<MyClx> fft(const std::vector<MyClx>& a) {
    const int n = a.size();
    if (n == 1) return a;
    const int n2 = n / 2;
    std::vector<MyClx> a_even(n2), a_odd(n2);
    for(int i = 0; i < n2; i+=1) {
        a_even[i] = a[i*2];
        a_odd[i]  = a[i*2+1];
    }
    auto f_even = fft(a_even), f_odd = fft(a_odd);
    std::vector<MyClx> f(n);
    MyClx zeta(cos(2*M_PI/n), sin(2*M_PI/n));
    MyClx x(1,0);
    for(int i = 0; i < n2; i++) {
        f[i] = f_even[i] + x * f_odd[i];
        f[i+n2] = f_even[i] - x * f_odd[i];
        x *= zeta;
    }
    return f;
}

template<class T> std::vector<MyClx> fft(const std::vector<T>& a) {
    int sz = a.size();
    std::vector<MyClx> a2(sz);
    for(int i = 0; i < sz; i++) {
        a2[i].real = a[i];
        a2[i].imag = 0;
    }
    return fft(a2);
}

std::vector<MyClx> ifft(std::vector<MyClx> a) {
    const int n = a.size();
    if (n == 1) return a;
    const int n2 = n / 2;
    std::vector<MyClx> a_even(n2), a_odd(n2);
    for(int i = 0; i < n2; i+=1) {
        a_even[i] = a[i*2];
        a_odd[i]  = a[i*2+1];
    }
    auto f_even = ifft(a_even), f_odd = ifft(a_odd);
    std::vector<MyClx> f(n);
    MyClx zeta(cos(2*M_PI/n), -sin(2*M_PI/n));
    MyClx x(1,0);
    for(int i = 0; i < n2; i++) {
        f[i] = f_even[i] + x * f_odd[i];
        f[i+n2] = f_even[i] - x * f_odd[i];
        x *= zeta;
    }
    return f;
}

std::vector<MyClx> ifft_wrapper(const std::vector<MyClx>& a) {
    auto f = ifft(a);
    int sz = f.size();
    for(int i = 0; i < sz; i++) f[i] /= sz;
    return f;
}
