template <class T, class Comp = less<T>>
struct erasable_priority_queue {
  priority_queue<T, vector<T>, Comp> q, q_erase;
  void push(T x) { q.push(move(x)); }
  template <class... Args>
  void emplace(Args&&... args) {
    q.emplace(forward<Args>(args)...);
  }
  void erase(T x) { q_erase.push(move(x)); }
  decltype(auto) top() {
    assert(!empty());
    expose_top();
    return q.top();
  }
  void pop() {
    assert(!empty());
    expose_top();
    q.pop();
  }
  int size() { return int(q.size()) - int(q_erase.size()); }
  bool empty() { return !size(); }
  void expose_top() {
    while (!q_erase.empty() && q_erase.top() == q.top()) {
      q_erase.pop();
      q.pop();
    }
  }
};
