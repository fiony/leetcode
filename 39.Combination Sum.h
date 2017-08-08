/*
Given a set of candidate numbers (C) (without duplicates) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.

The same repeated number may be chosen from C unlimited number of times.

Note:
All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
For example, given candidate set [2, 3, 6, 7] and target 7, 
A solution set is: 
[
  [7],
  [2, 2, 3]
]
*/
class Solution {
public:

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) 
	{
		vector<vector<int>> result;
		sort(candidates.begin(), candidates.end());
		GetCombinationSumRecursive(target, candidates.size(), result, candidates);
        return result;
    }
	
    // Get Combination for target using first N candidates (candidates is sorted)
	// F(X, n) = F(X-C[n], n) + F(X-C[n-1], n-1) + ... F(X-C[1], 1), where X >= C[n]
    bool GetCombinationSumRecursive(int target, int n, vector<vector<int>>& result, const vector<int>& candidates)
	{
		if(n==1)
		{
			if(target % candidates[0] == 0)
			{
				result.push_back(vector<int>(target/candidates[0], candidates[0]));
				return true;
			}
			return false;
		}
		
		bool solvable = false; 
		for(int i = 0; i < n; i++)
		{
			if(target > candidates[i])
			{
                vector<vector<int>> cur_result;
				if(GetCombinationSumRecursive(target-candidates[i], i+1, cur_result, candidates))
				{
					for(auto& ret : cur_result)
                    {
						ret.push_back({candidates[i]});// only append for current resurive result
                        result.push_back(ret);  // add a solveable result
                    }
					solvable = true;
				}
			}
			else if(target == candidates[i])
			{
				result.push_back({candidates[i]});
				solvable = true;
			}
			else
			{
				break;	// no need to check more
			}
		}
		return solvable;
	}
};
