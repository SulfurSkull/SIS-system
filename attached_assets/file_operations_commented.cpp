#include "student.h"    // Include student structure definitions
#include <fstream>      // Include for file input/output operations
#include <sstream>      // Include for string stream operations

/**
 * Load student data from file into memory
 */
void loadStudents() {
    ifstream file(FILENAME);                    // Open the file for reading
    if (!file.is_open()) return;               // If file can't be opened, exit function
    
    string line;                               // Variable to hold each line from file
    while (getline(file, line) && numStudents < MAX_STUDENTS) {  // Read each line until EOF or max students reached
        Student s;                             // Create a new student object
        stringstream ss(line);                 // Create string stream from current line
        string token;                          // Variable to store parsed tokens
        
        // Read ID - first value in the comma-separated line
        getline(ss, token, ',');              // Extract token until comma
        s.id = stoi(token);                   // Convert string to integer for ID
        
        // Read Name - second value in the line
        getline(ss, s.name, ',');             // Extract name until next comma
        
        // Read National ID - third value in the line
        getline(ss, s.nationalId, ',');       // Extract national ID until next comma
        
        // Read Courses - starts with number of courses
        getline(ss, token, ',');              // Get number of courses
        s.numCourses = stoi(token);           // Convert to integer
        for(int i = 0; i < s.numCourses; i++) {  // Loop through each course
            getline(ss, token, ':');          // Get course name (delimited by colon)
            s.courses[i].name = token;        // Store course name
            getline(ss, token, ',');          // Get course grade (delimited by comma)
            s.courses[i].grade = stof(token); // Convert grade to float
        }
        
        // Read GPA - after courses
        getline(ss, token, ',');              // Extract GPA value
        s.gpa = stof(token);                  // Convert to float
        
        // Read Study Plan - starts with number of study plan entries
        getline(ss, token, ',');              // Get number of study plan entries
        s.numStudyPlan = stoi(token);         // Convert to integer
        for(int i = 0; i < s.numStudyPlan; i++) {  // Loop through each study plan entry
            getline(ss, s.studyPlan[i], ','); // Extract study plan item (delimited by comma)
        }
        
        students[numStudents++] = s;          // Add student to array and increment count
    }
    file.close();                             // Close the file
}

/**
 * Save student data from memory to file
 */
void saveStudents() {
    ofstream file(FILENAME);                  // Open file for writing (overwrites existing file)
    for(int i = 0; i < numStudents; i++) {    // Loop through all students
        Student& s = students[i];             // Get reference to current student
        
        // Write basic student info (ID, name, national ID, number of courses)
        file << s.id << "," << s.name << "," << s.nationalId << ","
             << s.numCourses << ",";          // Write as CSV format
        
        // Write course information (name:grade pairs)
        for(int j = 0; j < s.numCourses; j++) {  // Loop through each course
            file << s.courses[j].name << ":" << s.courses[j].grade << ",";  // Course name:grade format
        }
        
        // Write GPA and study plan information
        file << s.gpa << "," << s.numStudyPlan << ",";  // GPA and number of study plan entries
        for(int j = 0; j < s.numStudyPlan; j++) {       // Loop through study plan entries
            file << s.studyPlan[j] << ",";              // Write each study plan item
        }
        file << "\n";                         // End of student record with newline
    }
    file.close();                             // Close the file
}