#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

struct Query
{
	int age;
	int idx;
};

typedef std::vector<int> vint;
typedef std::vector<vint> mint;
typedef std::vector<bool> vbool;
typedef std::pair<int, int> pint;
typedef std::vector<pint> vpint;
typedef std::vector<std::vector<Query>> mquery;
typedef std::vector<std::string> vstring;
typedef std::map<std::string, int> m_str_int;

mint graph;
vint age;
mquery queries;
vpint call_stack;
vstring idx_to_spec;
m_str_int spec_to_idx;
vstring results;

void dfs(int node)
{
	call_stack.push_back(std::make_pair(age[node], node));
	for (auto q : queries[node])
	{
		int ancestor_idx = std::lower_bound(call_stack.begin(), call_stack.end(), q.age, [](const pint &p1, const int value) -> bool
											{ return p1.first > value; })->second;
		results[q.idx] = idx_to_spec[ancestor_idx];
	}

	for (int child : graph[node])
		dfs(child);

	call_stack.pop_back();
}

void testcase()
{
	int n, q; std::cin >> n >> q;

	age = vint(n);
	int root_age = 0;
	int root_idx = -1;
	idx_to_spec = vstring(n);
	spec_to_idx = m_str_int();
	for (int i = 0; i < n; i++)
	{
		std::string s;
		std::cin >> s;
		int a;
		std::cin >> a;
		spec_to_idx[s] = i;
		idx_to_spec[i] = s;
		age[i] = a;
		if (a > root_age)
		{
			root_age = a;
			root_idx = i;
		}
	}

	graph = mint(n);
	for (int i = 0; i < n - 1; i++)
	{
		std::string s, p;
		std::cin >> s >> p;
		int idx_s = spec_to_idx[s];
		int idx_p = spec_to_idx[p];
		graph[idx_p].push_back(idx_s);
	}

	queries = mquery(n);
	results = vstring(q);
	for (int i = 0; i < q; i++)
	{
		std::string s; std::cin >> s;
		int age; std::cin >> age;
		int s_idx = spec_to_idx[s];
		queries[s_idx].push_back({age, i});
	}

	dfs(root_idx);

	for (int i = 0; i < q; i++)
		std::cout << results[i] << " ";
	std::cout << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}