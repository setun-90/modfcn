#include "modfcn.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

	if (argc < 3) {
		cerr << "Usage: test-2 $module-file $symbol\n";
		return 0;
	}

	mod m(argv[1]);
	void *some_sym(m.monosym(argv[2]));

	return 0;
}
