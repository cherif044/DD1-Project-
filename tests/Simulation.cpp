#include <iostream>
using namespace std;

bool circ1(bool A, bool B, bool C) {
	bool w1 = B && C;
	bool F = A || w1;
	return F;
}

bool circ2(bool A, bool B) {
	bool F = B && A;
	return F;
}
bool circ3(bool A, bool B, bool C) {
    bool w1 = A || B;
    bool F = C && w1;
    return F;
}
bool circ4(bool A, bool B) {
	bool w1 = !A;
	bool F = w1 ^ B; //xor
	return F;
}



bool circ5(bool A, bool B, bool C) {
	bool F = !(A && B && C);
	return F;

}
void printTruthTable1() {
    cout << "Truth Table for Circuit 1 (A OR (B AND C)):" << endl;
    cout << "A B C | F" << endl;
    cout << endl;
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            for (int C = 0; C < 2; C++) {
                cout << A << " " << B << " " << C << "  " << circ1(A, B, C) << endl;
            }
        }
    }
    cout << endl;
}

void printTruthTable2() {
    cout << "Truth Table for Circuit 2 (A AND B):" << endl;
    cout << "A B | F" << endl;
    cout  << endl;
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            cout << A << " " << B << "   " << circ2(A, B) << endl;
        }
    }
    cout << endl;
}



void printTruthTable4() {
    cout << "Truth Table for Circuit 4 (NOT A XOR B):" << endl;
    cout << "A B | F" << endl;
    cout  << endl;
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            cout << A << " " << B << "   " << circ4(A, B) << endl;
        }
    }
    cout << endl;
}

void printTruthTable3() {
    cout << "Truth Table for Circuit 3 (C OR (A OR B)):" << endl;
    cout << "A B C | F" << endl;
    cout << endl;
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            for (int C = 0; C < 2; C++) {
                cout << A << " " << B << " " << C << "   " << circ3(A, B, C) << endl;
            }
        }
    }
    cout << endl;
}
void printTruthTable5() {
    cout << "Truth Table for Circuit 5 (NOT (A AND B AND C)):" << endl;
    cout << "A B C | F" << endl;
    cout  << endl;
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            for (int C = 0; C < 2; C++) {
                cout << A << " " << B << " " << C << "   " << circ5(A, B, C) << endl;
            }
        }
    }
    cout << endl;
}

int main()
{
    printTruthTable1();
    printTruthTable2();
    printTruthTable3();
    printTruthTable4();
    printTruthTable5();
    return 0;
}
