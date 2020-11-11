#include <bits/stdc++.h>
#define sz(x) int(x.size())
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

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

struct Point{
	double x,y,z;
	Point(double x, double y, double z) : x(x), y(y), z(z){}
};

struct Face{
	int u,v,w;
	Face(int u, int v, int w) : u(u), v(v), w(w){}
};

double get_dist(Point a, Point b){
	double x = a.x - b.x;
	double y = a.y - b.y;
	double z = a.z - b.z;
	return sqrt( x*x + y*y + z*z );
}

vector<Face> faces;

vector<Point> get_vertices(string fname){
	ifstream file;
	file.open(fname.c_str(), ios::in);
	string line;
	vector<Point> ans;
	faces.clear();
	while(getline(file,line)){
		if(line[0] == 'v'){
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			ans.push_back(Point(x,y,z));
		}else if(line[0] == 'f'){
			istringstream iss(line);
			char ch; int u,v,w;
			iss >> ch >> u >> v >> w;
			faces.push_back(Face(u,v,w));
		}
	}
	return ans;
}

int main(int argc, char *argv[]){
	
	if(argc < 4){
		cerr << "Error" << endl;
		cerr << "El archivo se debe invocar ./hungarian complete.obj incomplete.obj incomplete_post_hungarian.obj" << endl;
		assert(false);
	}
	
	ofstream post;
	post.open(argv[3]);
	
	string comp = argv[1];
	string incomp = argv[2];
	vector<Point> vcomp = get_vertices(comp);
	vector<Point> vincomp = get_vertices(incomp);

	cerr << vincomp.size() << endl;
	cerr << vcomp.size() << endl;
	//assert( vincomp.size() <= vcomp.size() );
	if(sz(vincomp) > sz(vcomp)){
		cerr << "No cuadran la cantidad de vertices" << endl;
		return 0;
	}

	while(sz(vcomp) > sz(vincomp)){
		int idx = uniform_int_distribution<int>(0,sz(vcomp)-1)(rng);
		vcomp.erase(vcomp.begin() + idx);
	}

	Hungarian hung(vincomp.size(), vcomp.size());
	for(int i = 0; i < vincomp.size(); ++i){
		for(int j = 0; j < vcomp.size(); ++j){
			hung.set(i,j,get_dist(vincomp[i],vcomp[j]));
		}
	}

	string incomp_hung = argv[3];
	ofstream file;
	file.open(incomp_hung.c_str());
	double cost = hung.assign();
	cerr << fixed << setprecision(10) << "Costo minimo = " << cost << endl;
	// escribir en post
	double check = 0;
	for(int i = 0; i < sz(vincomp); ++i){
		file << "v " << vincomp[i].x << " " << vincomp[i].y << " " << vincomp[i].z << endl;
		Point other = vcomp[hung.L[i]];
		file << "# " << other.x << " " << other.y << " " << other.z << endl;
		check += get_dist(vincomp[i],other);
	}

	for(auto face : faces){
		file << "f " << face.u << " " << face.v << " " << face.w << endl;
	}

	assert(fabs(cost - check) <= 1e-6);

	return 0;
}
