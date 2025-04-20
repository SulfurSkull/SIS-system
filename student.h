/**
 * student.h - Header file for Student Information System
 * 
 * This file contains the data structures and function declarations
 * for the Student Information System.
 */

#ifndef STUDENT_H    // Guard against multiple inclusion
#define STUDENT_H    // Define the include guard

#include <string>    // Include string library for string data type
using namespace std; // Use the standard namespace

// Constants for the application
const int MAX_STUDENTS = 100;            // Maximum number of students in the system
const int MAX_COURSES = 10;              // Maximum number of courses per student
const int MAX_STUDY_PLAN = 20;           // Maximum number of study plan entries
const char FILENAME[] = "students.txt";  // File name for persistent storage

/**
 * Course structure - Represents a single course with name and grade
 */
struct Course {
    string name;     // Name of the course
    float grade;     // Grade achieved in the course (numeric value)
};

/**
 * Student structure - Main data structure for a student record
 */
struct Student {
    int id;          // Unique identifier for the student
    string name;     // Full name of the student
    string nationalId; // National ID number (14 digits)
    Course courses[MAX_COURSES]; // Array of courses taken by student
    int numCourses = 0;        // Number of courses currently registered
    float gpa = 0.0;           // Grade Point Average (0.0-4.0 scale)
    string studyPlan[MAX_STUDY_PLAN]; // Array of study plan entries
    int numStudyPlan = 0;      // Number of study plan entries
};

// External variable declarations
extern Student students[MAX_STUDENTS]; // Global array of students
extern int numStudents;                // Current number of students

// Function prototypes

// File operations
void loadStudents();     // Load students from file
void saveStudents();     // Save students to file

// Student management operations
void addStudent();       // Add a new student
void deleteStudent();    // Delete an existing student
void modifyStudent();    // Modify student information
void searchStudent();    // Search for a student

// User interface functions
void displayMenu();      // Display the main menu
void displayStudents(bool sortedById); // Display all students

// Academic management
void manageCourses(int index);    // Manage courses for a student
void computeGPA(int index);       // Calculate GPA for a student
void updateStudyPlan(int index);  // Update study plan for a student

// Utility functions
int findStudentById(int id);      // Find a student by ID
bool validateNationalId(const string& nationalId); // Validate national ID format
void sortStudentsById();          // Sort students by ID
void sortStudentsByName();        // Sort students by name
float convertGradeTo4Scale(float numericGrade); // Convert grade to 4.0 scale

#endif // End of STUDENT_H include guard
