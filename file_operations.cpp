#include "student.h"
#include <fstream>
#include <sstream>
#include <iostream>

void loadStudents() {
    ifstream file(FILENAME);
    if (!file.is_open()) {
        cout << "No existing student data found. Starting with empty database.\n";
        return;
    }

    string line;
    numStudents = 0; // Reset student count
    
    while (getline(file, line) && numStudents < MAX_STUDENTS) {
        if (line.empty()) continue; // Skip empty lines
        
        try {
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
                if (token.empty()) {
                    s.numCourses = 0; // Reset if there's an issue
                    break;
                }
                getline(ss, token, (i == s.numCourses - 1) ? ',' : ',');
                if (!token.empty()) {
                    s.courses[i].grade = stof(token);
                } else {
                    s.courses[i].grade = 0.0;
                }
            }

            // Read GPA
            getline(ss, token, ',');
            if (!token.empty()) {
                s.gpa = stof(token);
            } else {
                s.gpa = 0.0;
            }

            // Read Study Plan
            getline(ss, token, ',');
            if (!token.empty()) {
                s.numStudyPlan = stoi(token);
                for(int i = 0; i < s.numStudyPlan; i++) {
                    if (i == s.numStudyPlan - 1) {
                        getline(ss, s.studyPlan[i]);
                    } else {
                        getline(ss, s.studyPlan[i], ',');
                    }
                }
            } else {
                s.numStudyPlan = 0;
            }

            students[numStudents++] = s;
        } catch (const exception &e) {
            cout << "Error parsing student data: " << e.what() << endl;
            // Skip this record but continue with others
            continue;
        }
    }
    
    file.close();
    cout << numStudents << " students loaded from file.\n";
}

void saveStudents() {
    ofstream file(FILENAME);
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving.\n";
        return;
    }
    
    for(int i = 0; i < numStudents; i++) {
        try {
            Student& s = students[i];
            file << s.id << "," << s.name << "," << s.nationalId << ","
                 << s.numCourses << ",";
            
            // Write courses and grades
            if (s.numCourses > 0) {
                for(int j = 0; j < s.numCourses; j++) {
                    file << s.courses[j].name << ":" << s.courses[j].grade;
                    if (j < s.numCourses - 1) file << ",";
                }
            }
            
            file << "," << s.gpa << "," << s.numStudyPlan << ",";
            
            // Write study plan
            if (s.numStudyPlan > 0) {
                for(int j = 0; j < s.numStudyPlan; j++) {
                    file << s.studyPlan[j];
                    if (j < s.numStudyPlan - 1) file << ",";
                }
            }
            file << "\n";
        } catch (const exception &e) {
            cout << "Error saving student data: " << e.what() << endl;
            // Continue with next student
        }
    }
    
    file.close();
    cout << numStudents << " students saved to file.\n";
}
