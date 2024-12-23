#include<bits/stdc++.h>
using namespace std;

const int maxSpc=25, maxN=(1<<13)+5, maxC=102, maxNonSpc=91, maxT=6400, maxEpoch=1005;
const double eps=1e-5;

int n,N,numSpc,numNonSpc,T,C,istar,MC[maxN];
double epsilon,delta;

void Input_Config(char *filename) {
	ifstream f(filename);
	f >> T >> C >> epsilon >> delta >> numNonSpc >> numSpc;
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

static std::mt19937 prg(time(0));

template <typename T>
T ran_int(T a,T b) {
	std::uniform_int_distribution<T> u(a,b);
	return u(prg);
}

int sizeS[maxN];
long double sv,Binom[maxSpc][maxSpc];
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

long double Eworst[maxN][maxNonSpc][maxC],last[maxC],Eworst_rc[maxT][maxC];
void Calc_Eworst() {
	for(int t=1; t<=1; t++) {
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
	}
}

int p[maxSpc],resc[maxEpoch];
long double ans[maxEpoch];
void Simulate() {
	for(int epoch=1; epoch<=1000; epoch++) resc[epoch]=C;
	
	for(int t=1; t<=T; t++) {
		memcpy(last,Eworst_rc[T-t],sizeof(last));
		Calc_Eworst();
		
		for(int epoch=1; epoch<=1000; epoch++) {
			int resSpc=numSpc, resNonSpc=numNonSpc, S=0, curp=0;
			for(int i=0; i<numSpc; i++) p[i]=i;
			random_shuffle(p,p+numSpc);
			for(int i=1; i<=n; i++) {
				int lose=ran_int(0,resSpc+resNonSpc);
				if (lose==0) {
					ans[epoch]+=MC[S];
					break;
				} else if (lose<=resSpc) {
					if (resc[epoch] && resNonSpc && Eworst[N^S^(1<<p[curp])][resNonSpc][resc[epoch]]>Eworst[N^S][resNonSpc-1][resc[epoch]-1]+eps) {
						resNonSpc--;
						resc[epoch]--;
						continue;
					} else if (resc[epoch] && !resNonSpc) {
						bool sacrifice=0;
						for(int j=curp+1; j<numSpc; j++) if (Eworst[N^S^(1<<p[curp])][0][resc[epoch]]>Eworst[N^S^(1<<p[j])][0][resc[epoch]-1]+eps) {
							swap(p[curp],p[j]);
							sacrifice=1;
						}
						random_shuffle(p+curp+1,p+numSpc);
						resc[epoch]-=sacrifice;
					}
					S|=(1<<p[curp]);
					resSpc--;
					curp++;
				} else {
					resNonSpc--;
				}
			}
		}
		
		if (t%100==0) printf("t %d %.6f\n",t,(double)ans[1]);
	}
	
	for(int epoch=1; epoch<=1000; epoch++) ans[epoch]/=T;
}

int main(int argc,char *argv[]) {
	Input_Config(argv[1]);
	
	Pre();
	for(int t=0; t<T; t++) {
		Calc_Eworst();
		for(int c=0; c<=C; c++) Eworst_rc[t][c]=Eworst[N][numNonSpc][c];
		printf("preprocessing %d\n",t);
	}
	
	Simulate();
	for(int epoch=1; epoch<=1000; epoch++) printf("simulation %d %.6f\n",epoch,(double)ans[epoch]);
}