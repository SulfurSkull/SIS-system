/**
 * GUI Interface for Student Information System
 * 
 * This file contains the implementation of the ncurses-based
 * graphical user interface for the Student Information System.
 */

#include "student.h"   // Include student structure definitions
#include <ncurses.h>   // Include ncurses library for terminal UI
#include <menu.h>      // Include menu library for ncurses menus
#include <string>      // Include string library for string manipulation
#include <vector>      // Include vector for dynamic arrays
#include <sstream>     // Include string stream for string formatting
#include <iomanip>     // Include for input/output manipulators
#include <cstring>     // Include for C-style string functions
#include <ctime>       // Include for date and time functions
#include <algorithm>   // Include for sorting algorithms
#include <functional>  // Include for function objects

// Define global variables
Student students[MAX_STUDENTS];  // Array to store student data
int numStudents = 0;             // Counter for number of students

// Forward declarations of functions
void initializeGUI();            // Initialize the ncurses GUI
void displayMainMenu();          // Display the main menu
void processMainMenu();          // Process main menu selection
void displayStudentList(bool sortedById);  // Display list of students
void addStudentForm();           // Form to add a new student
void deleteStudentForm();        // Form to delete a student
void searchStudentForm();        // Form to search for a student
void modifyStudentForm();        // Form to modify student data
void manageCourseMenu();         // Menu for course management
void computeGPAForm();           // Form to compute GPA
void updateStudyPlanForm();      // Form to update study plan
void handleStudentListing(bool sortedById);  // Handle student listing
void cleanupGUI();               // Clean up GUI resources
void showMessage(const std::string& message);  // Show a message to the user
void displayFormField(int y, int x, const std::string& label, char* buffer, int bufSize, bool isNumeric = false);  // Display a form field
void centerText(int y, const std::string& text, int width);  // Center text on screen
int getNumericInput(int y, int x, int maxWidth);  // Get numeric input from user
void displayHeader();            // Display header with title and date
void drawBox(int startY, int startX, int height, int width);  // Draw a box around an area

// Global variables for ncurses
WINDOW* mainwin;
WINDOW* menuwin;
WINDOW* contentwin;
WINDOW* statuswin;
int termHeight, termWidth;
int contentHeight, contentWidth;
int menuHeight, menuWidth;
int statusHeight = 3;

// GUI colors
#define COLOR_HEADER 1
#define COLOR_MENU 2
#define COLOR_CONTENT 3
#define COLOR_STATUS 4
#define COLOR_HIGHLIGHT 5

// Initialize GUI
void initializeGUI() {
    // Initialize ncurses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  // Hide cursor
    
    // Get terminal dimensions
    getmaxyx(stdscr, termHeight, termWidth);
    
    // Define content area dimensions
    menuHeight = 12;
    menuWidth = 40;
    contentHeight = termHeight - statusHeight - 2;  // -2 for header
    contentWidth = termWidth;
    
    // Initialize color pairs
    init_pair(COLOR_HEADER, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_MENU, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_CONTENT, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_STATUS, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_HIGHLIGHT, COLOR_WHITE, COLOR_BLUE);
    
    // Create windows
    mainwin = newwin(termHeight, termWidth, 0, 0);
    menuwin = newwin(menuHeight, menuWidth, 3, (termWidth - menuWidth) / 2);
    contentwin = newwin(contentHeight, contentWidth, 2, 0);
    statuswin = newwin(statusHeight, termWidth, termHeight - statusHeight, 0);
    
    // Enable function keys and arrow keys in the menu window
    keypad(menuwin, TRUE);
    
    // Refresh the main window to show the changes
    refresh();
}

