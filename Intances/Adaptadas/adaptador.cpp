/* This program was written to adapt the instaces from TSPLIB format to transgenetic one 
 */
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

int main(){
	string inp;
	string doispontos;
	int n;
	int node;
	float dx, dy;
	int *dxs;
	int *dys;
	while (cin>>inp){
		if (inp=="DIMENSION"){
			cin>>doispontos;
			cin>>n;
			dxs = new int[n];
			dys = new int[n];
		}
		if (inp=="NODE_COORD_SECTION"){
			while (cin>>node){
				cin>>dx;
				cin>>dy;
				dxs[node-1] = dx;
				dys[node-1] = dy;
			}
		}
	}

	cout<<n<<endl;
	for (int i=0; i<n; i++){
		for (int j = i+1; j<n; j++){
			float distance = sqrt((dxs[i]-dxs[j])*(dxs[i]-dxs[j]) + (dys[i]-dys[j])*(dys[i]-dys[j]));
			cout<<i<<" "<<j<<" "<<round(distance)<<endl;
			//cout<<i<<" "<<j<<" "<<distance<<endl;
		}
	}

	delete[] dxs;
	delete[] dys;

	return 0;
}