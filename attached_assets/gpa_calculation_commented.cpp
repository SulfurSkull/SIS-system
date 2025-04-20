#include "student.h"    // Include student structure definitions
#include <iostream>     // Include for input/output operations
#include <iomanip>      // Include for output formatting (fixed, setprecision)
#include <algorithm>    // Include for min/max functions

/**
 * Convert numerical grade (0-100) to 4.0 scale
 * 
 * @param numericGrade The percentage grade (0-100)
 * @return The equivalent grade on 4.0 scale
 */
float convertGradeTo4Scale(float numericGrade) {
    // Convert percentage grade to 4.0 scale using standard grade scale
    if (numericGrade >= 93.0) return 4.00;      // A  (93-100) - 4.0 GPA
    else if (numericGrade >= 90.0) return 3.70; // A- (90-92.9) - 3.7 GPA
    else if (numericGrade >= 87.0) return 3.33; // B+ (87-89.9) - 3.33 GPA
    else if (numericGrade >= 83.0) return 3.00; // B  (83-86.9) - 3.0 GPA
    else if (numericGrade >= 80.0) return 2.70; // B- (80-82.9) - 2.7 GPA
    else if (numericGrade >= 77.0) return 2.30; // C+ (77-79.9) - 2.3 GPA
    else if (numericGrade >= 73.0) return 2.00; // C  (73-76.9) - 2.0 GPA
    else if (numericGrade >= 70.0) return 1.70; // C- (70-72.9) - 1.7 GPA
    else if (numericGrade >= 67.0) return 1.30; // D+ (67-69.9) - 1.3 GPA
    else if (numericGrade >= 63.0) return 1.00; // D  (63-66.9) - 1.0 GPA
    else if (numericGrade >= 60.0) return 0.70; // D- (60-62.9) - 0.7 GPA
    else return 0.00;                           // F  (0-59.9) - 0.0 GPA
}

/**
 * Compute the GPA for a student based on their courses
 * 
 * @param index The index of the student in the global array
 */
void computeGPA(int index) {
    Student& s = students[index];              // Get reference to student object
    
    // Check if student has any courses
    if(s.numCourses == 0) {                    // If no courses registered
        s.gpa = 0.0;                           // Set GPA to 0
        cout << "No courses available. GPA set to 0.0\n";  // Inform user
        return;                                // Exit the function
    }

    float total = 0.0;                        // Variable to store sum of grade points
    
    // Loop through all courses and sum up grade points
    for(int i = 0; i < s.numCourses; i++) {   // For each course
        // Convert each course grade to 4.0 scale before adding
        total += convertGradeTo4Scale(s.courses[i].grade);  // Add converted grade to total
    }
    
    // Calculate GPA by dividing total by number of courses
    s.gpa = total / s.numCourses;             // Calculate average grade point
    
    // Ensure GPA doesn't exceed 4.0 (cap it if necessary)
    s.gpa = min(s.gpa, 4.0f);                 // Cap at 4.0 (maximum possible GPA)
    
    // Display GPA with 2 decimal places for readability
    cout << "GPA updated: " << fixed << setprecision(2) << s.gpa << endl;  // Display formatted GPA
}