#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.length();
        int total_ones = count(s.begin(), s.end(), '1');
        
        // Step 1: Identify all global maximal contiguous '0'-blocks
        vector<int> start_idx, end_idx, lengths;
        for (int i = 0; i < n; ) {
            if (s[i] == '0') {
                int j = i;
                while (j < n && s[j] == '0') {
                    j++;
                }
                start_idx.push_back(i);
                end_idx.push_back(j - 1);
                lengths.push_back(j - i);
                i = j;
            } else {
                i++;
            }
        }
        
        int M = start_idx.size();
        
        // Step 2: Build Sparse Table over H[i] = lengths[i] + lengths[i + 1]
        vector<int> H;
        if (M >= 2) {
            H.resize(M - 1);
            for (int i = 0; i < M - 1; ++i) {
                H[i] = lengths[i] + lengths[i + 1];
            }
        }
        
        int P = H.size();
        vector<int> LOG(P + 1, 0);
        for (int i = 2; i <= P; ++i) {
            LOG[i] = LOG[i / 2] + 1;
        }
        
        int max_log = (P > 0) ? LOG[P] : 0;
        vector<vector<int>> st(max_log + 1, vector<int>(P, 0));
        
        if (P > 0) {
            for (int i = 0; i < P; ++i) {
                st[0][i] = H[i];
            }
            for (int j = 1; j <= max_log; ++j) {
                int len = 1 << (j - 1);
                for (int i = 0; i + (1 << j) <= P; ++i) {
                    st[j][i] = max(st[j - 1][i], st[j - 1][i + len]);
                }
            }
        }
        
        auto query_rmq = [&](int L, int R) -> int {
            if (L > R || P == 0) return 0;
            int j = LOG[R - L + 1];
            return max(st[j][L], st[j][R - (1 << j) + 1]);
        };
        
        // Step 3: Process each query
        vector<int> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int l = q[0], r = q[1];
            
            // Binary search for range [a, b] of overlapping '0'-blocks
            auto it_a = lower_bound(end_idx.begin(), end_idx.end(), l);
            int a = distance(end_idx.begin(), it_a);
            
            auto it_b = upper_bound(start_idx.begin(), start_idx.end(), r);
            int b = distance(start_idx.begin(), it_b) - 1;
            
            int gain = 0;
            if (a <= b) {
                int k = b - a + 1;
                if (k >= 2) {
                    if (k == 2) {
                        int L0 = end_idx[a] - max(start_idx[a], l) + 1;
                        int L1 = min(end_idx[b], r) - start_idx[b] + 1;
                        gain = L0 + L1;
                    } else if (k == 3) {
                        int L0 = end_idx[a] - max(start_idx[a], l) + 1;
                        int L1 = lengths[a + 1];
                        int L2 = min(end_idx[b], r) - start_idx[b] + 1;
                        gain = max(L0 + L1, L1 + L2);
                    } else { // k >= 4
                        int L0 = end_idx[a] - max(start_idx[a], l) + 1;
                        int L1 = lengths[a + 1];
                        int Lk_2 = lengths[b - 1];
                        int Lk_1 = min(end_idx[b], r) - start_idx[b] + 1;
                        
                        int cand1 = L0 + L1;
                        int cand2 = query_rmq(a + 1, b - 2);
                        int cand3 = Lk_2 + Lk_1;
                        gain = max({cand1, cand2, cand3});
                    }
                }
            }
            
            ans.push_back(total_ones + gain);
        }
        
        return ans;
    }
};

