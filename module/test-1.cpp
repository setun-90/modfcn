import modfcn;

int main(int argc, char **argv) {
	mod m(argv[1]);

	void *some_sym(m.monosym(argv[2]));
}
