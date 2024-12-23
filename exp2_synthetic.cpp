#include<bits/stdc++.h>
using namespace std;

const int maxn=55, maxT=8e5+5, maxC=205;
const double eps=1e-5;

int n,N,T,C,istar,numSpc,numNonSpc,prd;
double epsilon,delta;
long double alpha;

void Input_Config(char *filename) {
	ifstream f(filename);
	f >> T >> C >> epsilon >> delta >> n >> numSpc;
	f.close();
	
	istar=1;
	N=(1<<n)-1;
	alpha=2*n*(n-1)/(3*n-(long double)2);
	numNonSpc=n-numSpc-1;
}

static std::mt19937 prg(time(0));

template <typename T>
T ran_int(T a,T b) {
	std::uniform_int_distribution<T> u(a,b);
	return u(prg);
}

long double Eworst[maxn][maxn][maxC],last[maxC];
int strategy[maxT][maxC];
void Calc_Eworst(int T) {
	memset(strategy,255,sizeof(strategy));
	
	for(int t=0; t<T; t++) {
		for(int resSpc=0; resSpc<=numSpc; resSpc++)
			for(int resNonSpc=0; resNonSpc<=numNonSpc; resNonSpc++) {
				int res=resSpc+resNonSpc+1;
				for(int c=0; c<=C; c++) {
					Eworst[resSpc][resNonSpc][c]=(resNonSpc==0 && resSpc<=1 ?alpha :0 )+last[c];
					if (c) {
						if (resSpc) {
							// if (resNonSpc) Eworst[resSpc][resNonSpc][c]+=min(Eworst[resSpc-1][resNonSpc][c],Eworst[resSpc][resNonSpc-1][c-1])*resSpc;
								// else Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c]*resSpc;
							Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c]*resSpc;
						}
						if (resNonSpc) {
							if (resSpc && Eworst[resSpc][resNonSpc-1][c]>Eworst[resSpc-1][resNonSpc][c-1]+eps) {
								Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c-1]*resNonSpc;
								if (strategy[t][resSpc]==-1) strategy[t][resSpc]=c;
							} else Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc][resNonSpc-1][c]*resNonSpc;
						}
					} else {
						if (resSpc) Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc-1][resNonSpc][c]*resSpc;
						if (resNonSpc) Eworst[resSpc][resNonSpc][c]+=Eworst[resSpc][resNonSpc-1][c]*resNonSpc;
					}
					Eworst[resSpc][resNonSpc][c]/=res;
				}
			}
		
		for(int c=0; c<=C; c++) last[c]=Eworst[numSpc][numNonSpc][c];
		if (t%1000==0) printf("preprocessing %d %.6f\n",t,(double)(Eworst[numSpc][numNonSpc][C]/t));
	}
}

long double Simulate() {
	long double ans=0;
	int c=C;
	for(int t=1; t<=T; t++) {
		int resSpc=numSpc, resNonSpc=numNonSpc;
		for(int i=1; i<=n; i++) {
			int lose=ran_int(0,resSpc+resNonSpc);
			if (lose==0) {
				ans+=(resNonSpc==0 && resSpc<=1 ?alpha :0 );
				break;
			} else if (lose<=resSpc) {
				resSpc--;
			} else {
				if (c && resSpc && resNonSpc==1 && strategy[T-t][resSpc]!=-1 && strategy[T-t][resSpc]<=c) {
					resSpc--;
					c--;
				} else {
					resNonSpc--;
				}
			}
		}
	}
	
	return ans/T;
}

int main(int argc,char *argv[]) {
	Input_Config(argv[1]);
	
	Calc_Eworst(T);
	
	for(int i=1; i<=1000; i++)
		printf("simulation %d %.6f\n",i,(double)Simulate());
}