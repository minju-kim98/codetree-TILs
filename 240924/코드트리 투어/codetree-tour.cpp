#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

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
priority_queue<Product> products;
unordered_set<int> erased;
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
			products.push({ id, revenue, dest, cost });
		}
		else if (inst == 300) {
			int id;
			cin >> id;
			erased.insert(id);
		}
		else if (inst == 400) {
			if (products.empty()) cout << -1 << endl;
			else {
				int id = products.top().id;
				int dest = products.top().dest;
				int cost = products.top().cost;

				while (!erased.empty() && erased.find(id) != erased.end()) {

					products.pop();

					if (products.empty()) break;

					id = products.top().id;
					dest = products.top().dest;
					cost = products.top().cost;
				}

				vector<Product> tmp;

				while (!products.empty()) {
					int revenue = products.top().revenue;
					cost = products.top().cost;

					if (cost == 1e9 || revenue < cost) {
						tmp.push_back(products.top());
						products.pop();
					}
					else break;
				}

				if (products.empty()) cout << -1 << endl;
				else {
					cout << products.top().id << endl;
					products.pop();
				}

				for (auto i : tmp) {
						products.push(i);
					}
			}
		}
		else if (inst == 500) {
			cin >> start;
			bfs();
			
			priority_queue<Product> tmp;

			while (!products.empty()) {
				int id = products.top().id;

				if (erased.find(id) == erased.end()) {
					Product p = products.top();
					p.cost = visited[p.dest];
					tmp.push(p);
				}
				products.pop();
			}

			products = tmp;
		}
	}


	return 0;
}