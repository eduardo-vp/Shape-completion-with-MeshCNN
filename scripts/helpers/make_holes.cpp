#include <bits/stdc++.h>
#define pb push_back
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Point{
	double x,y,z;
	Point(){ x = y = z = 0; }
	Point(double x, double y, double z) : x(x), y(y), z(z){}
};

struct Face{
	int u,v,w;
	Face(int u, int v, int w) : u(u), v(v), w(w){}
};

vector<Face> faces;
vector<Point> points;
vector< vector<int> > adj;

void add_edge(int u, int v){
	adj[u].pb(v);
	adj[v].pb(u);
}

void read(){
	string line;
	while(getline(cin,line)){
		if(line.size() < 2) continue;
		if(line[0] == 'v' && line[1] != 'n'){
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			points.pb(Point(x,y,z));
			adj.pb( vector<int>() );
		}else if(line[0] == 'f'){
			istringstream iss(line);
			char ch; int u,v,w;
			iss >> ch >> u >> v >> w;
			u--; v--; w--;
			faces.pb(Face(u,v,w));
			add_edge(u,v);
			add_edge(v,w);
			add_edge(u,w);
		}
	}
}

struct UnionFind{
	int n, comps; vector<int> p, sz;
	UnionFind(int n) : n(n), comps(n), p(n), sz(n){
		iota(p.begin(),p.end(),0);
		fill(sz.begin(),sz.end(),1);
	}
	int find(int i){ return (p[i] == i) ? i : (p[i] = find(p[i])); }
	void join(int i, int j){
		int x = find(i), y = find(j);
		if(sz[x] < sz[y]) swap(x,y);
		p[y] = x;
		sz[x] += sz[y];
		comps--;
	}
};

int get_connected_components(vector<int> mask){
	int n = mask.size();
	UnionFind uf(n);
	for(int u = 0; u < n; ++u){
		if(mask[u] == 0) continue;
		for(int v : adj[u]){
			if(mask[v] == 0) continue;
			if(uf.find(u) != uf.find(v)){
				uf.join(u,v);
			}
		}
	}
	unordered_set<int> s;
	for(int u = 0; u < n; ++u){
		if(mask[u] == 0) continue;
		s.insert(uf.find(u));
	}
	return s.size();
}

bool inside(Point O, Point A, double r, int type){
	assert(type >= 0);
	assert(type < 2);
	if(type == 0){
		// esfera
		double x = A.x - O.x;
		double y = A.y - O.y;
		double z = A.z - O.z;
		return sqrt(x*x + y*y + z*z) <= r;
	}else{
		// cubo
		double x = fabs(A.x - O.x);
		double y = fabs(A.y - O.y);
		double z = fabs(A.z - O.z);
		return max({x,y,z}) <= r;
	}
}

void print(vector<int> mask){
	int n = points.size();
	int gid = 1;
	vector<int> to(n);
	cout << fixed << setprecision(6);
	cout << "# Imagen generada por el archivo make_holes.cpp" << endl;
	for(int u = 0; u < n; ++u){
		if(mask[u] == 0) continue;
		to[u] = gid++;
		cout << "v " << points[u].x << " " << points[u].y << " " << points[u].z << endl;
	}
	for(Face f : faces){
		int u = f.u, v = f.v, w = f.w;
		if(mask[u] && mask[v] && mask[w]){
			cout << "f " << to[u] << " " << to[v] << " " << to[w] << endl;
		}
	}
}

int main(int argc, char * argv[]){
	
	if(argc <= 2){
		cout << "El archivo necesita por lo menos 2 parametros (archivo de entrada y de salida)" << endl;
		cout << "Ejemplo: ./make_holes ./datasets/001.obj ./datasets/001_final.obj" << endl;
		assert(false);
	}

	string infile = argv[1];
	string outfile = argv[2];
	cout << "Archivo de entrada: " << infile << endl;
	cout << "Archivo de salida: " << outfile << endl;
	
	freopen(infile.c_str(), "r", stdin);
	freopen(outfile.c_str(), "w", stdout);
	
	read();
	
	int n = points.size();
	double rmin = 10, rmax = 70;
	// realizar fracturas
	vector<int> mask(n,1);
	// intento de fractura
	while(true){
		int type = rng()%2; // 0 -> esfera, 1 -> cubo
		double r = uniform_real_distribution<double>(rmin,rmax)(rng);
		int index = rng()%n;
		Point O = points[index];
		vector<int> still = mask;
		int off = 0;
		for(int u = 0; u < n; ++u){
			if(inside(O,points[u],r,type)) still[u] = 0, off++;
		}
		double frac = 1.0 * off / n; // porcentaje de vertices eliminados
		if(get_connected_components(still) == get_connected_components(mask) && frac > 0.08 && frac < 0.165){ 
			cerr << "Fracturado" << endl;
			mask = still;
			print(mask);
			return 0;
		}
	}

	cerr << "----------------------------------------------------------- EL ARCHIVO NO SE PROCESO -------------------------------------------------------------------" << endl;
	assert(false);
	
	return 0;
}
