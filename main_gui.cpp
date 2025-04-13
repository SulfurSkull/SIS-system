#include "student.h"
#include <iostream>

// External function declaration for GUI
extern void runGUI();

int main() {
    // Run the GUI version of the application
    runGUI();
    
    // This point is reached only if the GUI exits normally
    return 0;
}