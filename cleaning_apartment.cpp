#include <bits/stdc++.h>

using namespace std;

struct Edge {
	int from;
	int to;
};

struct ConvertHampathToSat {
	int numVertices;
	vector<Edge> edges;

	ConvertHampathToSat(int n, int m) :
		numVertices(n),
		edges(m)
	{}

	int calc_index(int i, int j)
	{
		return i + j*numVertices + 1;
	}

	void printEquisatisfiableSatFormula()
	{
		stringstream ss;
		int n = numVertices;
		int clauses_num = 0;
		int vars_num = n*n;

		/* each vertex must be visited exactly once */
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				ss << calc_index(i, j) << " ";
			}
			ss << 0 << endl;
			clauses_num++;
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = j+1; k < n; k++) {
					ss << -calc_index(i, j) << " " << -calc_index(i, k) << " " << 0 << endl;
					clauses_num++;
				}
			}
		}

		/* there is only one vertex on each position in the path */
		for (int j = 0; j < n; j++) {
			for (int i = 0; i < n; i++) {
				ss << calc_index(i, j) << " ";
			}
			ss << 0 << endl;
			clauses_num++;
		}
		for (int j = 0; j < n; j++) {
			for (int i = 0; i < n; i++) {
				for (int k = i+1; k < n; k++) {
					ss << -calc_index(i, j) << " " << -calc_index(k, j) << " " << 0 << endl;
					clauses_num++;
				}
			}
		}

		set<pair<int, int> > edge_set;

		for (size_t i = 0; i < edges.size(); i++) {
			int from = edges[i].from - 1;
			int to = edges[i].to - 1;

			edge_set.insert(make_pair(from, to));
			edge_set.insert(make_pair(to, from));
		}

		/* two successive vertices must be connected by an edge */
		for (int i = 0; i < n; i++) {
			for (int j = i+1; j < n; j++) {
				if (edge_set.find(make_pair(i, j)) != edge_set.end()) continue;

				for (int k = 0; k < n-1; k++) {
					ss << -calc_index(i, k) << " " << -calc_index(j, k+1) << " " << 0 << endl;
					clauses_num++;

					ss << -calc_index(j, k) << " " << -calc_index(i, k+1) << " " << 0 << endl;
					clauses_num++;
				}
			}
		}

		cout << clauses_num << " " << vars_num << endl;
		cout << ss.str();
	}
};

int main() {
	ios::sync_with_stdio(false);

	int n, m;
	cin >> n >> m;
	ConvertHampathToSat converter(n, m);

	for (int i = 0; i < m; ++i) {
		cin >> converter.edges[i].from >> converter.edges[i].to;
	}

	converter.printEquisatisfiableSatFormula();

	return 0;
}
