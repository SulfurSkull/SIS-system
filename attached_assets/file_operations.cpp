#include "student.h"
#include <fstream>
#include <sstream>

void loadStudents() {
    ifstream file(FILENAME);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line) && numStudents < MAX_STUDENTS) {
        Student s;
        stringstream ss(line);
        string token;

        // Read ID
        getline(ss, token, ',');
        s.id = stoi(token);

        // Read Name
        getline(ss, s.name, ',');

        // Read National ID
        getline(ss, s.nationalId, ',');

        // Read Courses
        getline(ss, token, ',');
        s.numCourses = stoi(token);
        for(int i = 0; i < s.numCourses; i++) {
            getline(ss, token, ':');
            s.courses[i].name = token;
            getline(ss, token, ',');
            s.courses[i].grade = stof(token);
        }

        // Read GPA
        getline(ss, token, ',');
        s.gpa = stof(token);

        // Read Study Plan
        getline(ss, token, ',');
        s.numStudyPlan = stoi(token);
        for(int i = 0; i < s.numStudyPlan; i++) {
            getline(ss, s.studyPlan[i], ',');
        }

        students[numStudents++] = s;
    }
    file.close();
}

void saveStudents() {
    ofstream file(FILENAME);
    for(int i = 0; i < numStudents; i++) {
        Student& s = students[i];
        file << s.id << "," << s.name << "," << s.nationalId << ","
             << s.numCourses << ",";
        
        for(int j = 0; j < s.numCourses; j++) {
            file << s.courses[j].name << ":" << s.courses[j].grade << ",";
        }
        
        file << s.gpa << "," << s.numStudyPlan << ",";
        for(int j = 0; j < s.numStudyPlan; j++) {
            file << s.studyPlan[j] << ",";
        }
        file << "\n";
    }
    file.close();
}