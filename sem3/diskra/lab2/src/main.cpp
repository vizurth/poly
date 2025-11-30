#include <iostream>
#include <vector>
#include <string>

#include "../include/bdd.h"
#include "../include/help.h"
#include "../include/zheg.h"
#include "../include/config.h"
#include "../include/ui.h"

using namespace std;

int main() {
    UI ui(vars, F);
    
    ui.run();
    
    return 0;
}