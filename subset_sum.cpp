// computes subset sum and returns choice in time complexity O(NAmax)
vector<char> subset_sum(const vector<int>& a, int target) {
    const int n = a.size();
    // assume a is split into a[0,b) and a[b,n), candidates for removal and addition, respectively.
    int b = 0;
    int initial_sum = 0;
    while(b < n && initial_sum + a[b] <= target) initial_sum += a[b++];
    if (b == n) return initial_sum == target ? vector<char>(n, true) : vector<char>{};
    // dp works within range [window_left, window_right)
    const int window_left = min(initial_sum, target + 1 - (b ? *max_element(a.begin(), a.begin() + b) : 0)),
              window_right = max(target, target - 1 + *max_element(a.begin() + b, a.end())) + 1,
              window_size = window_right - window_left;
    // dp[s] = max r s.t. any item in a[0, r) can be removed, under current sum = s
    vector<int> dp(window_size, -1), dp_old;
    vector<vector<int>> pre(n, vector<int>(window_size, -1));
    dp[initial_sum - window_left] = b;
    target -= window_left;
    for(int i = b; i < n; i++) {
        dp_old = dp;
        vector<int>& pre_i = pre[i];
        for(int s = target - 1; s >= 0; s--) {
            int ns = s + a[i];
            if (dp[s] > dp[ns]) {
                dp[ns] = dp[s];
                pre_i[ns] = -2;
            }
        }
        for(int s = window_size - 1; s > target; s--) {
            int last = max(0, dp_old[s]);
            for(int i = dp[s] - 1; i >= last; i--) {
                int ns = s - a[i];
                if (i > dp[ns]) {
                    dp[ns] = i;
                    pre_i[ns] = i;
                }
            }
        }
    }
    if (dp[target] == -1) return {};
    vector<char> subset(n);
    for(int i = 0; i < b; i++) subset[i] = true;
    for(int i = n - 1, s = target; i >= b; i--) {
        vector<int>& pre_i = pre[i];
        if (pre_i[s] == -1) continue;
        while(pre_i[s] != -2) {
            subset[pre_i[s]] = false;
            s += a[pre_i[s]];
        }
        subset[i] = true;
        s -= a[i];
    }
    return subset;
}