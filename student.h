#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_COURSES = 10;
const int MAX_STUDY_PLAN = 20;
const char FILENAME[] = "students.txt";

struct Course {
    string name;
    float grade;
};

struct Student {
    int id;
    string name;
    string nationalId;
    Course courses[MAX_COURSES];
    int numCourses = 0;
    float gpa = 0.0;
    string studyPlan[MAX_STUDY_PLAN];
    int numStudyPlan = 0;
};

extern Student students[MAX_STUDENTS];
extern int numStudents;

// Function prototypes
void loadStudents();
void saveStudents();
void addStudent();
void deleteStudent();
void modifyStudent();
void searchStudent();
void displayMenu();
void displayStudents(bool sortedById);
void manageCourses(int index);
void computeGPA(int index);
void updateStudyPlan(int index);
int findStudentById(int id);
bool validateNationalId(const string& nationalId);
void sortStudentsById();
void sortStudentsByName();
#endif
