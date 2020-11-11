#include <bits/stdc++.h>
using namespace std;

int main(int argc, char * argv[]){

    if(argc < 2){
        cerr << "Error, el programa se debe ejecutar con el nombre del archivo de entrada" << endl;
        cerr << "Ejemplo: ./check_empty ./datasets/hola.obj" << endl;
        assert(false);
    }

    string infile = argv[1];
    freopen(infile.c_str(), "r", stdin);

    string line;
    int cnt = 0;
    while( getline(cin,line) ){
        cnt++;
    }

    if(cnt <= 2){
        // gg, es un archivo vacio, se debe borrar
        exit(1);
    }

    return 0;
}