#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 1e9+7;
const ll MAXN = 1e6;
ll fac[MAXN];
ll invfact[MAXN];
ll power(ll a, ll b){
    if(b == 0)return 1;
    ll x = power(a , b / 2);
    if(b % 2){
        return (x%mod*x%mod*a%mod) % mod;
    }
    else{
        return (x*x) % mod;
    }
}
ll inverse(ll a){
    return power(a,mod-2);
}
void Compute_factorial(){
    fac[0]=1;
    invfact[0]=1;
    for(int i = 1 ; i < MAXN ; i++){
        fac[i] =(fac[i-1] * i) % mod;
        invfact[i] = inverse(fac[i])%mod;
 
    }
}
ll nCr(ll n , ll k){
    //cout<<"hi";
    if(n < 0 || k < 0 || k > n)return 0;
    ll num = fac[n];
    //cout<<fac[n];
    ll den = (invfact[k]*invfact[n-k])%mod;
    //cout<<invfact[6]<<"\n";
    return (num*den)%mod;
}
int main(){
    ll t;
    cin>>t;
    while(t--){
        ll a , b , c , m;
        cin >> a >> b >> c >> m;
        ll max_pair = (a-1)+(b-1)+(c-1);
        ll A[3];
        A[0]=a,A[1]=b,A[2]=c;
        sort(A,A+3);
        ll min_pair = A[2]-A[1]-A[0]-1;
        if(min_pair <= m and m <= max_pair){
            cout<<"YES\n";
        }
        else{
            cout<<"NO\n";
        }
    }
    return 0;
}