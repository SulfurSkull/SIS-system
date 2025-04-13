#include "student.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// Convert numerical grade (0-100) to 4.0 scale
float convertGradeTo4Scale(float numericGrade) {
    // Convert percentage grade to 4.0 scale
    if (numericGrade >= 93.0) return 4.00;      // A
    else if (numericGrade >= 90.0) return 3.70; // A-
    else if (numericGrade >= 87.0) return 3.33; // B+
    else if (numericGrade >= 83.0) return 3.00; // B
    else if (numericGrade >= 80.0) return 2.70; // B-
    else if (numericGrade >= 77.0) return 2.30; // C+
    else if (numericGrade >= 73.0) return 2.00; // C
    else if (numericGrade >= 70.0) return 1.70; // C-
    else if (numericGrade >= 67.0) return 1.30; // D+
    else if (numericGrade >= 63.0) return 1.00; // D
    else if (numericGrade >= 60.0) return 0.70; // D-
    else return 0.00;                           // F
}

void computeGPA(int index) {
    Student& s = students[index];
    if(s.numCourses == 0) {
        s.gpa = 0.0;
        cout << "No courses available. GPA set to 0.0\n";
        return;
    }

    float total = 0.0;
    for(int i = 0; i < s.numCourses; i++) {
        // Convert each course grade to 4.0 scale before adding
        total += convertGradeTo4Scale(s.courses[i].grade);
    }
    
    // Calculate GPA and ensure it doesn't exceed 4.0
    s.gpa = total / s.numCourses;
    s.gpa = min(s.gpa, 4.0f); // Cap at 4.0
    
    // Display GPA with 2 decimal places
    cout << "GPA updated: " << fixed << setprecision(2) << s.gpa << endl;
}
