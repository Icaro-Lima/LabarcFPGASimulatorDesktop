#include <iostream>
#include <cmath>

#include "VMain.h"

VMain * vmain;

int main() {
	vmain = new VMain;

	string input;
	while (true) {
		cin >> input;
		int value = 0;
		for (int i = 0; i < 8; i++) {
			value += (input[i] - 48) * pow(2, i);
		}
		vmain->SWI = value;
		vmain->CLOCK = input[8] - 48;

		vmain->eval();

		value = (int)vmain->LED;
		for (int i = 0; i < 8; i++) {
			cout << value % 2;
			value /= 2;
		}
		value = (int)vmain->SEG;
		for (int i = 0; i < 8; i++) {
			cout << value % 2;
			value /= 2;
		}
		cout << endl;
	}

	exit(0);
}