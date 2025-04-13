#include "student.h"
#include <iostream>
#include <algorithm>

int findStudentById(int id) {
    for(int i = 0; i < numStudents; i++) {
        if(students[i].id == id) return i;
    }
    return -1;
}

bool validateNationalId(const string& nationalId) {
    if(nationalId.length() != 14) return false;
    for(char c : nationalId) 
        if(!isdigit(c)) return false;
    return true;
}

void addStudent() {
    if(numStudents >= MAX_STUDENTS) {
        cout << "Maximum students reached!\n";
        return;
    }

    Student newStudent;
    cout << "Enter Student ID: ";
    cin >> newStudent.id;
    cin.ignore();
    
    if(findStudentById(newStudent.id) != -1) {
        cout << "ID already exists!\n";
        return;
    }

    cout << "Enter Name: ";
    getline(cin, newStudent.name);
    
    do {
        cout << "Enter National ID (14 digits): ";
        getline(cin, newStudent.nationalId);
    } while(!validateNationalId(newStudent.nationalId));

    newStudent.numCourses = 0;
    newStudent.gpa = 0.0;
    newStudent.numStudyPlan = 0;

    students[numStudents++] = newStudent;
    cout << "Student added successfully!\n";
}

void deleteStudent() {
    int id;
    cout << "Enter student ID to delete: ";
    cin >> id;
    
    int index = findStudentById(id);
    if(index == -1) {
        cout << "Student not found!\n";
        return;
    }

    // Confirm deletion
    char confirm;
    cout << "Are you sure you want to delete student " << students[index].name << "? (y/n): ";
    cin >> confirm;
    
    if(confirm != 'y' && confirm != 'Y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    // Shift remaining elements
    for(int i = index; i < numStudents-1; i++) {
        students[i] = students[i+1];
    }
    numStudents--;
    cout << "Student deleted successfully!\n";
}

void modifyStudent() {
    int id;
    cout << "Enter student ID to modify: ";
    cin >> id;
    cin.ignore();
    
    int index = findStudentById(id);
    if(index == -1) {
        cout << "Student not found!\n";
        return;
    }

    Student& s = students[index];
    cout << "Current name: " << s.name << "\nEnter new name (or press enter to keep): ";
    string newName;
    getline(cin, newName);
    if(!newName.empty()) s.name = newName;

    string newNationalId;
    do {
        cout << "Current national ID: " << s.nationalId 
             << "\nEnter new national ID (14 digits, or press enter to keep): ";
        getline(cin, newNationalId);
        if(newNationalId.empty()) break;
        if(validateNationalId(newNationalId)) {
            s.nationalId = newNationalId;
            break;
        }
        cout << "Invalid national ID format!\n";
    } while(true);
    
    cout << "Student information updated successfully!\n";
}

void searchStudent() {
    int choice;
    cout << "Search by:\n1. ID\n2. Name\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    if(choice == 1) {
        int id;
        cout << "Enter student ID: ";
        cin >> id;
        int index = findStudentById(id);
        if(index != -1) {
            cout << "Student found:\n";
            cout << "ID: " << students[index].id << "\nName: " << students[index].name 
                 << "\nNational ID: " << students[index].nationalId 
                 << "\nNumber of Courses: " << students[index].numCourses
                 << "\nGPA: " << students[index].gpa << "\n";
        } else {
            cout << "Student not found!\n";
        }
    } else if(choice == 2) {
        string name;
        cout << "Enter student name (or part of name): ";
        getline(cin, name);
        bool found = false;
        
        for(int i = 0; i < numStudents; i++) {
            // Case-insensitive search
            string studentNameLower = students[i].name;
            string searchNameLower = name;
            transform(studentNameLower.begin(), studentNameLower.end(), studentNameLower.begin(), ::tolower);
            transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);
            
            if(studentNameLower.find(searchNameLower) != string::npos) {
                cout << "ID: " << students[i].id << "\nName: " << students[i].name 
                     << "\nNational ID: " << students[i].nationalId 
                     << "\nNumber of Courses: " << students[i].numCourses
                     << "\nGPA: " << students[i].gpa << "\n\n";
                found = true;
            }
        }
        if(!found) cout << "No matching students found!\n";
    } else {
        cout << "Invalid choice!\n";
    }
}

void displayStudents(bool sortedById) {
    if(numStudents == 0) {
        cout << "No students to display!\n";
        return;
    }

    // Create a temporary array for sorting
    Student tempStudents[MAX_STUDENTS];
    copy(students, students + numStudents, tempStudents);

    if(sortedById) {
        sort(tempStudents, tempStudents + numStudents, 
            [](const Student& a, const Student& b) { return a.id < b.id; });
        cout << "\nStudent List (Sorted by ID):\n";
    } else {
        sort(tempStudents, tempStudents + numStudents, 
            [](const Student& a, const Student& b) { return a.name < b.name; });
        cout << "\nStudent List (Sorted by Name):\n";
    }

    // Print table header
    cout << "----------------------------------------\n";
    cout << "ID\tName\t\t\tNational ID\tGPA\n";
    cout << "----------------------------------------\n";
    
    for(int i = 0; i < numStudents; i++) {
        cout << tempStudents[i].id << "\t";
        
        // Ensure proper formatting for name (adjust tabs based on name length)
        if(tempStudents[i].name.length() < 8)
            cout << tempStudents[i].name << "\t\t\t";
        else if(tempStudents[i].name.length() < 16)
            cout << tempStudents[i].name << "\t\t";
        else
            cout << tempStudents[i].name << "\t";
            
        cout << tempStudents[i].nationalId << "\t"
             << tempStudents[i].gpa << "\n";
    }
    cout << "----------------------------------------\n";
    cout << "Total students: " << numStudents << "\n";
}

