#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Node { 
	int y, x; 
	bool operator == (Node next) const {
		return y == next.y && x == next.x;
	}
};
struct NodeHash {
	size_t operator() (Node next) const {
		return hash<int>()(next.y) ^ hash<int>()(next.x);
	}
};
struct Store {
	Node p;
	int num;
	bool arrived;
};
struct Camp {
	Node p;
	int minVisited;
	bool operator < (Camp next) const {
		if (minVisited != next.minVisited) return minVisited < next.minVisited;
		if (p.y != next.p.y) return p.y < next.p.y;
		return p.x < next.p.x;
	}
};
struct Person {
	Node p;
	bool isDone;
};

int N, M;
vector<vector<int>> map;
vector<Store> stores;
vector<Person> people;

int direct[4][2] = {
	-1, 0,
	0, -1,
	0, 1,
	1, 0,
};

vector<Node> getPath(unordered_map<Node, Node, NodeHash>& prevNode, Node start, Node finish) {
	vector<Node> result;
	Node now = prevNode[finish];
	while (now.y != start.y || now.x != start.x) {
		result.push_back(now);
		now = prevNode[now];
	}
	result.push_back(start);

	return result;
}

vector<Node> bfs(Node start, Node finish) {
	// start - 편의점
	// finish - 베이스캠프
	vector<vector<int>> visited(N, vector<int>(N, 0));
	unordered_map<Node, Node, NodeHash> prevNode;
	queue<Node> q;
	visited[start.y][start.x] = 1;
	q.push(start);

	while (!q.empty()) {
		Node now = q.front();
		q.pop();

		if (now == finish) {
			return getPath(prevNode, start, finish);
		}

		for (int i = 0; i < 4; i++) {
			int dy = now.y + direct[i][0];
			int dx = now.x + direct[i][1];

			if (dy < 0 || dy >= N || dx < 0 || dx >= N) continue;
			if (visited[dy][dx]) continue;
			if (map[dy][dx] == 2 && (finish.y != dy || finish.x != dx)) continue;

			prevNode[{dy, dx}] = now;
			visited[dy][dx] = 1;
			q.push({ dy, dx });
		}
	}

	return getPath(prevNode, start, finish);
}

vector<Camp> bfsCamp(Node start) {
	vector<Camp> result;
	vector<vector<int>> visited(N, vector<int>(N, 0));
	queue<Node> q;
	int minVisited = 1e9;
	visited[start.y][start.x] = 1;
	q.push(start);

	while (!q.empty()) {
		Node now = q.front();
		q.pop();

		if (map[now.y][now.x] == 1 && visited[now.y][now.x] < minVisited) {
			result.push_back({ {now.y, now.x}, visited[now.y][now.x] });
			minVisited = visited[now.y][now.x];
		}

		for (int i = 0; i < 4; i++) {
			int dy = now.y + direct[i][0];
			int dx = now.x + direct[i][1];

			if (dy < 0 || dy >= N || dx < 0 || dx >= N) continue;
			if (visited[dy][dx]) continue;
			if (map[dy][dx] == 2) continue;

			visited[dy][dx] = visited[now.y][now.x] + 1;
			q.push({ dy, dx });
		}
	}

	return result;
}

int main() {
	cin >> N >> M;
	map.assign(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	stores.push_back({ {0, 0}, 0 });
	for (int i = 1; i <= M; i++) {
		int y, x;
		cin >> y >> x;
		y--;
		x--;
		stores.push_back({ {y, x}, i, false });
	}

	int t = 0;
	vector<Node> v;
	people.push_back({ {0, 0}, true });
	while (true) {
		bool isDone = true;
		if (people.size() == M + 1) {
			for (int i = 1; i <= M; i++) {
				if (people[i].isDone == false) {
					isDone = false;
				}
			}
		}
		else {
			isDone = false;
		}

		if (isDone) break;

		t++;

		//1. 본인이 가고 싶은 편의점 방향을 향해서 1칸 움직임
		for (int i = 1; i < people.size(); i++) {
			if (people[i].isDone) continue;
			Node now = people[i].p;
			vector<Node> path = bfs(stores[i].p, now);
			people[i].p = path[0];
		}

		//2. 편의점에 도착했다면 해당 편의점에서 멈추기
		for (int i = 1; i < people.size(); i++) {
			Node now = people[i].p;
			if (now == stores[i].p) {
				stores[i].arrived = true;
				people[i].isDone = true;
				map[now.y][now.x] = 2;
			}
		}

		//3. t번 사람이 베이스 캠프에 들어감
		if (t <= M) {
			vector<Camp> camps = bfsCamp(stores[t].p);
			sort(camps.begin(), camps.end());
			Node minCamp = camps[0].p;
			people.push_back({ {minCamp.y, minCamp.x}, false});
			map[minCamp.y][minCamp.x] = 2;
		}
	}

	cout << t;

	return 0;
}