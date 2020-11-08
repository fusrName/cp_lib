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
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
int lowerBound = 0, upperBound = 13; // inclusive
std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
//std::uniform_int_distribution<int> distribution;
auto RI = std::bind (distribution, generator);
int r = RI();