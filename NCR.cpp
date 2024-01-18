#include<bits/stdc++.h>
using namespace std;
const int mod = 1e9+7;
int nCr(int n, int r){

    int num = 1;
    for(int i = n - r + 1 ; i <= n ; i++)
    {
        num *= i;   
    }
    for(int i = 1 ; i <= r; i++){
        num /= i;
    }
    return num;
}
int pow(int a, int b){
    int ans = 1;
    while(b){
        if(b&1){
            ans *= a;
            ans %= mod;
        }
        a *= a;
        a %= mod;
        b >>= 1;
    }
    return ans;
}
const int N = 1e5+1;
vector<int>fact(N);
vector<int>invfact(N);
void preFact(){
    fact[0] = 1;
    invfact[0] = 1;
    for(int i = 1 ; i < N ; i++){
        fact[i] = fact[i-1] * i;
        fact[i] %= mod;
        invfact[i] = pow(fact[i], mod-2);
    }

}
int main(){
    int t;
    cin >> t;//10^5
    preFact();
    while(t--){
        int n , r;
        cin >> n >> r;
        cout<<fact[n]/(fact[r]*fact[n-r])<<endl;
        int ans = (fact[n] % mod * invfact[r] % mod * invfact[n-r] %mod)%mod;
        
    }
    
}
