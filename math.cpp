//number theory
//factores , primes , prime factrozation
//sqrt method , seive method
//almost prime it can be representd as product of two primes 

#include<bits/stdc++.h>
#include<bits/stdc++.h>
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)
#define ll long long

using namespace std;
vector<ll>Primes;
bool isPrimes[90000001];
void Sieve(){
    isPrimes[0] = isPrimes[1] = true;
    ll maxN = 90000000;
    for(int i = 2 ; i * i <= maxN ; i++){
        if(!isPrimes[i]){
            for(int j = i * i ; j <= maxN ; j+=i){
                isPrimes[j] = true;
            }
        }
    }
    for(int i = 2 ; i <= maxN ; i++){
        if(!isPrimes[i]){
            Primes.push_back(i);
        }
    }

}
ll power(ll a, ll n, ll mod){
    ll res = 1;
    while(n){
        if(n & 1){
            res = (res * a) % mod;
        }
        a = (a * a) % mod;
        n >>= 1;
    }
    return res;
}
using namespace std;
bool Isprime(int num){
	for(int i = 2 ; i*i <= num ; i++){
		if(num%i==0)return false;
	}
	return true;
}
bool almost_prime(int num){
	int cnt = 0;
	for(int i = 2 ; i*i <= num ; i++){
		if(num % i == 0){
			if(Isprime(i) and Isprime(num/i) and i!=num/i){
				return true;
			}
		}
	}
	return false;
}
const int LIM = 1e6+5;
int main(){
	int N;
	cin>>N;
	if(almost_prime(N))cout<<"YES\n";
	else cout<<"NO\n";

}
