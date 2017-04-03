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
	float peso;
	float dx, dy;
	int linha =0, coluna=0;
	while (cin>>inp){
		//cout<<inp<<endl;
		if (inp=="DIMENSION:"){
			//cin>>doispontos;
			cin>>n;
			cout<<n<<endl;
		}
		if (inp=="EDGE_WEIGHT_SECTION"){
			while (cin>>peso){
				if (linha!=coluna){
					cout<<linha<<" "<<coluna<<" "<<peso<<endl;
				}
				if (coluna<n-1) coluna++;
				else {
					coluna = 0;
					linha++;
				}
			}
		}
	}

	return 0;
}