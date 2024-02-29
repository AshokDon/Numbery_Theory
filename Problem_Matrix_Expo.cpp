//https://www.spoj.com/problems/SEQ/


#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define MOD 1000000000
#define pb push_back
ll k , n;

class Matrix{
    public:
    ll n , m;
    vector<vector<ll>>a;
    Matrix(ll n , ll m){
        this->n = n;
        this->m = m;
        a.resize(n,vector<ll>(m,0));
    }
};
Matrix Multiply(Matrix A, Matrix B){
    Matrix C(k,k);
    for(int i = 0 ; i < k ; i++){
        for(int j = 0 ; j < k ; j++){
            ll sum = 0;
            for(int x = 0 ; x < k ; x++){
                sum = (sum + (A.a[i][x] * B.a[x][j])%MOD)%MOD;
            }
            C.a[i][j] = sum;
        }
    }
    return C;
}
Matrix Identity(){
    Matrix I(k,k);
    for(int i = 0 ; i < k ; i++){
        I.a[i][i] = 1;
    }
    return I;
}
Matrix power(Matrix A, ll p){
    if(p == 0){
        return Identity();
    }
    Matrix x = power(A,p/2);
    if(p%2==0){
        return Multiply(x,x);
    }
    else{
        return Multiply(Multiply(x,x),A);
    }
}
ll Find_Nth_Term(vector<ll>&b , vector<ll>&c){
    if(n <= k){
        return b[n-1];
    }
    Matrix T(k,k);
    vector<ll>Fi(k);
    for(int i = 0 ; i < k ; i++){
        Fi[i]  = b[i];
    }
    for(int i = 0 ; i < k ; i++){
        for(int j = 0 ; j < k ; j++){
            if(i < k - 1){
                if(j == i + 1){
                    T.a[i][j] = 1;
                }
                else{
                    T.a[i][j] = 0;
                }
            }
            else{
                T.a[i][j] = c[k - j - 1];
            }
        }
    }
    T = power(T,n-1);

    ll res = 0;
    for(int i = 0 ; i < k ; i++){
        res = (res + (T.a[0][i] * Fi[i])%MOD)%MOD;
    }
    return res;
}
int main(){
    ll t;
    cin >> t;
    while(t--){
        //ll k , n;
        cin >> k;
        vector<ll>b(k,0);
        vector<ll>c(k,0);

        for(int i = 0 ; i < k ; i++){
            cin >> b[i];
        }
        for(int i = 0 ; i < k ; i++){
            cin >> c[i];
        }
        cin >> n;
        ll ans = Find_Nth_Term(b,c);
        cout<<ans<<endl;

    }

}
