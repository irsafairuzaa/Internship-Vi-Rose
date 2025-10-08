#include <iostream>
#include <string.h>
using namespace std;

int main(){
    string huruf;
    cin >> huruf;

    int x;
    x = huruf.length();
    for (int i = 0; i < x; i++){
        if (huruf[i] >= 'a' && huruf[i] <= 'z'){
            huruf[i] -= 32;
        }else {
            huruf[i] += 32;
        }
    }
    cout << huruf;

    return 0;
}