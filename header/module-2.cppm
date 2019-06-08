#include <cstddef>

export module sizes;

export {
	template <typename type> size_t word_sizeof(void) {
		return sizeof(type)/sizeof(int);
	}
	template <typename type> size_t word_sizeof(type var) {
		return sizeof(var)/sizeof(int);
	}
}
