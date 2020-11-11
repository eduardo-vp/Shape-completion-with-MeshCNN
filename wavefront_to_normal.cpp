#include <bits/stdc++.h>
using namespace std;

int getType(string line){
	for(int i = 0; i+1 < line.size(); ++i){
		if(line[i] == line[i+1] && line[i] == '/') return 1;
	}
	return 0;
}

int main(int argc, char * argv[]){

	if(argc < 3){
		cerr << "Error, se esperan 3 archivos" << endl;
		cerr << "Ejemplo:" << "./wavefront_to_normal ./datasets./in.obj ./datasets./out.obj" << endl;
		assert(false);
	}

	string infile = argv[1];
	string outfile = argv[2];

	cerr << "infile = " << infile << endl;
	cerr << "outfile = " << outfile << endl;
	freopen(infile.c_str(), "r", stdin);
	freopen(outfile.c_str(), "w", stdout);

	string line;
	while(getline(cin,line)){
		if(line.size() < 4) continue;
		if(line[0] == 'v' && line[1] == ' '){
			// vertex
			cout << line << endl;
		}else if(line[0] == 'f'){
			int type = getType(line);
			if(type == 0){
				// a/b/c 
				istringstream iss(line);
				char ch; int a,b,c;
				iss >> ch;
				cout << "f";
				for(int t = 0; t < 3; ++t){
					iss >> a >> ch >> b >> ch >> c;
					cout << " " << a;
				}
				cout << endl;
			}else{
				// a//b
				istringstream iss(line);
				char ch; int a,b;
				iss >> ch;
				cout << "f";
				for(int t = 0; t < 3; ++t){
					iss >> a >> ch >> ch >> b;
					cout << " " << a;
				}
				cout << endl;
			}
		}
	}

	return 0;
}