// Display header
void displayHeader() {
    // Fill header area with color
    attron(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    for (int row = 0; row < 2; row++) {
        move(row, 0);
        for (int i = 0; i < termWidth; i++) {
            addch(' ');
        }
    }
    
    // Display title
    std::string title = "STUDENT INFORMATION SYSTEM";
    int centerX = (termWidth - title.length()) / 2;
    mvprintw(0, centerX, "%s", title.c_str());
    
    // Display date in top right corner
    time_t now = time(0);
    struct tm* ltm = localtime(&now);
    char dateBuffer[12];
    sprintf(dateBuffer, "%02d/%02d/%04d", 
            1 + ltm->tm_mon, ltm->tm_mday, 1900 + ltm->tm_year);
    mvprintw(0, termWidth - 12, "%s", dateBuffer);
    
    attroff(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    refresh();
}

// Clean up GUI resources
void cleanupGUI() {
    delwin(menuwin);
    delwin(contentwin);
    delwin(statuswin);
    delwin(mainwin);
    endwin();
}

// Center text on a line
void centerText(int y, const std::string& text, int width) {
    int x = (width - text.length()) / 2;
    if (x < 0) x = 0;
    mvprintw(y, x, "%s", text.c_str());
}

// Display a message in the status window
void showMessage(const std::string& message) {
    werase(statuswin);
    wattron(statuswin, COLOR_PAIR(COLOR_STATUS));
    
    // Fill entire status window with color
    for (int y = 0; y < statusHeight; y++) {
        for (int x = 0; x < termWidth; x++) {
            mvwprintw(statuswin, y, x, " ");
        }
    }
    
    // Print message centered
    mvwprintw(statuswin, 1, (termWidth - message.length()) / 2, "%s", message.c_str());
    wattroff(statuswin, COLOR_PAIR(COLOR_STATUS));
    wrefresh(statuswin);
    
    // Wait for a key press
    wgetch(statuswin);
}

// Draw a box around an area
void drawBox(int startY, int startX, int height, int width) {
    mvhline(startY, startX, 0, width);                 // Top horizontal line
    mvhline(startY + height - 1, startX, 0, width);    // Bottom horizontal line
    mvvline(startY, startX, 0, height);                // Left vertical line
    mvvline(startY, startX + width - 1, 0, height);    // Right vertical line
    mvaddch(startY, startX, ACS_ULCORNER);             // Upper left corner
    mvaddch(startY, startX + width - 1, ACS_URCORNER); // Upper right corner
    mvaddch(startY + height - 1, startX, ACS_LLCORNER);          // Lower left corner
    mvaddch(startY + height - 1, startX + width - 1, ACS_LRCORNER); // Lower right corner
}

// Display form field and get input
void displayFormField(int y, int x, const std::string& label, char* buffer, int bufSize, bool isNumeric) {
    mvprintw(y, x, "%s:", label.c_str());
    echo();
    curs_set(1);  // Show cursor
    
    // Clear the input area
    for (int i = 0; i < 30; i++) {
        mvprintw(y, x + label.length() + 2 + i, " ");
    }
    
    // Get input
    if (isNumeric) {
        int value;
        scanw("%d", &value); // For numeric input (ID)
        sprintf(buffer, "%d", value);
    } else {
        getstr(buffer);  // For string input
    }
    
    noecho();
    curs_set(0);  // Hide cursor
}

// Get numeric input specifically
int getNumericInput(int y, int x, int maxWidth) {
    char buffer[20] = {0};
    echo();
    curs_set(1);  // Show cursor
    
    // Clear the input area
    for (int i = 0; i < maxWidth; i++) {
        mvprintw(y, x + i, " ");
    }
    
    // Move to position for input
    move(y, x);
    getstr(buffer);
    
    noecho();
    curs_set(0);  // Hide cursor
    
    // Convert to integer
    int value = 0;
    try {
        value = std::stoi(buffer);
    } catch (...) {
        value = 0;  // Default if conversion fails
    }
    
    return value;
}

// Display Main Menu
void displayMainMenu() {
    // Clear and draw the menu window with a border
    werase(menuwin);
    wattron(menuwin, COLOR_PAIR(COLOR_MENU));
    
    // Draw a fancy box border
    box(menuwin, 0, 0);
    
    // Add decorative corners
    mvwaddch(menuwin, 0, 0, ACS_ULCORNER);
    mvwaddch(menuwin, 0, menuWidth - 1, ACS_URCORNER);
    mvwaddch(menuwin, menuHeight - 1, 0, ACS_LLCORNER);
    mvwaddch(menuwin, menuHeight - 1, menuWidth - 1, ACS_LRCORNER);
    
    // Menu title with decoration
    wattron(menuwin, A_BOLD);
    mvwhline(menuwin, 2, 1, ACS_HLINE, menuWidth - 2); // Line under title
    
    std::string title = " MAIN MENU ";
    mvwprintw(menuwin, 1, (menuWidth - title.length()) / 2, "%s", title.c_str());
    wattroff(menuwin, A_BOLD);
    
    // Menu items with improved formatting
    const int startRow = 4;
    const int indent = 5;
    
    mvwprintw(menuwin, startRow + 0, indent, "1. Add Student");
    mvwprintw(menuwin, startRow + 1, indent, "2. Delete Student");
    mvwprintw(menuwin, startRow + 2, indent, "3. Modify Student");
    mvwprintw(menuwin, startRow + 3, indent, "4. Search Student");
    mvwprintw(menuwin, startRow + 4, indent, "5. Display Students (By ID)");
    mvwprintw(menuwin, startRow + 5, indent, "6. Display Students (By Name)");
    mvwprintw(menuwin, startRow + 6, indent, "7. Manage Courses");
    mvwprintw(menuwin, startRow + 7, indent, "8. Compute GPA");
    mvwprintw(menuwin, startRow + 8, indent, "9. Update Study Plan");
    mvwprintw(menuwin, startRow + 9, indent, "0. Exit");
    
    // Separator line above prompt
    mvwhline(menuwin, menuHeight - 3, 1, ACS_HLINE, menuWidth - 2);
    
    // Prompt with highlighted background
    wattron(menuwin, COLOR_PAIR(COLOR_HIGHLIGHT));
    mvwprintw(menuwin, menuHeight - 2, 3, " Enter your choice: ");
    wattroff(menuwin, COLOR_PAIR(COLOR_HIGHLIGHT));
    
    // Finish drawing
    wattroff(menuwin, COLOR_PAIR(COLOR_MENU));
    wrefresh(menuwin);
}

// Process Main Menu Selection
void processMainMenu() {
    int choice;
    echo();  // Enable echo for input
    curs_set(1);  // Show cursor
    
    // Get user input
    wmove(menuwin, menuHeight - 2, 21);  // Position cursor for input
    wscanw(menuwin, "%d", &choice);
    
    noecho();  // Disable echo
    curs_set(0);  // Hide cursor
    
    // Process the choice
    switch (choice) {
        case 1:
            addStudentForm();
            break;
        case 2:
            deleteStudentForm();
            break;
        case 3:
            modifyStudentForm();
            break;
        case 4:
            searchStudentForm();
            break;
        case 5:
            handleStudentListing(true);
            break;
        case 6:
            handleStudentListing(false);
            break;
        case 7:
            manageCourseMenu();
            break;
        case 8:
            computeGPAForm();
            break;
        case 9:
            updateStudyPlanForm();
            break;
        case 0:
            // Save and exit
            saveStudents();
            cleanupGUI();
            exit(0);
            break;
        default:
            showMessage("Invalid choice! Please try again.");
            break;
    }
}

// Main GUI execution function
void runGUI() {
    // Load existing student data
    loadStudents();
    
    // Initialize ncurses GUI
    initializeGUI();
    
    // Main GUI loop
    bool running = true;
    while (running) {
        // Display header and menu
        displayHeader();
        displayMainMenu();
        
        // Process user input
        processMainMenu();
    }
    
    // Clean up resources (should only reach here if there's an exit method 
    // that doesn't call exit() directly)
    cleanupGUI();
    
    // Save student data before exiting
    saveStudents();
}

// Handle Student Listing (Both by ID and Name)
void handleStudentListing(bool sortedById) {
    // Clear the content window
    werase(contentwin);
    
    // If no students, show message
    if (numStudents == 0) {
        showMessage("No students to display!");
        return;
    }
    
    // Display students in content window
    displayStudentList(sortedById);
    
    // Wait for any key to return to main menu
    mvprintw(termHeight - 4, (termWidth - 25) / 2, "Press any key to continue");
    refresh();
    getch();
}

// Display Student List
void displayStudentList(bool sortedById) {
    // Create a temporary array for sorting
    Student tempStudents[MAX_STUDENTS];
    copy(students, students + numStudents, tempStudents);
    
    // Sort the students
    if (sortedById) {
        sort(tempStudents, tempStudents + numStudents, 
            [](const Student& a, const Student& b) { return a.id < b.id; });
    } else {
        sort(tempStudents, tempStudents + numStudents, 
            [](const Student& a, const Student& b) { return a.name < b.name; });
    }
    
    // Clear content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    
    // Display table header
    std::string title = sortedById ? "Student List (Sorted by ID)" : "Student List (Sorted by Name)";
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - title.length()) / 2, "%s", title.c_str());
    wattroff(contentwin, A_BOLD);
    
    // Table header
    mvwprintw(contentwin, 3, 2, "ID");
    mvwprintw(contentwin, 3, 12, "Name");
    mvwprintw(contentwin, 3, 42, "National ID");
    mvwprintw(contentwin, 3, 62, "GPA");
    
    // Separator line
    mvwhline(contentwin, 4, 1, ACS_HLINE, contentWidth - 2);
    
    // Display student data
    for (int i = 0; i < numStudents && i < contentHeight - 8; i++) {
        mvwprintw(contentwin, i + 5, 2, "%d", tempStudents[i].id);
        mvwprintw(contentwin, i + 5, 12, "%s", tempStudents[i].name.c_str());
        mvwprintw(contentwin, i + 5, 42, "%s", tempStudents[i].nationalId.c_str());
        
        // Format GPA with 2 decimal places
        std::stringstream gpaStream;
        gpaStream << std::fixed << std::setprecision(2) << tempStudents[i].gpa;
        mvwprintw(contentwin, i + 5, 62, "%s / 4.00", gpaStream.str().c_str());
    }
    
    // Status line
    std::stringstream statusStream;
    statusStream << "Total students: " << numStudents;
    mvwprintw(contentwin, contentHeight - 2, 2, "%s", statusStream.str().c_str());
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
}

