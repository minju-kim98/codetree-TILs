#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
struct Node {
	int y, x;
};

struct Santa {
	int id;
	Node position;
	int life;
	bool isAlive;
	int score;

	bool operator < (Santa next) {
		return id < next.id;
	}
};

struct Dist {
	int y, x;
	int distance;

	bool operator < (Dist next) {
		if (distance != next.distance) return distance < next.distance;
		if (y != next.y) return y > next.y;
		return x > next.x;
	}
};

int N, M, P, C, D;
Node rudolf;
vector<vector<int>> map;
vector<Santa> santas;

int rdist[8][2] = {
	-1, -1,
	-1, 0, 
	-1, 1,
	0, -1, 
	0, 1,
	1, -1,
	1, 0,
	1, 1,
};
int dist[4][2] = {
	-1, 0,
	0, 1,
	1, 0,
	0, -1,
};

int getDistance(Node r, Node s) {
	return (r.y - s.y) * (r.y - s.y) + (r.x - s.x) * (r.x - s.x);
}

bool play() {
	// 루돌프의 움직임
	vector<Dist> v;
	for (int i = 0; i < P; i++) {
		if (santas[i].isAlive) {
			int d = getDistance(rudolf, santas[i].position);
			v.push_back({ santas[i].position.y, santas[i].position.x, d});
		}
	}

	sort(v.begin(), v.end());
	Node nearest = { v[0].y, v[0].x };

	int minDirect = 0;
	int minDist = v[0].distance;
	for (int i = 0; i < 8; i++) {
		int dy = rudolf.y + rdist[i][0];
		int dx = rudolf.x + rdist[i][1];

		if (dy < 0 || dy >= N || dx < 0 || dx >= N) continue;

		int d = getDistance({ dy, dx }, nearest);
		if (d < minDist) {
			minDirect = i;
			minDist = d;
		}
	}

	int dy = rudolf.y + rdist[minDirect][0];
	int dx = rudolf.x + rdist[minDirect][1];

	if (map[dy][dx] > 0) {
		//루돌프가 움직여서 산타 충돌
		int s = map[dy][dx] - 1;
		// C만큼 점수를 얻음
		santas[s].score += C;
		// 루돌프가 이동해온 방향으로 C칸만큼 밀려남
		int ny = santas[s].position.y + C * rdist[minDirect][0];
		int nx = santas[s].position.x + C * rdist[minDirect][1];

		if (ny < 0 || ny >= N || nx < 0 || nx >= N) {
			map[santas[s].position.y][santas[s].position.x] = 0;
			santas[s].isAlive = false;
		}
		else {
			if (map[ny][nx] != 0) {
				// 상호작용 일어남
				// 해당 산타도 minDirect 방향으로 1칸 밀려나고...
				// 연쇄적으로 1칸씩 밀려 나가기 반복
				// 게임판 밖으로 밀려 나간 산타는 게임에서 탈락
				int my = ny;
				int mx = nx;
				while(my >= 0 && my < N && mx >= 0 && mx < N && map[my][mx] != 0) {
					my += rdist[minDirect][0];
					mx += rdist[minDirect][1];
				}
				while (my != ny || mx != nx) {
					int tmpy = my - rdist[minDirect][0];
					int tmpx = mx - rdist[minDirect][1];

					int nowS = map[tmpy][tmpx] - 1;
					if (my < 0 || my >= N || mx < 0 || mx >= N) {
						santas[nowS].isAlive = false;
						my = tmpy;
						mx = tmpx;
						continue;
					}
					
					map[my][mx] = map[tmpy][tmpx];
					santas[nowS].position = { my, mx };
					
					my = tmpy;
					mx = tmpx;
				}
				map[ny][nx] = 0;

			}
			map[santas[s].position.y][santas[s].position.x] = 0;
			map[ny][nx] = s + 1;
			santas[s].position = { ny, nx };
			// 충돌로 인한 산타 기절
			santas[s].life = 2;
		}
	}
	map[rudolf.y][rudolf.x] = 0;
	map[dy][dx] = -1;
	rudolf = { dy, dx };

	// 산타의 움직임
	for (int i = 0; i < P; i++) {
		// 기절했거나 이미 탈락한 산타는 움직일 수 없음
		if (!santas[i].isAlive || santas[i].life > 0) continue;
		// 루돌프에게 거리가 가장 가까워지는 방향으로 1칸 이동
		int distance = getDistance(santas[i].position, rudolf);
		int minIdx = -1;
		int minDist = distance;
		for (int k = 0; k < 4; k++) {
			int dy = santas[i].position.y + dist[k][0];
			int dx = santas[i].position.x + dist[k][1];

			// 다른 산타가 있는 칸이나 게임판 밖으로는 움직일 수 없음
			if (dy < 0 || dy >= N || dx < 0 || dx >= N) continue;
			if (map[dy][dx] > 0) continue;

			int d = getDistance({ dy, dx }, rudolf);
			if (d < minDist) {
				minIdx = k;
				minDist = d;
			}
		}

		// 움직일 수 있는 칸이 없거나, 있더라도 가까워질 방법이 없으면 움직이지 않음
		if (minIdx == -1) continue;
		// 상하좌우 인접한 4방향 중 한 곳으로 움직임
		int dy = santas[i].position.y + dist[minIdx][0];
		int dx = santas[i].position.x + dist[minIdx][1];

		// 산타가 움직여서 루돌프와 충돌 시
		if (map[dy][dx] == -1) {
			// 이동해온 반대방향으로 D칸만큼 밀려남
			santas[i].score += D;
			int idx = (minIdx + 2) % 4;
			int ny = dy + D * dist[idx][0];
			int nx = dx + D * dist[idx][1];

			if (ny < 0 || ny >= N || nx < 0 || nx >= N) {
				map[santas[i].position.y][santas[i].position.x] = 0;
				santas[i].isAlive = false;
			}
			else {
				if (map[ny][nx] != 0) {
					// 상호작용
					int my = ny;
					int mx = nx;
					while (my >= 0 && my < N && mx >= 0 && mx < N && map[my][mx] != 0) {
						my += dist[idx][0];
						mx += dist[idx][1];
					}
					
					while (my != ny || mx != nx) {
						int tmpy = my - dist[idx][0];
						int tmpx = mx - dist[idx][1];

						int nowS = map[tmpy][tmpx] - 1;

						if (my < 0 || my >= N || mx < 0 || mx >= N) {
							santas[nowS].isAlive = false;
							my = tmpy;
							mx = tmpx;
							continue;
						}

						map[my][mx] = map[tmpy][tmpx];
						santas[nowS].position = { my, mx };
						my = tmpy;
						mx = tmpx;
					}
				}
				map[santas[i].position.y][santas[i].position.x] = 0;
				map[ny][nx] = i + 1;
				santas[i].position = { ny, nx };
				// 충돌로 인한 산타 기절
				santas[i].life = 2;
			}
		}
		else {
			map[santas[i].position.y][santas[i].position.x] = 0;
			map[dy][dx] = i + 1;
			santas[i].position = { dy, dx };
		}
	}

	// 매 턴 이후 아직 탈락하지 않은 산타들에게는 1점씩을 추가로 부여
	bool isDone = true;
	for (int i = 0; i < P; i++) {
		if (santas[i].isAlive) {
			santas[i].score++;
			if (santas[i].life > 0) 
				santas[i].life--;
			isDone = false;
			
		}
	}

	return isDone;
}

int main() {
	cin >> N >> M >> P >> C >> D;
	map.assign(N, vector<int>(N, 0));
	int rr, rc;
	cin >> rr >> rc;
	rr--;
	rc--;
	rudolf = { rr, rc };
	map[rr][rc] = -1;
	for (int i = 0; i < P; i++) {
		int p, sr, sc;
		cin >> p >> sr >> sc;
		sr--;
		sc--;
		santas.push_back({ p, { sr, sc }, 0, true, 0 });
		map[sr][sc] = p;
	}

	sort(santas.begin(), santas.end());
	
	for (int i = 0; i < M; i++) {
		bool isDone = play();
		if (isDone) break;
	}

	for (int i = 0; i < P; i++) {
		cout << santas[i].score << " ";
	}

	return 0;
}