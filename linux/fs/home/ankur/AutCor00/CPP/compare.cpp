#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main (int argc, char **argv)
{
    if (argc < 3) {
        cerr << "Usage: <executable> <input file 1> <input file 2>\n";
        return -1;
    }

    ifstream ifh1, ifh2;
    ifh1.open(argv[1]);
    ifh2.open(argv[2]);

    int ds1, ds2;
    ifh1 >> ds1;
    ifh2 >> ds2;

    if (ds1 != ds2) {
        cerr << "Number of elements in " << argv[1] << " and " << argv[2] << " are not same.\n";
        return -1;
    }

    int *in1 = new int[ds1];
    int *in2 = new int[ds1];
    double res = 0.0;

    for (int i = 0; i < ds1; i++) {
        ifh1 >> in1[i];
        ifh2 >> in2[i];
        res += ((in1[i]-in2[i])*(in1[i]-in2[i]));
    }

    res = sqrt(res);
    cout << argv[1] << " " << argv[2] << " " << res << endl;
    return 0; 
}
