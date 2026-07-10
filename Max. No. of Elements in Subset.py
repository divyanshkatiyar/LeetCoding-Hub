class Solution {
public:
    int maximumLength(vector<int>& nums) {
        unordered_map<long long, int> cnt;

        for (int x : nums)
            cnt[x]++;

        int ans = 1;

        // Handle 1 separately
        if (cnt.count(1))
            ans = max(ans, cnt[1] - (cnt[1] % 2 == 0));

        for (auto &[x, f] : cnt) {
            if (x == 1) continue;

            long long cur = x;
            int len = 0;

            while (true) {
                if (!cnt.count(cur))
                    break;

                if (cnt[cur] == 1) {
                    len++;
                    break;
                }

                // cnt[cur] >= 2
                if (cur > 1000000000LL / cur) {
                    len++;
                    break;
                }

                long long nxt = cur * cur;

                if (!cnt.count(nxt)) {
                    len++;
                    break;
                }

                len += 2;
                cur = nxt;
            }

            ans = max(ans, len);
        }

        return ans;
    }
};
