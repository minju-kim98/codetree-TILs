#include <iostream>
#include <vector>
#include <set>
#include <deque>

using namespace std;

struct Knight {
	int num, r, c, h, w, k, damage;
};

int L, N, Q;
vector<vector<int>> map;
vector<vector<int>> knightMap;
vector<Knight> knights;
vector<Knight> toDamage;
int direct[4][2] = {
	-1, 0,
	0, 1, 
	1, 0,
	0, -1,
};

bool isInMap(int r, int c, int h, int w) {
	return r >= 0 && r + h <= L && c >= 0 && c + w <= L;
}

void moveKnights(int num, int d) {
	// 체스판에서 사라진 기사에게 명령을 내리면 아무 반응 없음
	if (knights[num].k <= 0) return;
	
	set<int> toMove;
	deque<int> toCheck;
	vector<int> toPush;
	toMove.insert(num);
	toCheck.push_back(num);
	toPush.push_back(num);
	// 상하좌우 중 한칸 이동
	
	while (!toCheck.empty()) {
		Knight now = knights[toCheck.front()];
		toCheck.pop_front();
		now.r += direct[d][0];
		now.c += direct[d][1];

		if (!isInMap(now.r, now.c, now.h, now.w)) {
			return;
		}

		for (int i = now.r; i < now.r + now.h; i++) {
			for (int j = now.c; j < now.c + now.w; j++) {
				if (map[i][j] == 2) {
					return;
				}

				if (knightMap[i][j] != now.num && knightMap[i][j] != 0) {
					if (toMove.find(knightMap[i][j]) == toMove.end()) {
						toMove.insert(knightMap[i][j]);
						toPush.push_back(knightMap[i][j]);
						toCheck.push_back(knightMap[i][j]);
					}
				}
			}
		}
	}

	for (int i = toPush.size() - 1; i >= 0; i--) {
		int knum = toPush[i];
		Knight now = knights[knum];
		for (int i = now.r; i < now.r + now.h; i++) {
			for (int j = now.c; j < now.c + now.w; j++) {
				knightMap[i][j] = 0;
			}
		}

		knights[knum].r += direct[d][0];
		knights[knum].c += direct[d][1];

		now = knights[knum];
		for (int i = now.r; i < now.r + now.h; i++) {
			for (int j = now.c; j < now.c + now.w; j++) {
				knightMap[i][j] = now.num;
			}
		}
	}

	for (auto const &i : toMove) {
		toDamage.push_back(knights[i]);
	}
	
}

void getDamage(int num) {
	for (int k = 0; k < toDamage.size(); k++) {
		Knight now = toDamage[k];
		if (now.num == num) continue;
		int damageCnt = 0;
		for (int i = now.r; i < now.r + now.h; i++) {
			for (int j = now.c; j < now.c + now.w; j++) {
				if (map[i][j] == 1)
					damageCnt++;
			}
		}

		knights[now.num].damage += damageCnt;
		knights[now.num].k -= damageCnt;

		if (knights[now.num].k <= 0) {
			for (int i = now.r; i < now.r + now.h; i++) {
				for (int j = now.c; j < now.c + now.w; j++) {
					knightMap[i][j] = 0;
				}
			}
		}
	}

	toDamage.clear();
}

int main() {
	cin >> L >> N >> Q;
	map.assign(L, vector<int>(L, 0));
	knightMap.assign(L, vector<int>(L, 0));

	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cin >> map[i][j];
		}
	}

	knights.push_back({ 0, 0, 0, 0, 0, 0, 0 });
	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		r--;
		c--;
		knights.push_back({ i, r, c, h, w, k, 0 });
		for (int j = r; j < r + h; j++) {
			for (int k = c; k < c + w; k++) {
				knightMap[j][k] = i;
			}
		}
	}


	for (int tc = 0; tc < Q; tc++) {
		int num, d;
		cin >> num >> d;

		moveKnights(num, d);
		getDamage(num);

	}

	int result = 0;
	for (int i = 1; i <= N; i++) {
		if (knights[i].k > 0) {
			result += knights[i].damage;
		}
	}
	cout << result;

	return 0;
}