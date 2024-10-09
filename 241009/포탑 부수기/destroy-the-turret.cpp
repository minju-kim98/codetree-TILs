#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Node {
	int y, x;
	bool operator == (Node next) const {
		return y == next.y && x == next.x;
	}
};

struct NodeHash {
	size_t operator()(Node next) const {
		return hash<int>()(next.y) ^ hash<int>()(next.x);
	}
};

struct Tower {
	int number;
	Node p;
	int power;
	int recent;

	bool operator < (Tower next) const {
		if (power != next.power) return power < next.power;
		if (recent != next.recent) return recent > next.recent;
		if (p.y + p.x != next.p.y + next.p.x) return p.y + p.x > next.p.y + next.p.x;
		return p.x > next.p.x;
	}
};

int N, M, K;
vector<vector<int>> map;
vector<vector<int>> towerMap;
unordered_map<Node, Node, NodeHash> prevNode;
vector<vector<int>> visited;
deque<Tower> towers;
int direct[4][2] = {
	0, 1,
	1, 0,
	0, -1,
	-1, 0,
};
int allDirect[8][2] = {
	-1, -1,
	-1, 0,
	-1, 1,
	0, -1,
	0, 1,
	1, -1,
	1, 0,
	1, 1,
};

void bfs(Node start, Node finish) {
	queue<Node> q;
	visited[start.y][start.x] = 1;
	q.push(start);

	while (!q.empty()) {
		Node now = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int dy = (now.y + direct[i][0] + N) % N;
			int dx = (now.x + direct[i][1] + M) % M;

			if (map[dy][dx] == 0) continue;
			if (visited[dy][dx]) continue;

			prevNode[{dy, dx}] = { now.y, now.x };
			visited[dy][dx] = 1;

			if (dy == finish.y && dx == finish.x) return;
			q.push({ dy, dx });
		}
	}
}

int main() {
	cin >> N >> M >> K;
	map.assign(N, vector<int>(M, 0));
	towerMap.assign(N, vector<int>(M, -1));

	int idx = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> map[i][j];
			towers.push_back({idx, {i, j}, map[i][j], 0});
			idx++;
		}
	}

	// 턴이 총 K번 반복
	for (int turn = 0; turn < K; turn++) {
		sort(towers.begin(), towers.end());
		int zeroCnt = 0;
		for (int i = 0; i < towers.size(); i++) {
			if (towers[i].power <= 0) zeroCnt++;
		}
		for (int i = 0; i < zeroCnt; i++) {
			towers.pop_front();
		}
		// 부서지지 않은 포탑이 1개가 된다면 그 즉시 종료
		if (towers.size() == 1) break;

		towerMap.assign(N, vector<int>(M, -1));
		for (int i = 0; i < towers.size(); i++) {
			Tower now = towers[i];
			towerMap[now.p.y][now.p.x] = i;
		}

		// 1. 공격자 선정
		// 가장 약한 포탑이 공격자로 선정 => tower.front()
		// 공격력이 N + M만큼 증가
		Node weak = towers.front().p;

		towers.front().power += (N + M);
		int power = towers.front().power;
		map[weak.y][weak.x] = power;
		towers.front().recent++;

		// 2. 공격자의 공격
		// 가장 강한 포탑 공격 => tower.back()
		Node strong = towers.back().p;

		visited.assign(N, vector<int>(M, 0));
		unordered_set<Node, NodeHash> damaged;
		
		bfs(weak, strong);

		if (visited[strong.y][strong.x]) {
			// 2-1. 레이저 공격
			Node now = strong;
			while (now.y != weak.y || now.x != weak.x) {
				damaged.insert(now);
				now = prevNode[now];
			}

			for (auto d : damaged) {
				
				if (d == weak) continue;
				if (d == strong) {
					towers.back().power = max(0, towers.back().power - power);
					map[d.y][d.x] = towers.back().power;
					continue;
				}
				int t = towerMap[d.y][d.x];
				towers[t].power = max(0, towers[t].power - power / 2);
				map[d.y][d.x] = towers[t].power;
			}
		}
		else {
			// 2-2 포탄 공격
			towers.back().power = max(0, towers.back().power - power);
			map[strong.y][strong.x] = towers.back().power;

			for (int i = 0; i < 8; i++) {
				int dy = (strong.y + allDirect[i][0] + N) % N;
				int dx = (strong.x + allDirect[i][1] + M) % M;
				
				if (map[dy][dx] == 0) continue;
				int t = towerMap[dy][dx];
				towers[t].power = max(0, towers[t].power - power / 2);
				map[dy][dx] = towers[t].power;
				damaged.insert({ dy, dx });
			}
		}

		// 4. 포탑 정비
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				// 부서진 포탑
				if (map[i][j] == 0) continue;
				if (towers.front().p.y == i && towers.front().p.x == j) continue;
				// 부서지지 않은 포탑 중 공격과 무관했던 포탑
				if (damaged.find({ i, j }) == damaged.end()) {
					int t = towerMap[i][j];
					map[i][j]++;
					towers[t].power++;
				}
			}
		}
	}

	sort(towers.begin(), towers.end());
	cout << towers.back().power;

	return 0;
}