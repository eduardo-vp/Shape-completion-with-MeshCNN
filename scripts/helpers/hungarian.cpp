#include <bits/stdc++.h>
using namespace std;

const double inf = 1e9;

bool zero(double x){ 
	return fabs(x) < 1e-9; 
}

// Algorithm for minimum

struct Hungarian{
	int n;
	vector<int> L,R;
	vector< vector<double> > cs;
	Hungarian(int N, int M) : n(max(N,M)), cs(n,vector<double>(n)), L(n), R(n){
		for(int x = 0; x < N; ++x){
			for(int y = 0; y < M; ++y){
				cs[x][y] = inf;
			}
		}
	}
	
	void set(int x, int y, double c){ cs[x][y] = c; }
	
	double assign(){
		int mat = 0;
		vector<int>  dad(n), sn(n);
		vector<double> ds(n), u(n), v(n);
		for(int i = 0; i < n; ++i){
			u[i] = *min_element(cs[i].begin(),cs[i].end());
		}
		for(int j = 0; j < n; ++j){
			v[j] = cs[0][j] - u[0];
			for(int i = 1; i < n; ++i){
				v[j] = min(v[j], cs[i][j] - u[i]);
			}
		}
		L = R = vector<int>(n, -1);
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				if(R[j] == -1 && zero(cs[i][j]-u[i]-v[j])){
					L[i] = j; 
					R[j] = i;
					mat++;
					break;
				}
			}
		}
		for(; mat < n; ++mat){
			int s = 0, j = 0, i;
			while(L[s] != -1) s++;
			fill(dad.begin(),dad.end(),-1);
			fill(sn.begin(),sn.end(),0);
			for(int k = 0; k < n; ++k){
				ds[k] = cs[s][k] - u[s] - v[k];
			}
			while(1){
				j = -1;
				for(int k = 0; k < n; ++k){
					if(!sn[k] && (j == -1 || ds[k] < ds[j])){
						j = k;
					}
				}
				sn[j] = 1; 
				i = R[j];
				if(i == -1) break;
				for(int k = 0; k < n; ++k) if(!sn[k]){
					auto new_ds = ds[j] + cs[i][k] - u[i] - v[k];
					if(ds[k] > new_ds){
						ds[k] = new_ds;
						dad[k] = j;
					}
				}
			}
			for(int k = 0; k < n; ++k){
				if(k != j && sn[k]){
					auto w = ds[k] - ds[j];
					v[k] += w;
					u[R[k]] -= w;
				}
			}
			u[s] += ds[j];
			while(dad[j] >= 0){
				int d = dad[j];
				R[j] = R[d];
				L[R[j]] = j;
				j = d;
			}
			R[j] = s;
			L[s] = j;
		}
		double value = 0;
		for(int i = 0; i < n; ++i){
			value += cs[i][L[i]];
		}
		return value;
	}
};



int main(int argc, char *argv[]){
	
	
	
	return 0;
}