// Add Student Form
void addStudentForm() {
    if (numStudents >= MAX_STUDENTS) {
        showMessage("Maximum students reached!");
        return;
    }
    
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 16) / 2, "ADD NEW STUDENT");
    wattroff(contentwin, A_BOLD);
    
    // Student data buffer
    Student newStudent;
    char idBuffer[10] = {0};
    char nameBuffer[50] = {0};
    char nationalIdBuffer[15] = {0};
    
    // Input fields
    mvwprintw(contentwin, 3, 10, "Student ID:");
    mvwprintw(contentwin, 5, 10, "Name:");
    mvwprintw(contentwin, 7, 10, "National ID (14 digits):");
    
    // Get input for each field
    wmove(contentwin, 3, 25);
    echo();
    wgetstr(contentwin, idBuffer);
    
    wmove(contentwin, 5, 25);
    wgetstr(contentwin, nameBuffer);
    
    bool validNationalId = false;
    while (!validNationalId) {
        wmove(contentwin, 7, 40);
        wgetstr(contentwin, nationalIdBuffer);
        
        std::string nationalIdStr(nationalIdBuffer);
        validNationalId = validateNationalId(nationalIdStr);
        
        if (!validNationalId) {
            mvwprintw(contentwin, 9, 10, "Invalid national ID format. Must be 14 digits.");
            mvwprintw(contentwin, 7, 40, "                              "); // Clear input area
        }
    }
    
    noecho();
    
    // Convert and validate input
    try {
        newStudent.id = std::stoi(idBuffer);
        newStudent.name = nameBuffer;
        newStudent.nationalId = nationalIdBuffer;
        newStudent.numCourses = 0;
        newStudent.gpa = 0.0;
        newStudent.numStudyPlan = 0;
        
        // Check if ID already exists
        if (findStudentById(newStudent.id) != -1) {
            showMessage("ID already exists!");
            return;
        }
        
        // Add the student
        students[numStudents++] = newStudent;
        showMessage("Student added successfully!");
    } catch (const std::exception& e) {
        showMessage("Error: Invalid input format!");
    }
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
}

