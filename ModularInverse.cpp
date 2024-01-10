#include<bits/stdc++.h>
using namespace std;
int mod = 1e9+7;
int BE(int a, int b){
    int ans = 1;
    while(b){
        if(b&1){
            b = b - 1;
            ans *= a;
        }
        else{
            b = b / 2;
            a = a * a;

        }
    }
    return ans;
}
int inverse(int b){
    int x = BE(b,mod-2);
}
int main()
{
    int a, b;
    cin >> a >> b;
    int ans = BE(a,b);
    cout<<inverse(b);
}
