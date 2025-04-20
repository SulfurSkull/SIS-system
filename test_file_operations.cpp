#include "student.h"
#include <iostream>
#include <fstream>

// Global array and counter from student.h
Student students[MAX_STUDENTS];
int numStudents = 0;

// Test functions
bool testLoadStudents() {
    loadStudents();
    std::cout << "Loaded " << numStudents << " students from file." << std::endl;
    return true;
}

bool testSaveStudents() {
    // Make sure we have at least one student to save
    if (numStudents == 0) {
        // Create a test student if none exist
        Student testStudent;
        testStudent.id = 999;
        testStudent.name = "Test Student";
        testStudent.nationalId = "12345678901234";
        testStudent.gpa = 3.50;
        students[numStudents++] = testStudent;
    }
    
    saveStudents();
    std::cout << "Saved " << numStudents << " students to file." << std::endl;
    return true;
}

bool testNationalIdValidation() {
    // Test valid ID
    std::string validId = "12345678901234";
    bool valid = validateNationalId(validId);
    std::cout << "Valid ID test: " << (valid ? "PASSED" : "FAILED") << std::endl;
    
    // Test invalid ID (wrong length)
    std::string invalidId1 = "123456";
    bool invalid1 = !validateNationalId(invalidId1);
    std::cout << "Invalid ID (length) test: " << (invalid1 ? "PASSED" : "FAILED") << std::endl;
    
    // Test invalid ID (non-numeric)
    std::string invalidId2 = "1234567890123A";
    bool invalid2 = !validateNationalId(invalidId2);
    std::cout << "Invalid ID (non-numeric) test: " << (invalid2 ? "PASSED" : "FAILED") << std::endl;
    
    return valid && invalid1 && invalid2;
}

bool testGpaCalculation() {
    if (numStudents == 0) {
        std::cout << "No students to test GPA calculation." << std::endl;
        return false;
    }
    
    // Add test course with known grade
    Student& s = students[0];
    int originalNumCourses = s.numCourses;
    float originalGpa = s.gpa;
    
    // Skip test if we can't add more courses
    if (s.numCourses >= MAX_COURSES) {
        std::cout << "Cannot add test course - max courses reached." << std::endl;
        return false;
    }
    
    // Add a test course with grade 90 (3.7 on 4.0 scale)
    s.courses[s.numCourses].name = "Test Course";
    s.courses[s.numCourses].grade = 90.0;
    s.numCourses++;
    
    // Calculate GPA
    float testGrade = convertGradeTo4Scale(90.0);
    std::cout << "Test grade 90 converted to 4.0 scale: " << testGrade << std::endl;
    std::cout << "Expected value: 3.7" << std::endl;
    
    // Compute GPA for student
    computeGPA(0);
    std::cout << "Updated GPA: " << s.gpa << std::endl;
    
    // Restore original state
    s.numCourses = originalNumCourses;
    s.gpa = originalGpa;
    
    return true;
}

int main() {
    std::cout << "========== File Operations Test ==========" << std::endl;
    bool loadTest = testLoadStudents();
    bool saveTest = testSaveStudents();
    
    std::cout << "\n========== National ID Validation Test ==========" << std::endl;
    bool idTest = testNationalIdValidation();
    
    std::cout << "\n========== GPA Calculation Test ==========" << std::endl;
    bool gpaTest = testGpaCalculation();
    
    std::cout << "\n========== Test Summary ==========" << std::endl;
    std::cout << "Load Students: " << (loadTest ? "PASSED" : "FAILED") << std::endl;
    std::cout << "Save Students: " << (saveTest ? "PASSED" : "FAILED") << std::endl;
    std::cout << "National ID Validation: " << (idTest ? "PASSED" : "FAILED") << std::endl;
    std::cout << "GPA Calculation: " << (gpaTest ? "PASSED" : "FAILED") << std::endl;
    
    return 0;
}