// Delete Student Form
void deleteStudentForm() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 18) / 2, "DELETE STUDENT");
    wattroff(contentwin, A_BOLD);
    
    // Input field
    mvwprintw(contentwin, 3, 10, "Enter student ID to delete:");
    
    // Get ID
    echo();
    int id;
    wmove(contentwin, 3, 40);
    wscanw(contentwin, "%d", &id);
    noecho();
    
    // Find student
    int index = findStudentById(id);
    if (index == -1) {
        showMessage("Student not found!");
        return;
    }
    
    // Show student info and confirm deletion
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 5, 10, "Student Information:");
    wattroff(contentwin, A_BOLD);
    
    mvwprintw(contentwin, 7, 10, "ID: %d", students[index].id);
    mvwprintw(contentwin, 8, 10, "Name: %s", students[index].name.c_str());
    mvwprintw(contentwin, 9, 10, "National ID: %s", students[index].nationalId.c_str());
    
    // Confirmation
    mvwprintw(contentwin, 11, 10, "Are you sure you want to delete this student? (y/n):");
    wrefresh(contentwin);
    
    // Get confirmation
    char confirm = wgetch(contentwin);
    
    if (confirm == 'y' || confirm == 'Y') {
        // Delete student by shifting remaining elements
        for (int i = index; i < numStudents - 1; i++) {
            students[i] = students[i + 1];
        }
        numStudents--;
        showMessage("Student deleted successfully!");
    } else {
        showMessage("Deletion cancelled.");
    }
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
}

