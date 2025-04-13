#include "student.h"
#include <iostream>

void computeGPA(int index) {
    Student& s = students[index];
    if(s.numCourses == 0) {
        s.gpa = 0.0;
        cout << "No courses available. GPA set to 0.0\n";
        return;
    }

    float total = 0.0;
    for(int i = 0; i < s.numCourses; i++) {
        total += s.courses[i].grade;
    }
    s.gpa = total / s.numCourses;
    cout << "GPA updated: " << s.gpa << endl;
}
