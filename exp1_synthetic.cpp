#include<bits/stdc++.h>
using namespace std;

const int maxn=85, maxT=5e+5, maxC=105;
const double eps=1e-5;

int n,N,C,istar,numSpc,numNonSpc,prd;
double epsilon,delta;
long double alpha;

void Input_Config(char *filename) {
	ifstream f(filename);
	f >> C >> epsilon >> delta >> n >> numSpc;
	f.close();
	
	istar=1;
	N=(1<<n)-1;
	alpha=2*n*(n-1)/(3*n-(long double)2);
	numNonSpc=n-numSpc-1;
}

long double Eworst[maxn][maxn][maxC],last[maxC];
int ans[maxC];
int Calc_Eworst() {
	for(int t=1; ; t++) {
		for(int c=0; c<=C; c++) last[c]=Eworst[numSpc][numNonSpc][c];
		
		for(int resSpc=0; resSpc<=numSpc; resSpc++)
			for(int resNonSpc=0; resNonSpc<=numNonSpc; resNonSpc++) {
				int res=resSpc+resNonSpc+1;
				for(int c=0; c<=C; c++) {
					Eworst[resSpc][resNonSpc][c]=(resNonSpc==0 && resSpc<=1 ?alpha :0 )+last[c];
					if (c) {
						if (resSpc) {
							if (resNonSpc) Eworst[resSpc][resNonSpc][c]+=min(Eworst[resSpc-1][resNonSpc][c],Eworst[resSpc][resNonSpc-1][c-1])*resSpc;
								else Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c]*resSpc;
						}
						if (resNonSpc) {
							if (resSpc) Eworst[resSpc][resNonSpc][c]+=min(Eworst[resSpc][resNonSpc-1][c],Eworst[resSpc-1][resNonSpc][c-1])*resNonSpc;
								else Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc][resNonSpc-1][c]*resNonSpc;
						}
					} else {
						if (resSpc) Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c]*resSpc;
						if (resNonSpc) Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc][resNonSpc-1][c]*resNonSpc;
					}
					Eworst[resSpc][resNonSpc][c]/=res;
				}
			}
		
		for(int ei=1; ei<=100; ei++) {
			long double e=ei/1000.0;
			if (!ans[ei] && 1-e<=Eworst[numSpc][numNonSpc][C]/t && Eworst[numSpc][numNonSpc][C]/t<=1+e) {
				ans[ei]=t;
			}
		}
		if (ans[1]) return t;
		if (t%1000==0) printf("%d: %.6f\n",t,(double)Eworst[numSpc][numNonSpc][C]/t);
	}
}

int main(int argc,char *argv[]) {
	Input_Config(argv[1]);
	
	int T=Calc_Eworst();
	
	for(int ei=1; ei<=100; ei++) printf("%.4f %d\n",ei/1000.0,ans[ei]);
}