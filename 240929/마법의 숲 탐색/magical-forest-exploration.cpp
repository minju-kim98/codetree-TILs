#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Car {
    int y, x, d;
};

const int MAX_SIZE = 70;
const int dx[4] = {0, 1, 0, -1};  // North, East, South, West
const int dy[4] = {-1, 0, 1, 0};

int R, C, K;
vector<vector<int>> forest(MAX_SIZE, vector<int>(MAX_SIZE, 0));
int result = 0;

bool isValid(int y, int x) {
    return y >= 0 && y < R && x >= 0 && x < C;
}

bool canMoveSouth(const Car& car) {
    for (int i = -1; i <= 1; i++) {
        if (!isValid(car.y + 2, car.x + i) || forest[car.y + 2][car.x + i] != 0) {
            return false;
        }
    }
    return true;
}

bool canRotate(const Car& car, int dir) {
    int nx = car.x + dx[dir];
    int ny = car.y + dy[dir];
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!isValid(ny + i, nx + j) || forest[ny + i][nx + j] != 0) {
                return false;
            }
        }
    }
    return true;
}

void placeCar(Car& car, int id) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            forest[car.y + i][car.x + j] = id;
        }
    }
    forest[car.y + dy[car.d]][car.x + dx[car.d]] = id + 1;  // Exit
}

int moveFairy(const Car& car) {
    int y = car.y, x = car.x;
    while (isValid(y + 1, x) && forest[y + 1][x] != 0) {
        y++;
    }
    return y;
}

void clearForest() {
    for (int i = 0; i < R; i++) {
        fill(forest[i].begin(), forest[i].end(), 0);
    }
}

bool moveCar(Car& car) {
    while (true) {
        if (canMoveSouth(car)) {
            car.y++;
        } else if (canRotate(car, (car.d + 3) % 4)) {  // Try rotating west
            car.x += dx[(car.d + 3) % 4];
            car.y++;
            car.d = (car.d + 3) % 4;
        } else if (canRotate(car, (car.d + 1) % 4)) {  // Try rotating east
            car.x += dx[(car.d + 1) % 4];
            car.y++;
            car.d = (car.d + 1) % 4;
        } else {
            break;
        }
    }
    return car.y >= 1;  // Car is inside the forest
}

int main() {
    cin >> R >> C >> K;
    if (R < 5 || R > MAX_SIZE || C < 5 || C > MAX_SIZE || K < 1 || K > 1000) {
        cout << "Invalid input" << endl;
        return 1;
    }

    for (int i = 0; i < K; i++) {
        int c, d;
        cin >> c >> d;
        if (c < 2 || c > C - 1 || d < 0 || d > 3) {
            cout << "Invalid input" << endl;
            return 1;
        }

        Car car = {0, c - 1, d};
        if (moveCar(car)) {
            placeCar(car, i * 2 + 1);
            result += moveFairy(car);
        } else {
            clearForest();
        }
    }

    cout << result << endl;
    return 0;
}