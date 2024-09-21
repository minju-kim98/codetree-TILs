#include <iostream>
#include <queue>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

struct Node { int y, x; };
struct Rotate {
	int value, angle, y, x;

	bool operator < (Rotate Next) const{
		if (value > Next.value) return true;
		if (value < Next.value) return false;
		
		if (angle < Next.angle) return true;
		if (angle > Next.angle) return false;

		if (x < Next.x) return true;
		if (x > Next.x) return false;

		return y < Next.y;
	}
};

int K, M;
int N = 5;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<vector<int>> arr;
vector<vector<int>> visited;
vector<Rotate> v;
vector<int> toErase;
deque<int> wall;
int direct[4][2] = {
	-1, 0,
	1, 0,
	0, -1,
	0, 1,
};


int bfs(Node start, int idx) {
	queue<Node> q;
	visited[start.y][start.x] = idx;
	q.push(start);

	while (!q.empty()) {
		Node now = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int dy = now.y + direct[i][0];
			int dx = now.x + direct[i][1];

			if (dy < 0 || dy >= N || dx < 0 || dx >= N) continue;
			if (arr[dy][dx] != arr[now.y][now.x]) continue;
			if (visited[dy][dx]) continue;

			visited[dy][dx] = idx;
			q.push({ dy, dx });
		}
	}

	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (visited[i][j] == idx) cnt++;
		}
	}

	return cnt;
}

void rotate90(int y, int x) {
	vector<vector<int>> tmp = arr;
	arr[y - 1][x - 1] = tmp[y + 1][x - 1];
	arr[y - 1][x] = tmp[y][x - 1];
	arr[y - 1][x + 1] = tmp[y - 1][x - 1];
	arr[y][x - 1] = tmp[y + 1][x];
	arr[y][x + 1] = tmp[y - 1][x];
	arr[y + 1][x - 1] = tmp[y + 1][x + 1];
	arr[y + 1][x] = tmp[y][x + 1];
	arr[y + 1][x + 1] = tmp[y - 1][x + 1];
}

int getValue() {
	visited.assign(5, vector<int>(5, 0));
	int idx = 1;
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!visited[i][j]) {
				int val = bfs({ i, j }, idx);
				if (val >= 3) { 
					cnt += val; 
					toErase.push_back(idx);
				}
				idx++;
			}
		}
	}

	return cnt;
}

int removeAndFill(int angle, int y, int x) {
	int allCnt = 0;
	arr = map;
	for (int i = 0; i < angle; i++) {
		rotate90(y, x);
	}

	int cnt = 0;
	do {
		toErase.clear();
		visited.assign(5, vector<int>(5, 0));
		cnt = getValue();

		if (cnt == 0) break;

		allCnt += cnt;

		//remove
		for (int idx : toErase) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (visited[i][j] == idx)
						arr[i][j] = 0;
				}
			}
		}

		//fill
		for (int j = 0; j < N; j++) {
			for (int i = N - 1; i >= 0; i--) {
				if (arr[i][j] == 0) {
					arr[i][j] = wall.front();
					wall.pop_front();
				}
			}
		}

	} while (cnt != 0);

	map = arr;

	return allCnt;
}

int main() {
	cin >> K >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int tmp;
		cin >> tmp;
		wall.push_back(tmp);
	}

	for (int turn = 0; turn < K; turn++) {
		v.clear();

		for (int i = 1; i < N - 1; i++) {
			for (int j = 1; j < N - 1; j++) {
				arr = map;
				for (int k = 1; k <= 3; k++) {
					rotate90(i, j);
					int value = getValue();
					v.push_back({ value, k, i, j });
				}
			}
		}

		sort(v.begin(), v.end());

		int get = removeAndFill(v[0].angle, v[0].y, v[0].x);

		if (get == 0) break;

		cout << get << " ";
	}

	return 0;
}