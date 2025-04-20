/**
 * Student Information System - GUI Version
 */

#include "student.h"    // Include the student header file with structure definitions
#include <iostream>     // Include for standard input/output operations

// External function declaration for GUI interface
extern void runGUI();   // This function is defined in gui_interface.cpp

/**
 * Main function for the GUI version of the application
 */
int main() {
    // Run the GUI version of the application
    runGUI();           // This launches the ncurses interface
    
    // This point is reached only if the GUI exits normally without using exit()
    return 0;           // Return success code
}