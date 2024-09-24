#include <iostream>
#include <vector>
#include <unordered_map>
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

vector<vector<Edge>> cities;
priority_queue<Product> products;
unordered_map<int, int> isValid;
int N, M, start;
vector<int> visited;

void bfs() {
	priority_queue<Edge> pq;
	visited.assign(N, 1e9);  // 초기 값을 매우 큰 값으로 설정
	visited[start] = 0;  // 시작점은 비용이 0
	pq.push({start, 0});  // 우선순위 큐에 시작점 추가

	while (!pq.empty()) {
		Edge nowEdge = pq.top();
		pq.pop();
		int now = nowEdge.to;
		int nowCost = nowEdge.value;

		if (visited[now] < nowCost)
			continue;

		for (Edge nextEdge : cities[now]) {
			int next = nextEdge.to;
			int nextCost = nextEdge.value;

			if (visited[next] > visited[now] + nextCost) {
				visited[next] = visited[now] + nextCost;
				pq.push({next, visited[next]});
			}
		}
	}
}

int main() {
	int Q;
	cin >> Q;
	for (int i = 0; i < Q; i++) {
		int inst;
		cin >> inst;

		if (inst == 100) {  // 도시와 도로 입력
			cin >> N >> M;
			cities.assign(N, vector<Edge>());
			for (int i = 0; i < M; i++) {
				int v, u, w;
				cin >> v >> u >> w;
				cities[v].push_back({u, w});
				cities[u].push_back({v, w});
			}
			bfs();  // BFS를 통해 모든 도시의 최단 거리를 계산
		}
		else if (inst == 200) {  // 새로운 제품 추가
			int id, revenue, dest, cost;
			cin >> id >> revenue >> dest;
			cost = visited[dest];  // 목적지까지의 최단 거리 비용 설정
			products.push({id, revenue, dest, cost});
			isValid[id] = 1;
		}
		else if (inst == 300) {  // 제품 무효화
			int id;
			cin >> id;
			if (isValid.find(id) != isValid.end()) {
				isValid[id] = 0;  // 해당 제품 무효화
			}
		}
		else if (inst == 400) {  // 최적의 제품 선택
			while (!products.empty() && isValid[products.top().id] == 0) {
				products.pop();  // 무효화된 제품은 삭제
			}

			if (products.empty()) {
				cout << -1 << endl;  // 유효한 제품이 없을 때
			} else {
				int id = products.top().id;
				int revenue = products.top().revenue;
				int cost = products.top().cost;

				// 비용이 너무 높아서 수익을 내지 못하는 경우 제거
				if (cost == 1e9 || revenue < cost) {
					cout << -1 << endl;
				} else {
					cout << id << endl;  // 최적의 제품 출력
					products.pop();
				}
			}
		}
		else if (inst == 500) {  // 출발 도시 변경
			cin >> start;
			bfs();  // 새로운 출발점으로 최단 거리 갱신

			priority_queue<Product> tmp;

			while (!products.empty()) {
				int id = products.top().id;

				if (isValid[id] == 1) {
					Product p = products.top();
					p.cost = visited[p.dest];  // 비용 갱신
					tmp.push(p);
				}
				products.pop();
			}

			products = tmp;  // 갱신된 큐로 교체
		}
	}
	return 0;
}