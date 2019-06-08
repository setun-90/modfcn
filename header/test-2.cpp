#include "modfcn.h"
#include <iostream>
#include <cstddef>

using namespace std;

int main(void) {

	mod m("./module-2.pcm");

	size_t (*sf)(void)(m.polysym<size_t (*)<long>(void)>("word_sizeof"));
	cout << "A long is " << sf() << " words on this arch\n";

	return 0;
}
