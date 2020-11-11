// oh, the less I know the better
// corner cases // int vs ll // cin vs scanf // clear structures // statement // doublesz
#include <bits/stdc++.h>
#define endl '\n'
#define fst first
#define snd second
#define pb push_back
#define sz(x) int(x.size())
#define REP(i,n) for(int i = 0; i < int(n); ++i)
#define trace(x) cout << #x << " = " << x << endl
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
using namespace std;
typedef long long ll;
typedef pair<int,int> ii;

struct Point{
	double x,y,z;
	Point(){ x = y = z = 0; }
	Point(double x, double y, double z) : x(x), y(y), z(z){}
	Point operator + (Point other){
		return Point( x + other.x, y + other.y, z + other.z);
	}
	Point operator - (Point other){
		return Point(x - other.x, y - other.y, z - other.z);
	}
	Point operator / (double f){
		return Point( x / f, y / f, z / f );
	}
	double cross(Point other){
		double cx = y * other.z - z * other.y;
		double cy = x * other.z - z * other.x;
		double cz = x * other.y - y * other.x;
		return sqrt(cx * cx + cy * cy + cz * cz);
	}
};


struct Face{
	int u,v,w;
	Face(){ u = v = w = 0; }
	Face(int u, int v, int w) : u(u), v(v), w(w){}
};

ii make_edge(int u, int v){ return make_pair(min(u,v),max(u,v)); }

void read_file(string infile, vector<Point>&points, vector<Face>& faces){
	ifstream file;
	file.open(infile.c_str(), ios::in);
	string line;
	while(getline(file,line)){
		if(line[0] == 'v'){
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			points.pb(Point(x,y,z));
		}else if(line[0] == 'f'){
			istringstream iss(line);
			char ch; int u,v,w;
			iss >> ch >> u >> v >> w;
			u--; v--; w--;
			faces.pb(Face(u,v,w));
		}
	}
}

int main(int argc, char *argv[]){

	if(argc < 3){
		cerr << "Error" << endl;
		cerr << "El archivo se debe invocar ./file archivo_entrada archivo_salida" << endl;
		assert(false);
	}

	string infile = argv[1];
	string outfile = argv[2];

	vector<Point> points;
	vector<Face> faces;

	read_file(infile, points, faces);

	cerr << "Cantidad de puntos = " << sz(points) << endl;
	cerr << "Cantidad de caras = " << sz(faces) << endl;

	vector<Point> fpoints;
	vector<Face> ffaces;
	map< ii,int > to_point;

	auto add = [&](int u, int v){
		ii edge = make_edge(u,v);
		if(to_point.count(edge)) return;
		int id = sz(to_point);
		to_point[edge] = id;
		fpoints.pb( (points[u] + points[v]) / 2.0);
	};

	auto get_id = [&](int u, int v){
		return to_point[make_edge(u,v)];
	};

	for(Face f : faces){
		int u = f.u, v = f.v, w = f.w;
		add(u,v);
		add(v,w);
		add(u,w);
		int iduv = get_id(u,v);
		int idvw = get_id(v,w);
		int iduw = get_id(u,w);
		ffaces.pb(Face(iduv, idvw, iduw));
	}

	ofstream file;
	file.open(outfile.c_str());
	file << fixed << setprecision(5);
	// todo esta indexado en 0 hasta ahora
	{
		vector<Point> points;
		for(auto p : fpoints){
			file << "v " << p.x << " " << p.y << " " << p.z << endl;
			points.pb(Point(p.x, p.y, p.z));
		}
		double mean = 0;
		vector<double> areas;
		for(auto f : ffaces){
			int u = f.u, v = f.v, w = f.w;
			Point A = points[u];
			Point B = points[v];
			Point C = points[w];
			Point AB = B - A;
			Point AC = C - A;
			double area = AB.cross(AC) / 2;
			areas.pb(area);
			mean += area;
		}

		double stdd = 0;
		mean /= areas.size();
		for(double area : areas){
			stdd += (mean - area) * (mean - area);
		}
		assert(false);

		for(auto f : ffaces){
			file << "f " << f.u+1 << " " << f.v+1 << " " << f.w+1 << endl;
		}
	}
	return 0;
}


