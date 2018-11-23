#include <math.h>

int ceil(double x) {
	int a = (int)x;
	if ((float)a != x) {
		return (int)(x + 1);
	}
	return (int)x;
}