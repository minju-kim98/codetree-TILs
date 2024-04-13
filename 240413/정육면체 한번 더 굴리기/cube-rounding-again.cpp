#include <iostream>
#include <queue>
using namespace std;

struct cord{
	int y;
	int x;
};

struct dice {
	int T;
	int F;
	int R;
	int dir;
};

int N, M;
int nowy, nowx;
int map[20][20];
int visited[20][20];
int result;
dice d = { 1, 2, 3, 0 };

int direct[4][2] = {
	0, 1,
	1, 0,
	0, -1,
	-1, 0
};

void bfs(cord start) {
	queue<cord> q;
	q.push(start);
	visited[start.y][start.x] = 1e9;
	int cnt = 1;

	while (!q.empty()) {
		cord now = q.front();
		q.pop();
		
		for (int i = 0; i < 4; i++) {
			int dy = now.y + direct[i][0];
			int dx = now.x + direct[i][1];

			if (dy >= N || dx >= N || dy < 0 || dx < 0) continue;
			if (visited[dy][dx])	continue;
			if (map[now.y][now.x] != map[dy][dx])	continue;
			visited[dy][dx] = 1e9;
			cnt++;
			q.push({ dy, dx });
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if(visited[i][j] == 1e9)
				visited[i][j] = map[start.y][start.x] * cnt;
		}
	}
	
}

void rollDice() {
	int nexty = nowy + direct[d.dir][0];
	int nextx = nowx + direct[d.dir][1];
	if (nexty >= N || nextx >= N || nexty < 0 || nextx < 0) {
		d.dir = (d.dir + 2) % 4;
		nexty = nowy + direct[d.dir][0];
		nextx = nowx + direct[d.dir][1];
	}

	int T = d.T; int F = d.F; int R = d.R;
	if (d.dir == 0) {
		d.T = 7 - R;
		d.F = F;
		d.R = T;
	}
	else if (d.dir == 1) {
		d.T = 7 - F;
		d.F = T;
		d.R = R;
	}
	else if (d.dir == 2) {
		d.T = R;
		d.F = F;
		d.R = 7 - T;
	}
	else {
		d.T = F;
		d.F = 7 - T;
		d.R = R;
	}

	result += visited[nexty][nextx];

	int down = 7 - d.T;
	if (down > map[nexty][nextx]) {
		d.dir = (d.dir + 1) % 4;
	}
	else if (down < map[nexty][nextx]) {
		d.dir = (d.dir - 1) % 4;
	}

	nowy = nexty;
	nowx = nextx;

}

int main() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!visited[i][j])
				bfs({ i, j });
		}
	}

	for (int i = 0; i < M; i++) {
		rollDice();
	}

	cout << result;
	return 0;
}