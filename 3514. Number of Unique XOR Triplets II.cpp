class Solution {
public:
    int uniqueXorTriplets(vector<int>& nums) {
        const int MAXX = 2048;

        vector<vector<bool>> dp(4, vector<bool>(MAXX, false));
        dp[0][0] = true;

        for (int x : nums) {
            for (int k = 2; k >= 0; --k) {
                for (int v = 0; v < MAXX; ++v) {
                    if (dp[k][v])
                        dp[k + 1][v ^ x] = true;
                }
            }
        }

        vector<bool> ans(MAXX, false);

        // Triplets with repeated indices evaluate to the element itself.
        for (int x : nums)
            ans[x] = true;

        // Triplets with three distinct indices.
        for (int v = 0; v < MAXX; ++v)
            if (dp[3][v])
                ans[v] = true;

        int res = 0;
        for (bool ok : ans)
            if (ok) ++res;

        return res;
    }
};
