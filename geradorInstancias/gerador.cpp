/*Gerador aleatorio de grafos completos mono-objetivo*/

#include <iostream>
#include <map> 
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <stack>   
#include <utility> 
#include <sys/times.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <queue>          // std::queue
using namespace std;

int main(){
	int n;
	//cin>>n;
	n=1000;
	cout<<n<<endl;
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			
				int peso = rand()%30 +1;
				cout<<i<<" "<<j<<" "<<peso<<endl;
			
		}
	}
}