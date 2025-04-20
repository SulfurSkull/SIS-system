/**
 * Student Operations Module
 * 
 * This file contains functions for managing student records including
 * adding, deleting, searching, and modifying student information.
 */

#include "student.h"    // Include student structure definitions
#include <iostream>     // Include for input/output operations
#include <algorithm>    // Include for sort and other algorithms
#include <string>       // Include for string operations

/**
 * Find a student by their ID in the student array
 * 
 * @param id The ID of the student to find
 * @return The index of the student in the array, or -1 if not found
 */
int findStudentById(int id) {
    // Linear search through the student array
    for(int i = 0; i < numStudents; i++) {    // Iterate through each student
        if(students[i].id == id) return i;    // If ID matches, return the index
    }
    return -1;                                // Return -1 if student not found
}

/**
 * Validate the format of a national ID
 * 
 * @param nationalId The national ID string to validate
 * @return true if the ID is valid, false otherwise
 */
bool validateNationalId(const string& nationalId) {
    // Check length (must be 14 digits)
    if(nationalId.length() != 14) return false;    // Incorrect length
    
    // Check that all characters are digits
    for(char c : nationalId)                      // Iterate through each character
        if(!isdigit(c)) return false;             // If not a digit, return false
    
    return true;                                  // All checks passed, ID is valid
}

/**
 * Add a new student to the system
 */
void addStudent() {
    // Check if maximum number of students reached
    if(numStudents >= MAX_STUDENTS) {
        cout << "Maximum students reached!\n";    // Display error message
        return;                                  // Exit function
    }

    Student newStudent;                          // Create a new student object
    
    // Get student ID
    cout << "Enter Student ID: ";                // Prompt for ID
    cin >> newStudent.id;                        // Read ID from user
    cin.ignore();                               // Clear input buffer
    
    // Check if ID already exists
    if(findStudentById(newStudent.id) != -1) {   // Search for existing ID
        cout << "ID already exists!\n";          // Display error if found
        return;                                  // Exit function
    }

    // Get student name
    cout << "Enter Name: ";                      // Prompt for name
    getline(cin, newStudent.name);              // Read name including spaces
    
    // Get national ID with validation
    do {
        cout << "Enter National ID (14 digits): ";    // Prompt for national ID
        getline(cin, newStudent.nationalId);         // Read national ID
    } while(!validateNationalId(newStudent.nationalId));  // Repeat until valid

    // Add student to array and increment count
    students[numStudents++] = newStudent;       // Store new student and increment count
    cout << "Student added successfully!\n";     // Confirm success to user
}

/**
 * Delete a student from the system
 */
void deleteStudent() {
    int id;                                     // Variable to store student ID
    cout << "Enter student ID to delete: ";      // Prompt for ID
    cin >> id;                                  // Read ID from user
    
    // Find the student
    int index = findStudentById(id);            // Get index of student by ID
    if(index == -1) {                           // If student not found
        cout << "Student not found!\n";          // Display error message
        return;                                 // Exit function
    }

    // Shift remaining elements to remove the student
    for(int i = index; i < numStudents-1; i++) {   // Iterate from found index to end
        students[i] = students[i+1];              // Shift each student down one position
    }
    numStudents--;                               // Decrement student count
    cout << "Student deleted successfully!\n";    // Confirm deletion to user
}

/**
 * Modify an existing student's information
 */
void modifyStudent() {
    int id;                                      // Variable to store student ID
    cout << "Enter student ID to modify: ";       // Prompt for ID
    cin >> id;                                   // Read ID from user
    cin.ignore();                                // Clear input buffer
    
    // Find the student
    int index = findStudentById(id);             // Get index of student by ID
    if(index == -1) {                            // If student not found
        cout << "Student not found!\n";           // Display error message
        return;                                  // Exit function
    }

    // Get reference to student for easier access
    Student& s = students[index];                // Reference to the student to modify
    
    // Modify name (if user provides a new one)
    cout << "Current name: " << s.name << "\nEnter new name (or press enter to keep): ";
    string newName;                              // Variable for new name
    getline(cin, newName);                       // Read new name (or empty line)
    if(!newName.empty()) s.name = newName;       // Update name if input not empty

    // Modify national ID (with validation)
    string newNationalId;                        // Variable for new national ID
    do {
        cout << "Current national ID: " << s.nationalId 
             << "\nEnter new national ID (14 digits, or press enter to keep): ";
        getline(cin, newNationalId);             // Read new national ID
        if(newNationalId.empty()) break;         // Keep current if input empty
        if(validateNationalId(newNationalId)) {  // Validate new national ID
            s.nationalId = newNationalId;        // Update if valid
            break;                              // Exit loop
        }
        cout << "Invalid national ID format!\n";  // Show error for invalid format
    } while(true);                               // Continue until valid or skipped
}

