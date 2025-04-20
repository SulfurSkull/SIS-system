/**
 * Sorting Module - Functions for sorting student data
 */

#include "student.h"    // Include student structure definitions
#include <algorithm>    // Include for std::sort and other sorting algorithms

/**
 * Sort students by ID in ascending order
 * 
 * This function sorts the global array of students by their unique ID
 * using C++ standard library sort algorithm with a lambda comparator.
 */
void sortStudentsById() {
    // Sort using std::sort with lambda function as comparator
    // Algorithm complexity: O(n log n)
    sort(students, students + numStudents,     // Sort range: start to end of array
        [](const Student& a, const Student& b) {  // Lambda comparator function
            return a.id < b.id;                // Compare by ID (ascending order)
        }
    );
    // After this function completes, the students array will be sorted by ID
}

/**
 * Sort students by name in alphabetical order
 * 
 * This function sorts the global array of students by their names
 * using C++ standard library sort algorithm with a lambda comparator.
 * String comparison follows lexicographical ordering.
 */
void sortStudentsByName() {
    // Sort using std::sort with lambda function as comparator
    // Algorithm complexity: O(n log n)
    sort(students, students + numStudents,     // Sort range: start to end of array
        [](const Student& a, const Student& b) {  // Lambda comparator function
            return a.name < b.name;            // Compare by name (lexicographical order)
        }
    );
    // After this function completes, the students array will be sorted alphabetically by name
}