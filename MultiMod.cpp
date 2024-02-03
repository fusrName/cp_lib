default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
template <int... ids>
struct MultiMod {
  tuple<dynamic_modint<ids>...> val;
  MultiMod() = default;
  MultiMod(integral auto x) { (..., (get<ids>(val) = x)); }
  friend MultiMod operator+(const MultiMod& lhs, const MultiMod& rhs) {
    MultiMod res;
    (..., (get<ids>(res.val) = get<ids>(lhs.val) + get<ids>(rhs.val)));
    return res;
  }
  friend MultiMod operator-(const MultiMod& lhs, const MultiMod& rhs) {
    MultiMod res;
    (..., (get<ids>(res.val) = get<ids>(lhs.val) - get<ids>(rhs.val)));
    return res;
  }
  friend MultiMod operator*(const MultiMod& lhs, const MultiMod& rhs) {
    MultiMod res;
    (..., (get<ids>(res.val) = get<ids>(lhs.val) * get<ids>(rhs.val)));
    return res;
  }
  friend bool operator<(const MultiMod& lhs, const MultiMod& rhs) {
    bool res = false;
    (... && (get<ids>(lhs.val).val() < get<ids>(rhs.val).val()
                 ? (res = true, false)
                 : get<ids>(lhs.val) == get<ids>(rhs.val)));
    return res;
  }
  friend bool operator==(const MultiMod& lhs, const MultiMod& rhs) {
    return (... && (get<ids>(lhs.val) == get<ids>(rhs.val)));
  }
  explicit operator bool() const { return (... || get<ids>(val).val()); }
  static void mod_init() {
    (..., dynamic_modint<ids>::set_mod(
              uniform_int_distribution<>(11, 2000001000)(gen)));
  }
  static MultiMod raw(auto x) {
    MultiMod res;
    (..., (get<ids>(res.val) = dynamic_modint<ids>::raw(x)));
    return res;
  }
};
