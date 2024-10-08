#include <bits/stdc++.h>
#define endl '\n'
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;
typedef pair<ll, ll> pll;
typedef tuple<int,int,int> tiii;
const int INF=1e9+1;
int dx[]={1,-1,0,0}, dy[]={0,0,1,-1};

struct runner {
    // 러너들의 위치, 탈출 저장
    int x,y;
    bool goal;
};

bool isfinished(vector<runner>& runners) {
    // 모든 러너들이 탈출했는지 체크하는 함수
    for(auto it : runners) {
        if(!it.goal) return false;
    }
    return true;
}

tiii findSquare(vector<vector<int>>& board, vector<runner>& runners, pii exit) {
    // 가장 작은 사각형의 범위를 찾는 함수
    // 리턴 : (square start x, square start y, sqaure size)
    int size = 1;
    vector<vector<int>> tmp(board.size(),vector<int>(board.size()));
    for(auto r : runners) {
        if(r.goal) continue;
        tmp[r.x][r.y]=1;
    }
    while((++size)<board.size()) {
        for(int r=1;r<=board.size()-size;r++) {
            for(int c=1;c<=board.size()-size;c++) {
                // 러너가 포함되었는지, 탈출구가 포함되었는지
                bool findRunner=0, findExit=0;
                for(int x=r;x<r+size;x++) {
                    for(int y=c;y<c+size;y++) {
                        if(x==exit.first && y==exit.second) findExit=1;
                        if(tmp[x][y]) findRunner=1;
                    }
                }
                if(findRunner && findExit) return {r,c,size};
            }
        }
    }
    return {0,0,0};
}

void rotate(vector<vector<int>>& board, vector<runner>& runners, pii& exit) {
    // 회전시키는 함수
    auto [startx,starty,size] = findSquare(board,runners,exit);
    // tmp는 회전시키기 위한 임시공간, rot은 러너들의 위치를 회전시키기 위해 저장
    vector<vector<int>> tmp(size, vector<int>(size));
    map<pii,pii> rot;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            rot[{startx+size-j-1,starty+i}]={startx+i,starty+j};
            tmp[i][j]=board[startx+size-j-1][starty+i];
        }
    }
    // 범위 내에 있는 러너들과 탈출구를 회전시킨다
    for(auto& [x,y,g] : runners) {
        if(!g && rot.find({x,y})!=rot.end()) {
            pii rotated=rot[{x,y}];
            x=rotated.first, y=rotated.second;
        }
    }
    exit=rot[exit];
    // board를 회전시키고 벽의 내구도를 1 감소시킨다
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) board[startx+i][starty+j]=tmp[i][j]>0 ? tmp[i][j]-1 : tmp[i][j];
    }
    return;
}

int move(vector<vector<int>>& board, vector<runner>& runners, pii exit) {
    // 러너들이 움직이는 함수
    int dist=0;
    for(auto& r : runners) {
        if(r.goal) continue;
        for(int i=0;i<4;i++) {
            int nx=r.x+dx[i], ny=r.y+dy[i];
            if(nx>=1 && nx<board.size() && ny>=1 && ny<board.size() && !board[nx][ny]) {
                if(exit.first==nx && exit.second==ny) {
                    r.goal=1;
                    dist++;
                    break;
                }
                if(abs(exit.first-nx)+abs(exit.second-ny)==abs(exit.first-r.x)+abs(exit.second-r.y)-1) {
                    // 거리가 1작은 경우 움직인다
                    r.x=nx, r.y=ny;
                    dist++;
                    break;
                }
            }
        }
    }
    return dist;
}

int main() {
    fastio
    //freopen("input.txt","r",stdin);
    int n,m,k; cin>>n>>m>>k;
    vector<vector<int>> board(n+1,vector<int>(n+1));
    for(int i=1;i<=n;i++) {
        for(int j=1;j<=n;j++) cin>>board[i][j];
    }
    vector<runner> runners(m);
    for(auto& it : runners) {
        cin>>it.x>>it.y;
        it.goal=0;
    }
    pii exit; cin>>exit.first>>exit.second;
    int totalDistance=0;
    while(k--) {
        totalDistance+=move(board,runners,exit);
        if(isfinished(runners)) break;
        rotate(board,runners,exit);
    }
    cout<<totalDistance<<endl;
    cout<<exit.first<<' '<<exit.second<<endl;
    return 0;
}