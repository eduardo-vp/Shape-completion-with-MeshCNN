#include <bits/stdc++.h>
#define sz(x) int(x.size())
using namespace std;

struct Point{
	double x,y,z;
	Point(double x, double y, double z) : x(x), y(y), z(z){}
	Point operator + (Point p){
		return Point(x+p.x, y+p.y, z+p.z);
	}
	Point operator - (Point p){
		return Point(x-p.x, y-p.y, z-p.z);
	}
	Point operator * (double f){
		return Point(x*f, y*f, z*f);
	}
};

struct Face{
	int u,v,w;
	Face(int u, int v, int w) : u(u), v(v), w(w){}
};

double getDist(Point A, Point B){
	double x = A.x - B.x;
	double y = A.y - B.y;
	double z = A.z - B.z;
	return sqrt(x*x + y*y + z*z);
}

int main(int argc, char *argv[]){

	if(argc < 3){
		cerr << "Error, el archivo espera 2 parametros (nombre de archivo de entrada y de salida)" << endl;
		cerr << "Ejemplo: ./dist_to_center ./datasets/Complete/alabastron.obj ./datasets/Complete_normalized/alabastron.obj" << endl;
		exit(1);
	}

	string infile = argv[1];
	string outfile = argv[2];
	cerr << "Archivo de entrada = " << infile << endl;
	cerr << "Archivo de salida = " << outfile << endl;
	freopen(infile.c_str(), "r", stdin);
	freopen(outfile.c_str(), "a", stdout);

	// lectura
	string line;
	vector<Face> faces;
	vector<Point> points;
	double xt = 0, yt = 0, zt = 0;
	vector<Point> real_points;
	while(getline(cin,line)){
		if(line[0] == 'v'){
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			points.push_back(Point(x,y,z));
			xt += x;
			yt += y;
			zt += z;
		}else if(line[0] == '#'){ 
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			real_points.push_back(Point(x,y,z));
		}else if(line[0] == 'f'){
			istringstream iss(line);
			char ch; int u,v,w;
			iss >> ch >> u >> v >> w;
			faces.push_back(Face(u,v,w));
		}
	}

	// procesamiento 
	int n = points.size();
	xt /= n;
	yt /= n;
	zt /= n;
	Point center(xt,yt,zt);
	Point origin(0,0,0);
	Point delta = origin - center;

	for(Point &p : points){
		p = p + delta;
	}

	for(Point &p : real_points){
		p = p + delta;
	}

	double RMAX = 1;
	function<bool(double)> check = [&](double f){
		for(Point p : points){
			p = p * f;
			if(getDist(p, origin) > RMAX) return false;
		}
		return true;
	};

	double low = 0, high = 10000000;
	while(high - low > 1e-4){
		double mid = (low + high) / 2;
		if(check(mid)) low = mid;
		else high = mid;
	}

	for(Point &p : points){
		p = p * low;
	}

	for(Point &p : real_points){
		p = p * low;
	}

	assert(sz(real_points) == 0 || sz(real_points) == sz(points));
	// impresion
	for(int i = 0; i < sz(points); ++i){
		Point p = points[i];
		cout << fixed << setprecision(6) << "v " << p.x << " " << p.y << " " << p.z << endl;
		if(i < sz(real_points)){
			p = real_points[i];
			cout << "# " << p.x << " " << p.y << " " << p.z << endl;
		}
	}
	for(Face f : faces){
		cout << "f " << f.u << " " << f.v << " " << f.w << endl;
	}
	
	return 0;
}