/**
 * Search for students by ID or name
 */
void searchStudent() {
    int choice;                                  // Variable for search method choice
    cout << "Search by:\n1. ID\n2. Name\nEnter choice: ";  // Display search options
    cin >> choice;                               // Get user choice
    cin.ignore();                                // Clear input buffer

    // Search by ID
    if(choice == 1) {
        int id;                                  // Variable for student ID
        cout << "Enter student ID: ";            // Prompt for ID
        cin >> id;                              // Read ID from user
        int index = findStudentById(id);         // Find student by ID
        
        // Display results
        if(index != -1) {                       // If student found
            cout << "Student found:\n";          // Display success message
            cout << "ID: " << students[index].id           // Display student ID
                 << "\nName: " << students[index].name     // Display student name
                 << "\nNational ID: " << students[index].nationalId << "\n";  // Display national ID
        } else {
            cout << "Student not found!\n";      // Display not found message
        }
    } 
    // Search by name (partial match)
    else if(choice == 2) {
        string name;                             // Variable for search term
        cout << "Enter student name: ";          // Prompt for name
        getline(cin, name);                     // Read name from user
        bool found = false;                      // Flag to track if any matches found
        
        // Search all students for name match
        for(int i = 0; i < numStudents; i++) {   // Iterate through all students
            // Check if search term is found anywhere in the name
            if(students[i].name.find(name) != string::npos) {  // If name contains search term
                // Display matching student
                cout << "ID: " << students[i].id                // Display student ID
                     << "\nName: " << students[i].name          // Display student name
                     << "\nNational ID: " << students[i].nationalId << "\n\n";  // Display national ID
                found = true;                   // Set found flag
            }
        }
        if(!found) cout << "No matching students found!\n";  // If no matches found
    } 
    // Invalid choice
    else {
        cout << "Invalid choice!\n";            // Display error for invalid choice
    }
}

/**
 * Display a list of all students sorted by ID or name
 * 
 * @param sortedById If true, sort by ID; otherwise sort by name
 */
void displayStudents(bool sortedById) {
    // Check if there are students to display
    if(numStudents == 0) {                      // If no students in system
        cout << "No students to display!\n";     // Display message
        return;                                 // Exit function
    }

    // Create a temporary array for sorting (to keep original order intact)
    Student tempStudents[MAX_STUDENTS];          // Temporary array for sorted display
    copy(students, students + numStudents, tempStudents);  // Copy all students to temp array

    // Sort the temporary array
    if(sortedById) {
        // Sort by ID
        sort(tempStudents, tempStudents + numStudents,     // Sort range: temp array
            [](const Student& a, const Student& b) { return a.id < b.id; });  // Compare by ID
    } else {
        // Sort by name
        sort(tempStudents, tempStudents + numStudents,     // Sort range: temp array
            [](const Student& a, const Student& b) { return a.name < b.name; });  // Compare by name
    }

    // Display the sorted list
    cout << "\nStudent List:\n";                // Display header
    cout << "ID\tName\t\tNational ID\tGPA\n";    // Display column headers
    for(int i = 0; i < numStudents; i++) {      // Iterate through sorted students
        cout << tempStudents[i].id << "\t"       // Display ID with tab
             << tempStudents[i].name << "\t"     // Display name with tab
             << tempStudents[i].nationalId << "\t"  // Display national ID with tab
             << tempStudents[i].gpa << "\n";     // Display GPA with newline
    }
}

/**
 * Manage courses for a specific student
 * 
 * @param index The index of the student in the array
 */
