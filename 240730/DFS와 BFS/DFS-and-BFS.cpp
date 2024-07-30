#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int N, M, start;
vector<vector<int>> v;
int used[1001];
int printed[1001];

void dfs(int now) {

	cout << now << " ";

	for (int i = 0; i < v[now].size(); i++) {
		int next = v[now][i];
		if (used[next] == 1) continue;
		used[next] = 1;
		dfs(next);
	}
}

void bfs() {
	for (int i = 0; i < 1001; i++)	used[i] = 0;
	queue<int> q;

	used[start] = 1;
	q.push(start);

	while (!q.empty()) {
		int now = q.front();
		q.pop();
		
		cout << now << " ";

		for (int i = 0; i < v[now].size(); i++) {
			int next = v[now][i];
			if (used[next] == 1) continue;
			q.push(next);
			used[next] = 1;
		}
	}
}

int main() {
	cin >> N >> M >> start;
	v.resize(N + 1);

	for (int i = 0; i < M; i++) {
		int a, b;
		cin >> a >> b;
		v[a].push_back(b);
		v[b].push_back(a);
	}

	for (int i = 0; i < v.size(); i++) {
		sort(v[i].begin(), v[i].end());
	}

	used[start] = 1;
	dfs(start);

	cout << "\n";

	bfs();

	return 0;
}