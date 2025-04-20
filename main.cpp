/**
 * Student Information System - Console Version
 */

#include "student.h"    // Include the student header file that contains declarations
#include <iostream>     // Include for input/output stream operations
#include <fstream>      // Include for file operations
#include <algorithm>    // Include for sorting algorithms

// Global array to store student data
Student students[MAX_STUDENTS];  // Array to hold the student records
int numStudents = 0;             // Counter for the current number of students

/**
 * Main function - entry point of the program
 */
int main() {
    loadStudents();     // Load existing student data from file
    int choice;         // Variable to store user menu choice
    
    // Main program loop
    do {
        displayMenu();          // Display the main menu options
        cin >> choice;          // Get user's choice
        cin.ignore();           // Clear input buffer
        
        // Process the user's choice
        switch(choice) {
            case 1: addStudent(); break;       // Add a new student
            case 2: deleteStudent(); break;    // Delete an existing student
            case 3: modifyStudent(); break;    // Modify student information
            case 4: searchStudent(); break;    // Search for a student
            case 5: displayStudents(true); break;   // Display students sorted by ID
            case 6: displayStudents(false); break;  // Display students sorted by name
            case 7: {
                // Manage courses for a specific student
                int id;                          // Variable to store student ID
                cout << "Enter student ID: ";    // Prompt for student ID
                cin >> id;                       // Get the ID from user
                int index = findStudentById(id); // Find student index by ID
                if(index != -1) manageCourses(index);  // Manage courses if student found
                else cout << "Student not found!\n";   // Display error if student not found
                break;
            }
            case 8: {
                // Compute GPA for a specific student
                int id;                          // Variable to store student ID
                cout << "Enter student ID: ";    // Prompt for student ID
                cin >> id;                       // Get the ID from user
                int index = findStudentById(id); // Find student index by ID
                if(index != -1) computeGPA(index);     // Compute GPA if student found
                else cout << "Student not found!\n";   // Display error if student not found
                break;
            }
            case 9: {
                // Update study plan for a specific student
                int id;                          // Variable to store student ID
                cout << "Enter student ID: ";    // Prompt for student ID
                cin >> id;                       // Get the ID from user
                int index = findStudentById(id); // Find student index by ID
                if(index != -1) updateStudyPlan(index); // Update study plan if student found
                else cout << "Student not found!\n";    // Display error if student not found
                break;
            }
            case 10: 
                saveStudents();                        // Save student data to file
                cout << "Data saved. Exiting program.\n"; // Inform user data is saved
                break;
            default:
                cout << "Invalid choice. Please try again.\n"; // Handle invalid menu choices
        }
    } while(choice != 10); // Continue until user chooses to exit

    return 0; // End program with success status
}

/**
 * Display the main menu options to the user
 */
void displayMenu() {
    cout << "\nStudent Information System\n";              // Display program title
    cout << "1. Add Student\n";                           // Option to add student
    cout << "2. Delete Student\n";                        // Option to delete student
    cout << "3. Modify Student\n";                        // Option to modify student
    cout << "4. Search Student\n";                        // Option to search student
    cout << "5. Display Students (Sorted by ID)\n";       // Option to display by ID
    cout << "6. Display Students (Sorted by Name)\n";     // Option to display by name
    cout << "7. Manage Courses\n";                        // Option to manage courses
    cout << "8. Compute GPA\n";                           // Option to compute GPA
    cout << "9. Update Study Plan\n";                     // Option to update study plan
    cout << "10. Exit\n";                                 // Option to exit program
    cout << "Enter choice: ";                             // Prompt for user input
}
