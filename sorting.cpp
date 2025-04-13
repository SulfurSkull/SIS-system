#include "student.h"
#include <algorithm>

void sortStudentsById() {
    sort(students, students + numStudents, [](const Student& a, const Student& b) {
        return a.id < b.id;
    });
}

void sortStudentsByName() {
    sort(students, students + numStudents, [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
}
