//number theory
//factores , primes , prime factrozation
//sqrt method , seive method
//almost prime it can be representd as product of two primes 

#include<bits/stdc++.h>
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
