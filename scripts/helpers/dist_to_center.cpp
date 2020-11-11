#include <bits/stdc++.h>
using namespace std;

struct Point{
	double x,y,z;
	Point(double x, double y, double z) : x(x), y(y), z(z){}
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
		cerr << "Ejemplo: ./dist_to_center ./datasets/Complete/alabastron.obj ./distancias.txt" << endl;
		exit(1);
	}

	string infile = argv[1];
	string outfile = argv[2];
	cerr << "Archivo de entrada = " << infile << endl;
	cerr << "Archivo de salida = " << outfile << endl;
	freopen(infile.c_str(), "r", stdin);
	freopen(outfile.c_str(), "a", stdout);

	string line;
	vector<Point> points;
	double xt = 0, yt = 0, zt = 0;
	while(getline(cin,line)){
		if(line[0] == 'v'){
			istringstream iss(line);
			char ch; double x,y,z;
			iss >> ch >> x >> y >> z;
			points.push_back(Point(x,y,z));
			xt += x;
			yt += y;
			zt += z;
		}
	}

	int n = points.size();
	xt /= n;
	yt /= n;
	zt /= n;
	Point center(xt,yt,zt);
	double dist = 0;
	for(Point p : points){
		dist = max(dist, getDist(p,center));
	}

	cout << fixed << setprecision(6) << dist << endl;

	return 0;
}
