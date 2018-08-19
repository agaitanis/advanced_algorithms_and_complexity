#include <bits/stdc++.h>
using namespace std;

static void explore(int x, const vector<vector<int> > &adj, vector<int> &visited, vector<int> &ordered)
{
	visited[x] = 1;

	const vector<int> &neibs = adj[x];

	for (size_t i = 0; i < neibs.size(); i++) {
		int y = neibs[i];

		if (!visited[y]) {
			explore(y, adj, visited, ordered);
		}
	}

	ordered.push_back(x);
}

static vector<int> topo_sort(const vector<vector<int> > &adj)
{
	vector<int> visited(adj.size(), 0);
	vector<int> ordered;

	for (size_t i = 0; i < adj.size(); i++) {
		int x = i;

		if (!visited[x]) {
			explore(x, adj, visited, ordered);
		}
	}

	reverse(ordered.begin(), ordered.end());

	return ordered;
}

static vector<vector<int> > get_reverse_graph(const vector<vector<int> > &adj)
{
	vector<vector<int> > r_adj(adj.size());

	for (size_t i = 0; i < adj.size(); i++) {
		int x = i;

		for (size_t j = 0; j < adj[i].size(); j++) {
			int y = adj[i][j];

			r_adj[y].push_back(x);
		}
	}

	return r_adj;
}

static void explore2(int x, const vector<vector<int> > &adj, vector<int> &visited, vector<int> &scc)
{
	const vector<int> &neibs = adj[x];

	visited[x] = 1;
	scc.push_back(x);

	for (size_t i = 0; i < neibs.size(); i++) {
		int y = neibs[i];

		if (!visited[y]) {
			explore2(y, adj, visited, scc);
		}
	}
}

static vector<vector<int> > strongly_connected_components(const vector<vector<int> > &adj)
{
	vector<vector<int> > r_adj = get_reverse_graph(adj);
	vector<int> ordered = topo_sort(r_adj);
	vector<int> visited(adj.size(), 0);
	vector<vector<int> > sccs;

	for (size_t i = 0; i < ordered.size(); i++) {
		int x = ordered[i];

		if (!visited[x]) {
			vector<int> scc;

			explore2(x, adj, visited, scc);
			sccs.push_back(scc);
		}
	}

	return sccs;
}

struct Clause {
	int firstVar;
	int secondVar;
};

struct TwoSatisfiability {
	int numVars;
	vector<Clause> clauses;

	TwoSatisfiability(int n, int m) :
		numVars(n),
		clauses(m)
	{  }

	int varToGraphIndex(int x)
	{
		return x > 0 ? x - 1 : -x - 1 + numVars;
	}

	int graphIndexToVar(int i)
	{
		return i < numVars ? i + 1 : -i - 1 + numVars;
	}

	bool isSatisfiable(vector<int> &result)
	{
		int n = numVars;
		vector<vector<int> > adj(2*n);
		vector<int> parent(2*n);

		/* Construct the Implication Graph G */
		for (size_t i = 0; i < clauses.size(); i++) {
			int x = clauses[i].firstVar;
			int y = clauses[i].secondVar;

			adj[varToGraphIndex(-x)].push_back(varToGraphIndex(y));
			adj[varToGraphIndex(-y)].push_back(varToGraphIndex(x));
		}

		/* Find Strongly Connected Components (SCCs) of G */
		vector<vector<int> > sccs = strongly_connected_components(adj);

		for (size_t i = 0; i < sccs.size(); i++) {
			const vector<int> &scc = sccs[i];

			for (size_t j = 0; j < scc.size(); j++) {
				parent[scc[j]] = i;
			}
		}

		/* Check for unsatisfiable */
		for (size_t i = 0; i < n; i++) {
			if (parent[i] == parent[i+n]) return false;
		}

		/* Topological Sort of SCCs */
		vector<vector<int> > scc_adj(sccs.size());

		for (size_t i = 0; i < adj.size(); i++) {
			const vector<int> &neibs = adj[i];
			int parent1 = parent[i];

			for (size_t j = 0; j < neibs.size(); j++) {
				int parent2 = parent[neibs[j]];

				if (parent1 == parent2) continue;
				scc_adj[parent1].push_back(parent2);
			}
		}

		vector<int> ordered = topo_sort(scc_adj);

		/* Reverse iteration of SCCs and assignment of variables */
		for (vector<int>::reverse_iterator rit = ordered.rbegin(); rit != ordered.rend(); rit++) {
			int parent = *rit;
			const vector<int> &scc = sccs[parent];

			for (size_t i = 0; i < scc.size(); i++) {
				int x = graphIndexToVar(scc[i]);

				if (result[scc[i] % n] == -1) {
					result[scc[i] % n] = x < 0 ? 1 : 0;
				}
			}
		}

		return true;
	}
};

int main()
{
	ios::sync_with_stdio(false);

	int n, m;
	cin >> n >> m;
	TwoSatisfiability twoSat(n, m);
	for (int i = 0; i < m; ++i) {
		cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
	}

	vector<int> result(n, -1);
	if (twoSat.isSatisfiable(result)) {
		cout << "SATISFIABLE" << endl;
		for (int i = 1; i <= n; ++i) {
			if (result[i-1] == 1) {
				cout << -i;
			} else if (result[i-1] == 0) {
				cout << i;
			} else {
				assert(0);
			}
			if (i < n) {
				cout << " ";
			} else {
				cout << endl;
			}
		}
	} else {
		cout << "UNSATISFIABLE" << endl;
	}

	return 0;
}
