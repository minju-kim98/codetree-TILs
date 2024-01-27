#include <iostream>
#include <vector>
#include <string>
using namespace std;

int N;
vector<int> v;

int main() {
    cin >> N;
    for(int i = 0; i < N; i++){
        string command;
        int num;
        cin >> command;

        if(command == "push_back"){
            cin >> num;
            v.push_back(num);
        }
        else if(command == "pop_back"){
            int n = v.back();
            v.pop_back();
        }
        else if(command == "size"){
            cout << v.size() << "\n";
        }
        else if(command == "get"){
            cin >> num;
            cout << v[num - 1] << "\n";
        }
    }
    return 0;
}