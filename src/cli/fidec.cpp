#include "fidec.h"
using namespace std;

int main() {
	cout << "\n#########################################################\n";
	cout << "\tFinite-Difference Embedding Code (FiDEC)\n";
	cout << "#########################################################\n\n";
	
	cout << "Available surface metrics:\n";
	cout << "\n";

	cout << "Choose a surface metric: ";
	string metricName;
	cin >> metricName;

	cout << "You chose the surface metric " << metricName.data() << "!\n";
}
