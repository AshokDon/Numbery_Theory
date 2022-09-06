//chenis remainder throm
//given p1 and p2 as well as r1 and r2 where x%p1=r1 and x%p2=r2 where 
//p1 and p2 are primes
// find x ?
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
	ll p1,p2,r1,r2;
	cin >> p1 >> p2 >> r1 >> r2;
	//method 1
	// this approach gives time complexity of O(p1*p2)
	//what if p1 and p2 are big
	for(int i = 0 ; i < p1*p2 ; i++){
		if(i % p1 == r1 and i % p2 == r2){
			cout<<i<<endl;

		}
	}
	// mathod 2
	// x -> r1 % p1
	// x -> r2 % p2
	// x -> r1 , r1+p1 , r1+2p1 , r1+3p1
	for(int x = 0 ; r1 + p1 * x < p1*p2 ; x++){
		if((r1+p1*x)%p2==r2){
			cout<<r1+p1*x<<endl;
		}
	}

}
