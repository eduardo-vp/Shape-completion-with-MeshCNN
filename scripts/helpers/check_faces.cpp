#include <bits/stdc++.h>
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
using namespace std;

int main(){

	fastio;
	int cnt = 0;
	bool ok = true;
	string cad, prev;
	while(cin >> cad){
		if(prev == "Faces:"){
			// this is the number of vertices
			int x = stoi(cad);
			//cout << "x = " << x << endl;
			ok = ok && (x == 2048);
		}
		if(cad == "f") cnt++;
		prev = cad;
	}

	//cout << "cnt = " << cnt << endl;
	ok = ok && (cnt == 2048);

	if(ok) cerr << "Ok, fine" << endl;
	assert(ok);

	return 0;
}

