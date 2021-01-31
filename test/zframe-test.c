#include <czmq.h>

int main(int argc, char const * const * const argv)
{
	zframe_t * frame = zframe_new("this is a test frame", 20);
	zframe_print(frame, "");
	zframe_destroy(&frame);
	return 0;
}
