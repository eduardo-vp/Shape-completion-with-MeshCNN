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

int main(int argc, char *argv[]){

	if(argc < 3){
        cerr << "Se esperan 2 argumentos" << endl;
        cerr << "Ejemplo: ./get_mesh_from_incomplete ./datasets/3D_Pottery/train/bla.obj salida.obj" << endl;
        assert(false);
    }

    string infile = argv[1];
    string outfile = argv[2];

    freopen(infile.c_str(), "r", stdin);
    freopen(outfile.c_str(), "w", stdout);

    string line;
    while(getline(cin,line)){
        if(line[0] == '#'){
            cout << "v " << line.substr(1,line.size()-1) << endl;
        }else if(line[0] == 'f'){
            cout << line << endl;
        }
    }

	return 0;
}