// Search Student Form
void searchStudentForm() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 18) / 2, "SEARCH STUDENT");
    wattroff(contentwin, A_BOLD);
    
    // Search options
    mvwprintw(contentwin, 3, 10, "Search by:");
    mvwprintw(contentwin, 4, 15, "1. ID");
    mvwprintw(contentwin, 5, 15, "2. Name");
    mvwprintw(contentwin, 7, 10, "Enter choice (1 or 2):");
    
    // Get search type
    echo();
    int choice;
    wmove(contentwin, 7, 35);
    wscanw(contentwin, "%d", &choice);
    noecho();
    
    // Process based on choice
    if (choice == 1) {
        // Search by ID
        mvwprintw(contentwin, 9, 10, "Enter student ID:");
        echo();
        int id;
        wmove(contentwin, 9, 30);
        wscanw(contentwin, "%d", &id);
        noecho();
        
        int index = findStudentById(id);
        if (index != -1) {
            wattron(contentwin, A_BOLD);
            mvwprintw(contentwin, 11, 10, "Student found:");
            wattroff(contentwin, A_BOLD);
            
            mvwprintw(contentwin, 13, 15, "ID: %d", students[index].id);
            mvwprintw(contentwin, 14, 15, "Name: %s", students[index].name.c_str());
            mvwprintw(contentwin, 15, 15, "National ID: %s", students[index].nationalId.c_str());
            mvwprintw(contentwin, 16, 15, "Courses: %d", students[index].numCourses);
            
            // Format GPA
            std::stringstream gpaStream;
            gpaStream << std::fixed << std::setprecision(2) << students[index].gpa;
            mvwprintw(contentwin, 17, 15, "GPA: %s / 4.00", gpaStream.str().c_str());
        } else {
            mvwprintw(contentwin, 11, 10, "Student not found!");
        }
    } else if (choice == 2) {
        // Search by name
        mvwprintw(contentwin, 9, 10, "Enter student name (or part of name):");
        echo();
        char nameBuffer[50] = {0};
        wmove(contentwin, 9, 50);
        wgetstr(contentwin, nameBuffer);
        noecho();
        
        std::string searchName(nameBuffer);
        bool found = false;
        int row = 11;
        
        for (int i = 0; i < numStudents; i++) {
            // Case-insensitive search
            std::string studentNameLower = students[i].name;
            std::string searchNameLower = searchName;
            transform(studentNameLower.begin(), studentNameLower.end(), studentNameLower.begin(), ::tolower);
            transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);
            
            if (studentNameLower.find(searchNameLower) != std::string::npos) {
                if (!found) {
                    wattron(contentwin, A_BOLD);
                    mvwprintw(contentwin, row, 10, "Matching students:");
                    wattroff(contentwin, A_BOLD);
                    row += 2;
                }
                
                // Format GPA
                std::stringstream gpaStream;
                gpaStream << std::fixed << std::setprecision(2) << students[i].gpa;
                
                mvwprintw(contentwin, row, 10, "ID: %d, Name: %s, GPA: %s / 4.00", 
                          students[i].id, students[i].name.c_str(), gpaStream.str().c_str());
                row += 1;
                found = true;
                
                // Avoid listing too many students
                if (row > contentHeight - 5) break;
            }
        }
        
        if (!found) {
            mvwprintw(contentwin, 11, 10, "No matching students found!");
        }
    } else {
        mvwprintw(contentwin, 9, 10, "Invalid choice!");
    }
    
    // Wait for user
    mvwprintw(contentwin, contentHeight - 2, (contentWidth - 25) / 2, "Press any key to continue");
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
    wgetch(contentwin);
}

// Modify Student Form
void modifyStudentForm() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 18) / 2, "MODIFY STUDENT");
    wattroff(contentwin, A_BOLD);
    
    // Input field
    mvwprintw(contentwin, 3, 10, "Enter student ID to modify:");
    
    // Get ID
    echo();
    int id;
    wmove(contentwin, 3, 40);
    wscanw(contentwin, "%d", &id);
    noecho();
    
    // Find student
    int index = findStudentById(id);
    if (index == -1) {
        showMessage("Student not found!");
        return;
    }
    
    // Show current info and fields to modify
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 5, 10, "Current Information:");
    wattroff(contentwin, A_BOLD);
    
    mvwprintw(contentwin, 7, 10, "ID: %d", students[index].id);
    mvwprintw(contentwin, 8, 10, "Name: %s", students[index].name.c_str());
    mvwprintw(contentwin, 9, 10, "National ID: %s", students[index].nationalId.c_str());
    
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 11, 10, "Enter new information (leave blank to keep current):");
    wattroff(contentwin, A_BOLD);
    
    // New name
    echo();
    char nameBuffer[50] = {0};
    mvwprintw(contentwin, 13, 10, "New name:");
    wmove(contentwin, 13, 30);
    wgetstr(contentwin, nameBuffer);
    
    // New national ID
    char nationalIdBuffer[15] = {0};
    mvwprintw(contentwin, 14, 10, "New national ID (14 digits):");
    wmove(contentwin, 14, 45);
    wgetstr(contentwin, nationalIdBuffer);
    noecho();
    
    // Update student information if new values provided
    if (strlen(nameBuffer) > 0) {
        students[index].name = nameBuffer;
    }
    
    if (strlen(nationalIdBuffer) > 0) {
        if (validateNationalId(nationalIdBuffer)) {
            students[index].nationalId = nationalIdBuffer;
        } else {
            showMessage("Invalid national ID format! Must be 14 digits.");
            return;
        }
    }
    
    showMessage("Student information updated successfully!");
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
}

