#pragma once

#include <iostream>
#include "Multiset.hpp"

using namespace std;


class UI {
public:

	UI(Multiset& A, Multiset& B) : A(A), B(B) {}
	bool canUseOp = false;
	void run();
private:
	Multiset& A;
	Multiset& B;

	void printMenu();
	void printOpMenu();
	void printSetMenu();

	void handleGenerateUniverse();
    void handleManualFill();
    void handleAutoFill();
    void handleSetMenu();
    void handleOpMenu();
};
