#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
	int to;
	int value;

	bool operator < (Edge right) const {
		return value > right.value;
	}
};

struct Product {
	int id;
	int revenue;
	int dest;
	int cost;

	bool operator < (Product Next) const {
		if (revenue - cost < Next.revenue - Next.cost) return true;
		if (revenue - cost > Next.revenue - Next.cost) return false;

		return id > Next.id;
	}
};

vector<vector <Edge>> cities;
unordered_map<int, Product> products;
int N, M, start;
vector<int> visited;

void bfs() {
	priority_queue <Edge> pq;
	visited.assign(N, 1e9);
	visited[start] = 0;
	pq.push({start, 0});

	while (!pq.empty()) {
		Edge nowEdge = pq.top();
		pq.pop();
		int now = nowEdge.to;
		int nowCost = nowEdge.value;

		if (visited[now] < nowCost)
			continue;

		for (int i = 0; i < cities[now].size(); i++) {
			Edge nextEdge = cities[now][i];
			int next = nextEdge.to;
			int nextCost = nextEdge.value;

			if (visited[next] <= visited[now] + nextCost) continue;

			visited[next] = visited[now] + nextCost;
			pq.push({ next, visited[next] });
		}
	}
}

bool cmp(pair<int, Product>& a, pair<int, Product>& b) {
	return b.second < a.second;
}

int main() {
	int Q;
	cin >> Q;
	for (int i = 0; i < Q; i++) {
		int inst;
		cin >> inst;

		if (inst == 100) {
			cin >> N >> M;
			cities.assign(N, vector<Edge>());
			for (int i = 0; i < M; i++) {
				int v, u, w;
				cin >> v >> u >> w;
				cities[v].push_back({u, w});
				cities[u].push_back({v, w});
			}

			bfs();
		}
		else if (inst == 200) {
			int id, revenue, dest, cost;
			cin >> id >> revenue >> dest;
			cost = visited[dest];
			products[id] = { id, revenue, dest, cost };
		}
		else if (inst == 300) {
			int id;
			cin >> id;
			if (products.find(id) != products.end()) {
				products.erase(id);
			}
		}
		else if (inst == 400) {
			int answer = -1;
			vector<pair<int, Product>> v(products.begin(), products.end());
			sort(v.begin(), v.end(), cmp);

			for (auto const &elem : v) {
				if (elem.second.cost == 1e9 || elem.second.revenue < elem.second.cost)
					continue;
				else {
					answer = elem.first;
					products.erase(elem.first);
					break;
				}
			}

			cout << answer << endl;
		}
		else if (inst == 500) {
			cin >> start;
			bfs();
			
			for (auto &p : products) {
				p.second.cost = visited[p.second.dest];
			}
		}
	}


	return 0;
}