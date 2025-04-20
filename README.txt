========================================================
STUDENT INFORMATION SYSTEM
Version 1.0
Author: SulfurSkull
Date: April 20, 2025
========================================================

OVERVIEW
--------
The Student Information System is a comprehensive C++ application designed to help 
educational institutions manage student records, courses, grades, and study plans. 
The system provides a full suite of tools for tracking student information, academic 
performance, and educational planning.

This application comes in two versions:
1. A text-based console interface 
2. A more visual ncurses-based graphical user interface

FEATURES
--------
- Student Management:
  * Add, delete, modify, and search student records
  * Track student details including ID, name, and national ID

- Course Management:
  * Add, delete, and modify courses for each student
  * Record course grades and calculate GPA on a 4.0 scale

- Academic Planning:
  * Create and manage study plans for students
  * Track academic progress and performance

- Data Persistence:
  * Save and load data from files (students.txt)
  * Automatic data backup on exit

SYSTEM REQUIREMENTS
------------------
- C++ compiler (supports C++11 or higher)
- ncurses library (for GUI version)
- Linux/Unix terminal environment (for optimal GUI display)

HOW TO COMPILE
-------------
1. Console Version:
   $ g++ -o student_system main.cpp file_operations.cpp gpa_calculation.cpp sorting.cpp student_operations.cpp -std=c++11

2. GUI Version:
   $ g++ -o student_system_gui main_gui.cpp file_operations.cpp gpa_calculation.cpp sorting.cpp student_operations.cpp gui_interface.cpp -lncurses -std=c++11

HOW TO RUN
---------
1. Console Version:
   $ ./student_system

2. GUI Version:
   $ ./student_system_gui

USING THE APPLICATION
--------------------
Both versions offer the same functionality through a menu-driven interface:

1. Add Student - Add a new student to the system
2. Delete Student - Remove a student from the system
3. Modify Student - Change student information
4. Search Student - Find a student by ID or name
5. Display Students (By ID) - Show all students sorted by ID
6. Display Students (By Name) - Show all students sorted by name
7. Manage Courses - Add/delete/modify courses for a student
8. Compute GPA - Calculate a student's GPA on a 4.0 scale
9. Update Study Plan - Manage a student's academic plan

DATA STORAGE
-----------
Student data is stored in a text file called "students.txt" in the following format:
- Student ID
- Student Name
- National ID
- GPA
- Number of Courses
- Course Details (name and grade for each course)
- Number of Study Plan Items
- Study Plan Details (each line representing a study plan item)

TROUBLESHOOTING
--------------
1. If you encounter permission issues with the executable files, use:
   $ chmod +x student_system
   $ chmod +x student_system_gui

2. For the GUI version, ensure you have ncurses installed:
   $ sudo apt-get install libncurses5-dev libncursesw5-dev (Debian/Ubuntu)
   $ sudo yum install ncurses-devel (CentOS/RHEL)

3. If the GUI display appears corrupted, try resizing your terminal window.

CONTACT
-------
For questions, suggestions, or bug reports, please contact:
Email: contact@sulfurskull.com

========================================================
© 2025 SulfurSkull. All Rights Reserved.
========================================================