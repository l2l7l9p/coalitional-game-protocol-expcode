#include<bits/stdc++.h>
using namespace std;

const int maxn=25, maxN=(1<<13)+5, maxC=102, maxNonSpc=91;
const double eps=1e-5;

int n,N,numSpc,numNonSpc,C,istar,MC[maxN];
double epsilon,delta;

void Input_Config(char *filename) {
	ifstream f(filename);
	f >> C >> epsilon >> delta >> numNonSpc >> numSpc;
	int m;
	f >> m;
	while (m--) {
		int num,x,S=0;
		f >> num;
		while (num--) {
			f >> x;
			S|=(1<<(x-1));
		}
		MC[S]++;
	}
	f.close();
	
	istar=0;
	n=1+numSpc+numNonSpc;
	N=(1<<numSpc)-1;
}

int sizeS[maxN];
long double sv,Binom[maxn][maxn];
void Pre() {
	for(int S=1; S<=N; S++) {
		int lowbit=S&(-S);
		sizeS[S]=sizeS[S^lowbit]+1;
	}
	
	for(int i=0; i<numSpc; i++) {
		int NS1=(1<<(numSpc-1-i))-1, NS2=(1<<i)-1;
		for(int S1=0; S1<=NS1; S1++)
			for(int S2=0; S2<=NS2; S2++)
				MC[(S1<<(i+1))|(1<<i)|S2]+=MC[(S1<<(i+1))|S2];
	}
	
	for(int i=0; i<=numSpc; i++) {
		Binom[i][0]=1;
		for(int j=1; j<=i; j++) Binom[i][j]=Binom[i-1][j-1]+Binom[i-1][j];
	}
	
	for(int S=0; S<=N; S++) sv+=MC[S]/Binom[numSpc][sizeS[S]];
	sv/=(numSpc+1);
	printf("sv: %.6f\n",(double)sv);
}

long double Eworst[maxN][maxNonSpc][maxC],last[maxC];
int anseps[1005],ansc[105];
void Calc_Eworst() {
	for(int t=1; ; t++) {
		for(int c=0; c<=C; c++) last[c]=Eworst[N][numNonSpc][c];
		
		for(int S=0; S<=N; S++) {
			for(int resNonSpc=0; resNonSpc<=numNonSpc; resNonSpc++) {
				for(int c=0; c<=C; c++) {
					Eworst[S][resNonSpc][c]=MC[N^S]+last[c];
					if (resNonSpc) Eworst[S][resNonSpc][c]+=Eworst[S][resNonSpc-1][c]*resNonSpc;
					if (c) {
						if (resNonSpc) {
							for(int SS=S, lowbit; SS; SS-=lowbit) {
								lowbit=SS&(-SS);
								Eworst[S][resNonSpc][c]+=min(Eworst[S^lowbit][resNonSpc][c],Eworst[S][resNonSpc-1][c-1]);
							}
						} else {
							long double minSacrifice=-1;
							for(int SS=S, lowbit; SS; SS-=lowbit) {
								lowbit=SS&(-SS);
								minSacrifice=(minSacrifice<0 ?Eworst[S^lowbit][0][c-1] :min(minSacrifice,Eworst[S^lowbit][0][c-1]));
							}
							for(int SS=S, lowbit; SS; SS-=lowbit) {
								lowbit=SS&(-SS);
								Eworst[S][0][c]+=min(Eworst[S^lowbit][0][c],minSacrifice);
							}
						}
					} else {
						for(int SS=S, lowbit; SS; SS-=lowbit) {
							lowbit=SS&(-SS);
							Eworst[S][resNonSpc][0]+=Eworst[S^lowbit][resNonSpc][0];
						}
					}
					Eworst[S][resNonSpc][c]/=(sizeS[S]+resNonSpc+1);
				}
			}
		}
		
		for(int c=0; c<=C; c++) if (!ansc[c] && sv*(1-epsilon)<=Eworst[N][numNonSpc][c]/t) ansc[c]=t;
		for(int ei=1; ei<=100; ei++) if (!anseps[ei] && sv*(1-ei/1000.0)<=Eworst[N][numNonSpc][20]/t) anseps[ei]=t;
		if (sv*0.999<=Eworst[N][numNonSpc][20]/t && sv*(1-epsilon)<=Eworst[N][numNonSpc][C]/t) return;
		printf("%d %.6f %.6f\n",t,(double)(Eworst[N][numNonSpc][C]/t),(double)(Eworst[N][numNonSpc][20]/t));
	}
}

int main(int argc,char *argv[]) {
	Input_Config(argv[1]);
	
	Pre();
	Calc_Eworst();
	
	for(int c=0; c<=C; c++) printf("%d %d\n",c,ansc[c]);
	for(int ei=1; ei<=100; ei++) printf("%.4f %d\n",ei/1000.0,anseps[ei]);
}