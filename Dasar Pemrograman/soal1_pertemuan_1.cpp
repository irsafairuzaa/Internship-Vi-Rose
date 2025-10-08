#include <iostream>
using namespace std;

int main(){
    int p, q;
    cin >>p ;
    cin >>q ;

    int x;
    x = (p*p) + (q*q) + 1;

    if (x % 4 == 0){
        cout << x/4;
    }else{
        cout << -1;
    }
}