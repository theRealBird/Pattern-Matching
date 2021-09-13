#include <bits/stdc++.h>
using namespace std;


const int ModAdler = 65521;

long long adler32Hash (string& s){

    int a = 1;
    int b = 0;

    for(int i=0;i<s.size();i++){

        a+= ((int)s[i])%ModAdler;
        b+= a%ModAdler;

    }

    long long x = b<<16;
    x = x|a;

    return x;

}

long long Adler32Update(long long x,string& s, int index){

    long long xa = x;
    long long xb = x;
    int b = xb>>16;
    xa=xa<<16;
    xa=xa>>16;
    int a = (int) xa; 
    a += (s[index]-s[index-s.length()]);
    b += (s[index]-s.length()*(s[index-s.length()]));

    long long newhash = b<<16 | a;
    return newhash;

}


long long LargePrime = 109;
long long Base = 256;

int ModPower(int x, int n, int M) {
    if(n==0) return 1;

    else if(n%2 == 0)        
        return ModPower((x*x) % M, n/2, M);

    else                             
        return (x*ModPower((x*x) % M, (n-1)/2, M)) % M;
}


int Hash (string& s){

    int h = 0;

    for(int i=0;i<s.size();i++){
        h = (h + ((int)s[i])*ModPower(Base,s.size()-i-1,LargePrime))%LargePrime;
    }

    return h;
}

int HashUpdate (long long h, string& text,int index,int l){

    int len = l;
    int cRemove = (((int)text[index-len])*ModPower(Base,len-1,LargePrime))%LargePrime;

    int newH = (((h - cRemove)*Base)%LargePrime + (int)(text[index]))%LargePrime;

    return newH;

}


int main(){

    string p = "Aditya";
    string GS = "AmoghSagarAdityaRajRohit";

    int hashP = Hash(p);

    string k = GS.substr(0,p.size());
    int hash = Hash(k);

    for(int i=p.size();i<GS.size();i++){

        if(hash==hashP){

            if(GS.substr(i-p.size(),p.size()) == p){
                cout<<"Yay!"<<"\n";
                return 0;
            }
            
        }
        
        hash = HashUpdate(hash,GS,i,p.size());

    }


    cout<<"No :(";
    return 0;

}




