#include "student.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Student students[MAX_STUDENTS];
int numStudents = 0;

int main() {
    loadStudents();
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: modifyStudent(); break;
            case 4: searchStudent(); break;
            case 5: displayStudents(true); break;
            case 6: displayStudents(false); break;
            case 7: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                int index = findStudentById(id);
                if(index != -1) manageCourses(index);
                else cout << "Student not found!\n";
                break;
            }
            case 8: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                int index = findStudentById(id);
                if(index != -1) computeGPA(index);
                else cout << "Student not found!\n";
                break;
            }
            case 9: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                int index = findStudentById(id);
                if(index != -1) updateStudyPlan(index);
                else cout << "Student not found!\n";
                break;
            }
            case 10: saveStudents(); break;
        }
    } while(choice != 10);

    return 0;
}

void displayMenu() {
    cout << "\nStudent Information System\n";
    cout << "1. Add Student\n";
    cout << "2. Delete Student\n";
    cout << "3. Modify Student\n";
    cout << "4. Search Student\n";
    cout << "5. Display Students (Sorted by ID)\n";
    cout << "6. Display Students (Sorted by Name)\n";
    cout << "7. Manage Courses\n";
    cout << "8. Compute GPA\n";
    cout << "9. Update Study Plan\n";
    cout << "10. Exit\n";
    cout << "Enter choice: ";
}