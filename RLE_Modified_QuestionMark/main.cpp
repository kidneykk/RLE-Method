#include "RLE.h"

int main() {
	RLE rle;
	rle.encode("rrtyui1.bmp");
	rle.decode("rrtyui1.bmp.rle");

	return 0;
}