void manageCourses(int index) {
    Student& s = students[index];               // Reference to the student
    int choice;                                 // Variable for menu choice
    
    // Course management menu loop
    do {
        // Display menu options
        cout << "\nCourse Management for " << s.name << "\n";
        cout << "1. Add Course\n2. Remove Course\n3. View Courses\n4. Back\nChoice: ";
        cin >> choice;                          // Get user choice
        cin.ignore();                           // Clear input buffer

        // Process menu choice
        switch(choice) {
            case 1:  // Add a course
                // Check if max courses reached
                if(s.numCourses >= MAX_COURSES) {
                    cout << "Maximum courses reached!\n";  // Display error
                    break;                              // Exit case
                }
                
                // Get course details
                cout << "Enter course name: ";              // Prompt for course name
                getline(cin, s.courses[s.numCourses].name);  // Read course name
                cout << "Enter grade (0-100): ";           // Prompt for grade
                cin >> s.courses[s.numCourses].grade;      // Read grade
                cin.ignore();                             // Clear input buffer
                
                // Validate grade range
                if(s.courses[s.numCourses].grade < 0 || s.courses[s.numCourses].grade > 100) {
                    cout << "Invalid grade! Course not added.\n";  // Display error
                    break;                                // Exit case
                }
                
                // Add course and update GPA
                s.numCourses++;                            // Increment course count
                computeGPA(index);                         // Recalculate GPA
                cout << "Course added!\n";                  // Confirm addition
                break;
                
            case 2:  // Remove a course
                // Check if student has courses
                if(s.numCourses == 0) {
                    cout << "No courses to remove!\n";      // Display error
                    break;                                // Exit case
                }
                
                // Display course list
                cout << "Courses:\n";
                for(int i = 0; i < s.numCourses; i++) {    // Iterate through courses
                    cout << i+1 << ". " << s.courses[i].name    // Display course number and name
                         << " (" << s.courses[i].grade << ")\n";  // Display grade in parentheses
                }
                
                // Get course to remove
                int courseIndex;                            // Variable for course index
                cout << "Enter course number to remove: ";   // Prompt for selection
                cin >> courseIndex;                         // Read selection
                
                // Validate selection
                if(courseIndex < 1 || courseIndex > s.numCourses) {
                    cout << "Invalid selection!\n";         // Display error
                    break;                                 // Exit case
                }
                
                // Remove course (shift remaining courses)
                for(int i = courseIndex-1; i < s.numCourses-1; i++) {  // Start from selected course
                    s.courses[i] = s.courses[i+1];          // Shift each course down
                }
                s.numCourses--;                             // Decrement course count
                computeGPA(index);                          // Recalculate GPA
                cout << "Course removed!\n";                 // Confirm removal
                break;
                
            case 3:  // View courses
                // Display all courses
                cout << "\nCourses for " << s.name << ":\n";  // Display header with student name
                for(int i = 0; i < s.numCourses; i++) {      // Iterate through courses
                    cout << "- " << s.courses[i].name << ": "   // Display course name
                         << s.courses[i].grade << "\n";        // Display grade
                }
                cout << "GPA: " << s.gpa << "\n";            // Display current GPA
                break;
        }
    } while(choice != 4);  // Continue until Back option selected
}

/**
 * Update study plan for a specific student
 * 
 * @param index The index of the student in the array
 */
void updateStudyPlan(int index) {
    Student& s = students[index];                  // Reference to the student
    int choice;                                    // Variable for menu choice
    
    // Study plan management menu loop
    do {
        // Display menu options
        cout << "\nStudy Plan Management for " << s.name << "\n";
        cout << "1. Add Course to Plan\n2. Remove Course from Plan\n3. View Plan\n4. Back\nChoice: ";
        cin >> choice;                             // Get user choice
        cin.ignore();                              // Clear input buffer

        // Process menu choice
        switch(choice) {
            case 1:  // Add a course to plan
                // Check if plan is full
                if(s.numStudyPlan >= MAX_STUDY_PLAN) {
                    cout << "Study plan is full!\n";         // Display error
                    break;                                  // Exit case
                }
                
                // Get course name
                cout << "Enter course name: ";               // Prompt for course name
                getline(cin, s.studyPlan[s.numStudyPlan]);  // Read course name
                s.numStudyPlan++;                           // Increment plan count
                cout << "Course added to study plan!\n";     // Confirm addition
                break;
                
            case 2:  // Remove a course from plan
                // Check if plan is empty
                if(s.numStudyPlan == 0) {
                    cout << "Study plan is empty!\n";        // Display error
                    break;                                  // Exit case
                }
                
                // Display study plan
                cout << "Study Plan:\n";
                for(int i = 0; i < s.numStudyPlan; i++) {   // Iterate through plan
                    cout << i+1 << ". " << s.studyPlan[i] << "\n";  // Display numbered item
                }
                
                // Get item to remove
                int planIndex;                              // Variable for plan index
                cout << "Enter item number to remove: ";     // Prompt for selection
                cin >> planIndex;                           // Read selection
                
                // Validate selection
                if(planIndex < 1 || planIndex > s.numStudyPlan) {
                    cout << "Invalid selection!\n";          // Display error
                    break;                                  // Exit case
                }
                
                // Remove item (shift remaining items)
                for(int i = planIndex-1; i < s.numStudyPlan-1; i++) {  // Start from selected item
                    s.studyPlan[i] = s.studyPlan[i+1];      // Shift each item down
                }
                s.numStudyPlan--;                           // Decrement plan count
                cout << "Course removed from study plan!\n"; // Confirm removal
                break;
                
            case 3:  // View study plan
                // Display all plan items
                cout << "\nStudy Plan for " << s.name << ":\n";  // Display header with student name
                for(int i = 0; i < s.numStudyPlan; i++) {       // Iterate through plan
                    cout << "- " << s.studyPlan[i] << "\n";      // Display each item
                }
                break;
        }
    } while(choice != 4);  // Continue until Back option selected
}