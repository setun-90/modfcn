#include "modfcn.h"
#include <exception>

using namespace std;

int main(void) {
	mod m("module-1.pcm");
	void (*func)(void)((void (*)(void))m.monosym("hello"));
	func();

	return 0;
}
