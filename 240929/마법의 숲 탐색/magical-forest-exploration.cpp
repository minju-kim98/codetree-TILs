#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Car { int y, x, d; };
struct Node { int y, x; };

int R, C, K, c, d, result;
int direct[4][2] = {
	-1, 0,
	0, 1,
	1, 0,
	0, -1,
};
bool moving = false;
Car now;
vector<vector<int>> map;

bool moveNorth() { 
	// 남쪽으로 더이상 이동할 수 없을 때 까지 이동
	int dy = now.y;
	int dx = now.x;
	while (dy + 2 < R) {
		if (map[dy + 1][dx - 1] == 0 && map[dy + 2][dx] == 0 && map[dy + 1][dx + 1] == 0) {
			dy++;
			continue;
		}
		else {
			break;
		}
	}

	if (dy == now.y) return false;
	
	now.y = dy;
	now.x = c;
	now.d = d;

	return true;
}

bool moveWest() {
	// 서쪽으로 회전하면서 아래로 한칸
	int dy = now.y;
	int dx = now.x;

	if (dy + 2 >= R || dx - 2 < 0) return false;
	if (map[dy + 1][dx - 2] == 0 && map[dy + 2][dx - 1] == 0) {
		if (dy >= 1 && map[dy - 1][dx - 1] == 0 && map[dy][dx - 2] == 0 && map[dy + 1][dx - 1] == 0) {
			now.y = dy + 1;
			now.x = dx - 1;
			c = dx - 1;
			now.d = (now.d + 3) % 4;
			return true;
		}
		else if (dy < 1) {
			now.y = dy + 1;
			now.x = dx - 1;
			c = dx - 1;
			now.d = (now.d + 3) % 4;
			return true;
		}
		
	}
	
	return false;
}

bool moveEast() {
	// 동쪽으로 회전하면서 아래로 한칸
	int dy = now.y;
	int dx = now.x;

	if (dx + 2 >= C || dy + 2 >= R) return false;
	if (map[dy + 1][dx + 2] == 0 && map[dy + 2][dx + 1] == 0) {
		if (dy >= 1 && map[dy - 1][dx + 1] == 0 && map[dy][dx + 2] == 0 && map[dy + 1][dx + 1] == 0) {
			now.y = dy + 1;
			now.x = dx + 1;
			c = dx + 1;
			now.d = (now.d + 1) % 4;
			return true;
		}
		else if (dy < 1) {
			now.y = dy + 1;
			now.x = dx + 1;
			c = dx + 1;
			now.d = (now.d + 1) % 4;
			return true;
		}
	}

	return false;
}

int moveAngel() {
	// 정령 이동
	vector<vector<int>> visited(R, vector<int>(C, 0));
	queue<Node> q;
	visited[now.y][now.x] = 1;
	q.push({ now.y, now.x });

	while (!q.empty()) {
		Node here = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int dy = here.y + direct[i][0];
			int dx = here.x + direct[i][1];

			if (dy < 0 || dy >= R || dx < 0 || dx >= C) continue;
			if (visited[dy][dx]) continue;
			if (map[dy][dx] == 0) continue;

			if (map[dy][dx] == map[here.y][here.x] + 1) {
				visited[dy][dx] = 1;
				q.push({ dy, dx });
				continue;
			}

			if (map[here.y][here.x] % 2 != 0 && map[dy][dx] != map[here.y][here.x]) continue;

			visited[dy][dx] = 1;
			q.push({ dy, dx });
		}
	}
	
	for (int i = R - 1; i >= 0; i--) {
		for (int j = 0; j < C; j++) {
			if (visited[i][j] == 1)
				return i + 1;
		}
	}


	return 0;
}


int main() {
	cin >> R >> C >> K;
	map.resize(R, vector<int>(C, 0));
	int idx = 1;
	for (int i = 0; i < K; i++) {
		cin >> c >> d;
		c--;
		now.y = 0;
		now.x = c;
		now.d = d;

		moving = false;
		bool moveable = true;
		while (moveable) {
			moveable = false;
			moveable = moveable || moveNorth();
			moving = true;
			if(!moveable)
				moveable = moveable || moveWest();
			if(!moveable)
				moveable = moveable || moveEast();
		}
		
		if (now.y < 1) {
			map.assign(R, vector<int>(C, 0));
			continue;
		}

		map[now.y - 1][now.x] = idx;
		map[now.y][now.x - 1] = idx;
		map[now.y][now.x] = idx;
		map[now.y][now.x + 1] = idx;
		map[now.y + 1][now.x] = idx;

		map[now.y + direct[now.d][0]][now.x + direct[now.d][1]] = idx + 1;

		idx += 2;


		result += moveAngel();
	}

	std::cout << result;

	return 0;
}