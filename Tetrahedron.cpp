//A. Tetrahedron
//link : https://codeforces.com/gym/316783/problem/A

//solution
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int32_t mod = 1e9+7;
//ll mod = 1e9+7;
struct matrix{
    ll mat[4][4];
};
void TakeInput(){
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif
}
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
matrix mul(matrix A, matrix B){
    matrix C;
    for(int row = 0 ; row < 4 ; row++){
        for(int col = 0 ; col < 4 ; col++){
            ll sum = 0;
            for(int i = 0 ; i < 4 ; i++){
                sum+=((A.mat[row][i]%mod)*(B.mat[i][col]%mod))%mod;
            }
            C.mat[row][col]=sum%mod;
        }
    }
    return C;

}
matrix identity(){
    matrix id;
    id.mat[0][0] = 1, id.mat[0][1] = 0, id.mat[0][2] = 0, id.mat[0][3]=0;
    id.mat[1][0] = 0, id.mat[1][1] = 1, id.mat[1][2] = 0, id.mat[1][3]=0;
    id.mat[2][0] = 0, id.mat[2][1] = 0, id.mat[2][2] = 1, id.mat[2][3]=0;
    id.mat[3][0] = 0, id.mat[3][1] = 0, id.mat[3][2] = 0, id.mat[3][3]=1;
    return id;

}
matrix MatrixExpo(matrix A, ll n){
    if(n==0)return identity();
    matrix res = MatrixExpo(A,n/2);
    if(n%2==0){
        return mul(res,res);
    }
    else{
        return mul(mul(res,res),A);
    }
}
void Don(){
    ll n;
    cin >> n;
    matrix A;
    A.mat[0][0] = 0, A.mat[0][1] = 1, A.mat[0][2] = 1, A.mat[0][3] = 1;
    A.mat[1][0] = 1, A.mat[1][1] = 0, A.mat[1][2] = 1, A.mat[1][3] = 1;
    A.mat[2][0] = 1, A.mat[2][1] = 1, A.mat[2][2] = 0, A.mat[2][3] = 1;
    A.mat[3][0] = 1, A.mat[3][1] = 1, A.mat[3][2] = 1, A.mat[3][3] = 0;
    cout<<MatrixExpo(A,n).mat[3][3]%mod;
}
int main(){
    TakeInput();
    Don();
}