// Manage Course Menu
void manageCourseMenu() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 18) / 2, "MANAGE COURSES");
    wattroff(contentwin, A_BOLD);
    
    // Input field
    mvwprintw(contentwin, 3, 10, "Enter student ID:");
    
    // Get ID
    echo();
    int id;
    wmove(contentwin, 3, 30);
    wscanw(contentwin, "%d", &id);
    noecho();
    
    // Find student
    int index = findStudentById(id);
    if (index == -1) {
        showMessage("Student not found!");
        return;
    }
    
    // Course management loop
    bool exit = false;
    while (!exit) {
        // Clear the content window except the border and title
        for (int i = 2; i < contentHeight - 1; i++) {
            wmove(contentwin, i, 1);
            for (int j = 1; j < contentWidth - 1; j++) {
                waddch(contentwin, ' ');
            }
        }
        
        // Student name
        wattron(contentwin, A_BOLD);
        std::string courseTitle = "Course Management for " + students[index].name;
        mvwprintw(contentwin, 3, (contentWidth - courseTitle.length()) / 2, 
                 "%s", courseTitle.c_str());
        wattroff(contentwin, A_BOLD);
        
        // Course menu
        mvwprintw(contentwin, 5, 10, "1. Add Course");
        mvwprintw(contentwin, 6, 10, "2. Remove Course");
        mvwprintw(contentwin, 7, 10, "3. View Courses");
        mvwprintw(contentwin, 8, 10, "4. Back to Main Menu");
        
        mvwprintw(contentwin, 10, 10, "Enter choice (1-4):");
        wrefresh(contentwin);
        
        // Get course menu choice
        echo();
        int choice;
        wmove(contentwin, 10, 30);
        wscanw(contentwin, "%d", &choice);
        noecho();
        
        switch (choice) {
            case 1: {
                // Add Course
                if (students[index].numCourses >= MAX_COURSES) {
                    showMessage("Maximum courses reached!");
                    break;
                }
                
                // Clear input area
                for (int i = 12; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // Course input
                mvwprintw(contentwin, 12, 10, "Enter course name:");
                echo();
                char courseBuffer[50] = {0};
                wmove(contentwin, 12, 30);
                wgetstr(contentwin, courseBuffer);
                
                mvwprintw(contentwin, 13, 10, "Enter grade (0-100):");
                float grade;
                wmove(contentwin, 13, 30);
                wscanw(contentwin, "%f", &grade);
                noecho();
                
                // Validate grade
                if (grade < 0 || grade > 100) {
                    showMessage("Invalid grade! Must be between 0 and 100.");
                    break;
                }
                
                // Add course
                students[index].courses[students[index].numCourses].name = courseBuffer;
                students[index].courses[students[index].numCourses].grade = grade;
                students[index].numCourses++;
                
                // Recalculate GPA
                computeGPA(index);
                showMessage("Course added successfully!");
                break;
            }
            case 2: {
                // Remove Course
                if (students[index].numCourses == 0) {
                    showMessage("No courses to remove!");
                    break;
                }
                
                // Clear input area
                for (int i = 12; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // List courses
                wattron(contentwin, A_BOLD);
                mvwprintw(contentwin, 12, 10, "Current Courses:");
                wattroff(contentwin, A_BOLD);
                
                for (int i = 0; i < students[index].numCourses; i++) {
                    mvwprintw(contentwin, 14 + i, 10, "%d. %s (%.2f)", 
                             i + 1, 
                             students[index].courses[i].name.c_str(),
                             students[index].courses[i].grade);
                }
                
                // Get course to remove
                mvwprintw(contentwin, 14 + students[index].numCourses + 1, 10, 
                         "Enter course number to remove:");
                
                echo();
                int courseIndex;
                wmove(contentwin, 14 + students[index].numCourses + 1, 40);
                wscanw(contentwin, "%d", &courseIndex);
                noecho();
                
                // Validate and remove
                if (courseIndex < 1 || courseIndex > students[index].numCourses) {
                    showMessage("Invalid selection!");
                    break;
                }
                
                // Shift courses to remove
                for (int i = courseIndex - 1; i < students[index].numCourses - 1; i++) {
                    students[index].courses[i] = students[index].courses[i + 1];
                }
                students[index].numCourses--;
                
                // Recalculate GPA
                computeGPA(index);
                showMessage("Course removed successfully!");
                break;
            }
            case 3: {
                // View Courses
                if (students[index].numCourses == 0) {
                    showMessage("No courses registered for this student!");
                    break;
                }
                
                // Clear input area
                for (int i = 12; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // Display courses
                wattron(contentwin, A_BOLD);
                mvwprintw(contentwin, 12, 10, "Courses for %s:", students[index].name.c_str());
                mvwprintw(contentwin, 14, 10, "No.");
                mvwprintw(contentwin, 14, 20, "Course Name");
                mvwprintw(contentwin, 14, 40, "Grade");
                mvwprintw(contentwin, 14, 60, "GPA Scale");
                wattroff(contentwin, A_BOLD);
                
                // Horizontal line
                mvwhline(contentwin, 15, 10, ACS_HLINE, 70);
                
                // Course listing
                for (int i = 0; i < students[index].numCourses; i++) {
                    mvwprintw(contentwin, 16 + i, 10, "%d", i + 1);
                    mvwprintw(contentwin, 16 + i, 20, "%s", students[index].courses[i].name.c_str());
                    mvwprintw(contentwin, 16 + i, 40, "%.2f", students[index].courses[i].grade);
                    mvwprintw(contentwin, 16 + i, 60, "%.2f", convertGradeTo4Scale(students[index].courses[i].grade));
                }
                
                // Horizontal line
                mvwhline(contentwin, 16 + students[index].numCourses, 10, ACS_HLINE, 70);
                
                // Total GPA
                std::stringstream gpaStream;
                gpaStream << std::fixed << std::setprecision(2) << students[index].gpa;
                mvwprintw(contentwin, 17 + students[index].numCourses, 10, 
                         "Cumulative GPA: %s / 4.00", gpaStream.str().c_str());
                
                // Wait for user
                mvwprintw(contentwin, contentHeight - 2, (contentWidth - 25) / 2, 
                         "Press any key to continue");
                wrefresh(contentwin);
                wgetch(contentwin);
                break;
            }
            case 4:
                exit = true;
                break;
            default:
                showMessage("Invalid choice! Please try again.");
                break;
        }
        
        wrefresh(contentwin);
    }
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
}

// Compute GPA Form
void computeGPAForm() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 13) / 2, "COMPUTE GPA");
    wattroff(contentwin, A_BOLD);
    
    // Input field
    mvwprintw(contentwin, 3, 10, "Enter student ID:");
    
    // Get ID
    echo();
    int id;
    wmove(contentwin, 3, 30);
    wscanw(contentwin, "%d", &id);
    noecho();
    
    // Find student
    int index = findStudentById(id);
    if (index == -1) {
        showMessage("Student not found!");
        return;
    }
    
    // Calculate GPA and display
    if (students[index].numCourses == 0) {
        // No courses
        students[index].gpa = 0.0;
        showMessage("No courses available. GPA set to 0.0");
    } else {
        // Compute GPA
        computeGPA(index);
        
        // Show result
        std::stringstream message;
        message << "GPA updated: " << std::fixed << std::setprecision(2) 
                << students[index].gpa << " / 4.00";
        showMessage(message.str());
    }
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
    wrefresh(contentwin);
}

