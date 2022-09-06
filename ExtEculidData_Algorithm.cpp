//modular arthmetic
//(a*b)%p = ((a%p)*(b%p))%p
//to over come the overflow of a number
//like 2^20^3 do a mode with 1e9+7 then we can handle
//when we find powers and factriol there it become a problem 
//mod for +,*,-
// (a+b)%mod = (a%mod + b%mod)%mod
// (a*b)%mod = (a%mod * b%mod)%mod
// (a-b)%mod = (a%mod - b%mod + mod)%mod
// fermat's theorem
//a^(p-1)%p=1 it works for all primes p where a is not multiple of p
//Extended Euclid algorithm
//let say we have 2 values a and b and gcd(a,b)-->g
// ax + by = g
// ax % g =0 --> by % g == 0 --> g % g == 0;
// ax + by = gcd(a,b)
// gcd(a,b) = gcd(b,a%b)
// let take a = 35 and b = 15 then gcd g = 5
// ax + by = 5 --> 35(x) + 15(y) = 5 --> 35(1) + 15(-2) = 5 this equations are called Liner diopaintain
// ax + by = gcd(a,b) --> ax1 + by1 = gcd(b,a%b) --> bx1 + (a%b)y1 = gcd(b,a%b)
// a%b = a - b*(a/b) --> bx1 + a-b(a/b)y1 = gcd(b,a%b)
// bx1 + ay1 - b*(a/b)y1 = ax + by
// b(x1 - (a/b)y1) + ay1 = ax + by
// x = y1 and y = x1 -(a/b)y1
//one of the application is to calculate multiplacative modular inverse 
//example a*b = 1 then b is multiplacative inverse
// a*b = 1 (mod m) the b is the muliplactive moudulo inverse
//(a*b)%m = 1
//a*b-1 multiple of m
// ab - 1 = mq
// ab - mq = 1
// ab + m(-q) = 1
// ab + mQ = 1
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 1000000007;
class ExtEculidData{
public:
	ll gcd;
	ll x;
	ll y;
};
ExtEculidData gcdExtended(ll a , ll b){
	if(b == 0){
		//we do something
		ExtEculidData base;
		base.gcd = a; 
		base.x = 1;
		base.y = 0;
		return base;
	}
	ExtEculidData temp = gcdExtended(b , a % b);
	ExtEculidData myAns;
	myAns.gcd = temp.gcd;
	myAns.x = temp.y;
	myAns.y = (temp.x - ((a/b)*temp.y));
	return myAns;
}
int main(){
	int a,b;
	cin>>a>>b;
	cout<<-2%7<<"\n";
	//ExtEculidData ans = gcdExtended(a,b);
	//cout<<ans.gcd<<" "<<ans.x<<" "<<ans.y;
}