void manageCourses(int index) {
    Student& s = students[index];
    int choice;
    
    do {
        cout << "\nCourse Management for " << s.name << "\n";
        cout << "1. Add Course\n2. Remove Course\n3. View Courses\n4. Back\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                if(s.numCourses >= MAX_COURSES) {
                    cout << "Maximum courses reached!\n";
                    break;
                }
                cout << "Enter course name: ";
                getline(cin, s.courses[s.numCourses].name);
                
                do {
                    cout << "Enter grade (0-100): ";
                    cin >> s.courses[s.numCourses].grade;
                    cin.ignore();
                    
                    if(s.courses[s.numCourses].grade < 0 || s.courses[s.numCourses].grade > 100) {
                        cout << "Invalid grade! Must be between 0 and 100.\n";
                    } else {
                        break;
                    }
                } while(true);
                
                s.numCourses++;
                computeGPA(index);
                cout << "Course added!\n";
                break;
                
            case 2:
                if(s.numCourses == 0) {
                    cout << "No courses to remove!\n";
                    break;
                }
                cout << "Courses:\n";
                for(int i = 0; i < s.numCourses; i++) {
                    cout << i+1 << ". " << s.courses[i].name 
                         << " (" << s.courses[i].grade << ")\n";
                }
                int courseIndex;
                cout << "Enter course number to remove: ";
                cin >> courseIndex;
                if(courseIndex < 1 || courseIndex > s.numCourses) {
                    cout << "Invalid selection!\n";
                    break;
                }
                for(int i = courseIndex-1; i < s.numCourses-1; i++) {
                    s.courses[i] = s.courses[i+1];
                }
                s.numCourses--;
                computeGPA(index);
                cout << "Course removed!\n";
                break;
                
            case 3:
                if(s.numCourses == 0) {
                    cout << "No courses registered for " << s.name << ".\n";
                    break;
                }
                
                cout << "\nCourses for " << s.name << ":\n";
                cout << "------------------------\n";
                cout << "No.\tCourse Name\tGrade\n";
                cout << "------------------------\n";
                for(int i = 0; i < s.numCourses; i++) {
                    cout << i+1 << ".\t" << s.courses[i].name << "\t\t" 
                         << s.courses[i].grade << "\n";
                }
                cout << "------------------------\n";
                cout << "GPA: " << s.gpa << "\n";
                break;
                
            case 4:
                cout << "Returning to main menu.\n";
                break;
                
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 4);
}

void updateStudyPlan(int index) {
    Student& s = students[index];
    int choice;
    
    do {
        cout << "\nStudy Plan Management for " << s.name << "\n";
        cout << "1. Add Course to Plan\n2. Remove Course from Plan\n3. View Plan\n4. Back\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                if(s.numStudyPlan >= MAX_STUDY_PLAN) {
                    cout << "Study plan is full!\n";
                    break;
                }
                cout << "Enter course name for study plan: ";
                getline(cin, s.studyPlan[s.numStudyPlan]);
                s.numStudyPlan++;
                cout << "Course added to study plan!\n";
                break;
                
            case 2:
                if(s.numStudyPlan == 0) {
                    cout << "Study plan is empty!\n";
                    break;
                }
                cout << "Study Plan:\n";
                for(int i = 0; i < s.numStudyPlan; i++) {
                    cout << i+1 << ". " << s.studyPlan[i] << "\n";
                }
                int planIndex;
                cout << "Enter item number to remove: ";
                cin >> planIndex;
                if(planIndex < 1 || planIndex > s.numStudyPlan) {
                    cout << "Invalid selection!\n";
                    break;
                }
                for(int i = planIndex-1; i < s.numStudyPlan-1; i++) {
                    s.studyPlan[i] = s.studyPlan[i+1];
                }
                s.numStudyPlan--;
                cout << "Course removed from study plan!\n";
                break;
                
            case 3:
                if(s.numStudyPlan == 0) {
                    cout << "Study plan is empty for " << s.name << ".\n";
                    break;
                }
                
                cout << "\nStudy Plan for " << s.name << ":\n";
                cout << "------------------------\n";
                for(int i = 0; i < s.numStudyPlan; i++) {
                    cout << i+1 << ". " << s.studyPlan[i] << "\n";
                }
                cout << "------------------------\n";
                cout << "Total courses in plan: " << s.numStudyPlan << "\n";
                break;
                
            case 4:
                cout << "Returning to main menu.\n";
                break;
                
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 4);
}
