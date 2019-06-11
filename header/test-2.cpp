#include "modfcn.h"
#include <iostream>
#include <cstddef>

using namespace std;

int main(void) {

	mod m("./module-2.pcm");
	auto sf((size_t (*)(void))m.polysym("size_t word_sizeof<long>(void)"));
	cout << "A long is " << sf() << " words on this arch\n";

	return 0;
}
