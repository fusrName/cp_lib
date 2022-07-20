// to obtain type name
#include<boost/core/demangle.hpp>
#define GET_TYPE(x) boost::core::demangle(typeid(x).name())
// example
std::string tname = GET_TYPE(345ull);


// to measure execution time
#include<chrono>
auto begin = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();
auto elapsed = begin - end;
auto elapsed_nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
double elapsed_time = elapsed_nanos / 1e9;


// to output double with fixed precision
auto initialize__ = []() {
std::cout << std::fixed << std::setprecision(16);
return 0;}();


// provides arbitrary precision integer type
#include<boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;
// example
cpp_int n;


// makes visible only cin, cout, endl
using std::cout;
using std::cin;
using std::endl;


// generates random numbers
std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
int lowerBound = 0, upperBound = 13; // inclusive
using dist_type = std::uniform_int_distribution<>;
using param_type = dist_type::param_type;
dist_type dist(lowerBound, upperBound);
// dist.param(param_type(3, 5));
auto RI = std::bind (dist, gen);
int r = RI();

std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
using dist_type = std::uniform_int_distribution<>;
using ptype = dist_type::param_type;
dist_type dist;
dist(gen, ptype{1, 5});

// print vector
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& x) {
    int n = x.size();
    if (n == 0) return os;
    for(int i = 0; i < n-1; ++i) {
        os << x[i] << ' ';
    }
    return os << x[n-1];
}

// returns d-dimensional vector
template<int d, class T>
enable_if_t<d == 1, vector<T>> VV(int n) { return vector<T>(n); }
template<int d, class T>
enable_if_t<d == 1, vector<T>> VV(int n, const T& x) { return vector<T>(n, x); }
template<int d, class T, class... Args>
auto VV(enable_if_t<(d > 1), int> n, const Args&... args) { return vector(n, VV<d - 1, T>(args...)); }

// // 3d vector of size 3 * 4 * 5 filled with 6
// auto v = VV<3, ll>(3, 4, 5, 6);