// Update Study Plan Form
void updateStudyPlanForm() {
    // Clear the content window
    werase(contentwin);
    wattron(contentwin, COLOR_PAIR(COLOR_CONTENT));
    box(contentwin, 0, 0);
    
    // Form title
    wattron(contentwin, A_BOLD);
    mvwprintw(contentwin, 1, (contentWidth - 18) / 2, "UPDATE STUDY PLAN");
    wattroff(contentwin, A_BOLD);
    
    // Input field
    mvwprintw(contentwin, 3, 10, "Enter student ID:");
    
    // Get ID
    echo();
    int id;
    wmove(contentwin, 3, 30);
    wscanw(contentwin, "%d", &id);
    noecho();
    
    // Find student
    int index = findStudentById(id);
    if (index == -1) {
        showMessage("Student not found!");
        return;
    }
    
    // Study plan management loop
    bool exit = false;
    while (!exit) {
        // Clear the content window except the border and title
        for (int i = 4; i < contentHeight - 1; i++) {
            wmove(contentwin, i, 1);
            for (int j = 1; j < contentWidth - 1; j++) {
                waddch(contentwin, ' ');
            }
        }
        
        // Student name
        wattron(contentwin, A_BOLD);
        std::string planTitle = "Study Plan Management for " + students[index].name;
        mvwprintw(contentwin, 4, (contentWidth - planTitle.length()) / 2, 
                 "%s", planTitle.c_str());
        wattroff(contentwin, A_BOLD);
        
        // Study plan menu
        mvwprintw(contentwin, 6, 10, "1. Add Course to Plan");
        mvwprintw(contentwin, 7, 10, "2. Remove Course from Plan");
        mvwprintw(contentwin, 8, 10, "3. View Plan");
        mvwprintw(contentwin, 9, 10, "4. Back to Main Menu");
        
        mvwprintw(contentwin, 11, 10, "Enter choice (1-4):");
        wrefresh(contentwin);
        
        // Get study plan menu choice
        echo();
        int choice;
        wmove(contentwin, 11, 30);
        wscanw(contentwin, "%d", &choice);
        noecho();
        
        switch (choice) {
            case 1: {
                // Add Course to Plan
                if (students[index].numStudyPlan >= MAX_STUDY_PLAN) {
                    showMessage("Study plan is full!");
                    break;
                }
                
                // Clear input area
                for (int i = 13; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // Course input
                mvwprintw(contentwin, 13, 10, "Enter course name for study plan:");
                echo();
                char courseBuffer[50] = {0};
                wmove(contentwin, 13, 45);
                wgetstr(contentwin, courseBuffer);
                noecho();
                
                // Add course to plan
                students[index].studyPlan[students[index].numStudyPlan++] = courseBuffer;
                showMessage("Course added to study plan!");
                break;
            }
            case 2: {
                // Remove Course from Plan
                if (students[index].numStudyPlan == 0) {
                    showMessage("Study plan is empty!");
                    break;
                }
                
                // Clear input area
                for (int i = 13; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // List courses in plan
                wattron(contentwin, A_BOLD);
                mvwprintw(contentwin, 13, 10, "Current Study Plan:");
                wattroff(contentwin, A_BOLD);
                
                for (int i = 0; i < students[index].numStudyPlan; i++) {
                    mvwprintw(contentwin, 15 + i, 10, "%d. %s", 
                             i + 1, 
                             students[index].studyPlan[i].c_str());
                }
                
                // Get course to remove
                mvwprintw(contentwin, 15 + students[index].numStudyPlan + 1, 10, 
                         "Enter item number to remove:");
                
                echo();
                int planIndex;
                wmove(contentwin, 15 + students[index].numStudyPlan + 1, 40);
                wscanw(contentwin, "%d", &planIndex);
                noecho();
                
                // Validate and remove
                if (planIndex < 1 || planIndex > students[index].numStudyPlan) {
                    showMessage("Invalid selection!");
                    break;
                }
                
                // Shift courses to remove
                for (int i = planIndex - 1; i < students[index].numStudyPlan - 1; i++) {
                    students[index].studyPlan[i] = students[index].studyPlan[i + 1];
                }
                students[index].numStudyPlan--;
                
                showMessage("Course removed from study plan!");
                break;
            }
            case 3: {
                // View Study Plan
                if (students[index].numStudyPlan == 0) {
                    showMessage("Study plan is empty for this student!");
                    break;
                }
                
                // Clear input area
                for (int i = 13; i < contentHeight - 2; i++) {
                    wmove(contentwin, i, 1);
                    for (int j = 1; j < contentWidth - 1; j++) {
                        waddch(contentwin, ' ');
                    }
                }
                
                // Display study plan
                wattron(contentwin, A_BOLD);
                mvwprintw(contentwin, 13, 10, "Study Plan for %s:", students[index].name.c_str());
                wattroff(contentwin, A_BOLD);
                
                // Horizontal line
                mvwhline(contentwin, 14, 10, ACS_HLINE, 50);
                
                // Course listing
                for (int i = 0; i < students[index].numStudyPlan; i++) {
                    mvwprintw(contentwin, 15 + i, 10, "%d. %s", 
                             i + 1, students[index].studyPlan[i].c_str());
                }
                
                // Horizontal line
                mvwhline(contentwin, 15 + students[index].numStudyPlan, 10, ACS_HLINE, 50);
                
                // Total courses
                mvwprintw(contentwin, 16 + students[index].numStudyPlan, 10, 
                         "Total courses in plan: %d", students[index].numStudyPlan);
                
                // Wait for user
                mvwprintw(contentwin, contentHeight - 2, (contentWidth - 25) / 2, 
                         "Press any key to continue");
                wrefresh(contentwin);
                wgetch(contentwin);
                break;
            }
            case 4:
                exit = true;
                break;
            default:
                showMessage("Invalid choice! Please try again.");
                break;
        }
        
        wrefresh(contentwin);
    }
    
    wattroff(contentwin, COLOR_PAIR(COLOR_CONTENT));
}

// This function is already defined above (lines 307-331)