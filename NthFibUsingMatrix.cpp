#include<bits/stdc++.h>
using namespace std;
#define ll long long
const int mod = 1e9+7;
class Matrix{
    public:
    ll mat[2][2];
    Matrix(){
        mat[0][0] = 0;
        mat[0][1] = 0;
        mat[1][0] = 0;
        mat[1][1] = 0;
    }
};

Matrix mul(Matrix A, Matrix B){
    Matrix C;
    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 2 ; j++){
            C.mat[i][j] = 0;//00
            for(int k = 0 ; k < 2 ; k++){//1
                C.mat[i][j] = (C.mat[i][j]%mod + (A.mat[i][k]%mod*B.mat[k][j]%mod)%mod)%mod;
            }
            C.mat[i][j] = C.mat[i][j]%mod;
        }
    }
    return C;
}
Matrix Identity(){
    Matrix I;
    I.mat[0][0] = 1;
    I.mat[0][1] = 0;
    I.mat[1][0] = 0;
    I.mat[1][1] = 1;
    return I;
}
Matrix MatrixPower(Matrix A, int exp){
    //base case
    if(exp == 0){
        return Identity();
    }
    //recursive case
    Matrix smallAns = MatrixPower(A,exp/2);
    if(exp%2 == 0){
        return mul(smallAns,smallAns);
    }
    else{
        return mul(A,mul(smallAns,smallAns));
    }
}
int main(){
    int exp;
    cin >> exp;

    Matrix A;
    A.mat[0][0] = 0; A.mat[0][1] = 1;
    A.mat[1][0] = 1; A.mat[1][1] = 1;
    
    
   // find the nth fibonacci number
    Matrix ans = MatrixPower(A,exp);
    cout<<ans.mat[0][1]%mod<<endl;

    return 0;

    
    
}
