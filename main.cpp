#include "Common.h"
#include "Klondike.h"

using namespace std;

int main(int argc, char **argv) {
	try {
		Klondike k;
		k.~Klondike();
	}
	catch (int exception) {}
	return 0;
}
