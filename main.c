#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definitions
typedef struct {
    char id[10];
    char name[50];
    char password[20];
} User;

typedef struct {
    char taskId[10];
    char studentId[10];
    char taskTitle[50];
    char description[100];
    char assignedBy[50];
} Task;

typedef struct {
    char taskId[10];
    char studentId[10];
    char grade[3];
    char teacherName[50];
} Grade;

typedef struct {
    char subjectCode[10];
    char subjectName[30];
    int isActive;
} Subject;

typedef struct {
    char teacherId[10];
    char teacherName[50];
    char subjectCode[10];
    char subjectName[30];
} TeacherSubject;

typedef struct {
    char studentId[10];
    char studentName[50];
    char subjectCode[10];
    char subjectName[30];
} StudentSubject;

// Function prototypes
void main_menu();
int login(char role[]);
void admin_panel();
void teacher_panel(char teacherName[]);
void student_panel(char studentId[]);

// Admin functions
void add_student();
void add_teacher();
void delete_student();
void delete_teacher();
void view_all_users();
void reset_password();

// Teacher functions
void view_students();
void assign_task(char teacherName[]);
void grade_student(char teacherName[]);
void view_assignments(char teacherName[]);
void view_all_grades_report(char teacherName[]);
void search_student();
void update_task(char teacherName[]);
void delete_assignment(char teacherName[]);
void generate_grade_summary(char teacherName[]);

// Student functions
void view_profile(char studentId[]);
void view_my_tasks(char studentId[]);
void view_my_grades(char studentId[]);

// File operations
User* load_users(char filename[], int *count);
void save_users(char filename[], User users[], int count);
Task* load_tasks(int *count);
void save_tasks(Task tasks[], int count);
Grade* load_grades(int *count);
void save_grades(Grade grades[], int count);

// Subject Management Functions
void initializeSubjects();
void manageSubjects();
void addSubject();
void editSubject();
void deleteSubject();
void viewAllSubjects();
void assignSubjectToTeacher();
void assignSubjectToStudent();
void viewTeacherAssignments();
void viewStudentEnrollments();
void removeTeacherAssignment();
void removeStudentEnrollment();

// Subject File Operations
Subject* loadSubjects(int *count);
void saveSubjects(Subject subjects[], int count);
TeacherSubject* loadTeacherSubjects(int *count);
void saveTeacherSubjects(TeacherSubject assignments[], int count);
StudentSubject* loadStudentSubjects(int *count);
void saveStudentSubjects(StudentSubject enrollments[], int count);

// Utility functions
void clear_screen();
void pause_screen();
void generate_task_id(char taskId[]);

int main() {
    printf("============================================\n");
    printf("              Grade Book                 \n");
    printf("============================================\n\n");
    printf("Welcome to Grade Book System.\n\n");
    
    main_menu();
    return 0;
}

void main_menu() {
    int choice;
    char role[20];
    
    while(1) {
        clear_screen();
        printf("=========================================\n");
        printf("            Login Portal              \n");
        printf("=========================================\n\n");
        printf("Choose your role:\n\n");
        printf("  1. Admin Login\n");
        printf("  2. Teacher Login\n");
        printf("  3. Student Login\n");
        printf("  0. Exit\n\n");
        printf("Enter choice (0-3): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                strcpy(role, "admin");
                printf("\nPreparing admin access...\n");
                if(login(role)) {
                    admin_panel();
                }
                break;
            case 2:
                strcpy(role, "teacher");
                printf("\nSetting up teacher dashboard...\n");
                if(login(role)) {
                    char teacherName[50];
                    printf("\nEnter your full name: ");
                    scanf(" %[^\n]", teacherName);
                    printf("Welcome, %s!\n", teacherName);
                    pause_screen();
                    teacher_panel(teacherName);
                }
                break;
            case 3:
                strcpy(role, "student");
                printf("\nLoading student portal...\n");
                if(login(role)) {
                    char studentId[10];
                    printf("\nEnter your Student ID: ");
                    scanf("%s", studentId);
                    printf("Access granted!\n");
                    pause_screen();
                    student_panel(studentId);
                }
                break;
            case 0:
                printf("\nThank you for using Grade Book System!\n");
                printf("Goodbye!\n");
                pause_screen();
                exit(0);
            default:
                printf("\nInvalid option. Choose 0-3.\n");
                pause_screen();
        }
    }
}

int login(char role[]) {
    char filename[30];
    char username[50], password[20];
    
    sprintf(filename, "users_%s.dat", role);
    
    printf("\n=========================================\n");
    printf("         %s Login                    \n", role);
    printf("=========================================\n\n");
    printf("Please enter your credentials:\n\n");
    printf("Username: ");
    scanf(" %[^\n]", username);
    printf("Password: ");
    scanf("%s", password);
    
    printf("\nVerifying credentials...\n");
    
    // Check credentials
    int count;
    User *users = load_users(filename, &count);
    
    if(users == NULL) {
        // Create default admin if no users exist and trying admin login
        if(strcmp(role, "admin") == 0) {
            User admin = {"admin", "Administrator", "admin123"};
            save_users(filename, &admin, 1);
            if(strcmp(username, "Administrator") == 0 && strcmp(password, "admin123") == 0) {
                printf("\nWelcome! Default admin account.\n");
                printf("Tip: Change password for security.\n");
                free(users);
                pause_screen();
                return 1;
            }
        }
        printf("\nNo user accounts found or invalid credentials.\n");
        printf("Check username and password, or contact admin.\n");
        pause_screen();
        return 0;
    }
    
    for(int i = 0; i < count; i++) {
        if(strcmp(users[i].name, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nLogin successful! Welcome, %s!\n", username);
            free(users);
            pause_screen();
            return 1;
        }
    }
    
    printf("\nInvalid credentials.\n");
    printf("Check username and password and try again.\n");
    free(users);
    pause_screen();
    return 0;
}

void admin_panel() {
    int choice;
    
    while(1) {
        clear_screen();
        printf("=========================================\n");
        printf("        Admin Dashboard                \n");
        printf("      System Management Center       \n");
        printf("=========================================\n\n");
        printf("Management Options:\n\n");
        printf("User Management:\n");
        printf("  1. Add New Student\n");
        printf("  2. Add New Teacher\n");
        printf("  3. Remove Student\n");
        printf("  4. Remove Teacher\n");
        printf("  5. View All Users\n");
        printf("  6. Reset Password\n\n");
        printf("Subject Management:\n");
        printf("  7. Manage Subjects\n");
        printf("  8. Assign Subject to Teacher\n");
        printf("  9. Enroll Student in Subject\n");
        printf(" 10. View Teacher Assignments\n");
        printf(" 11. View Student Enrollments\n\n");
        printf(" 12. Logout\n\n");
        printf("Enter choice (1-12): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: 
                printf("\nAdding new student...\n");
                pause_screen();
                add_student(); 
                break;
            case 2: 
                printf("\nAdding new teacher...\n");
                pause_screen();
                add_teacher(); 
                break;
            case 3: 
                printf("\nRemoving student...\n");
                pause_screen();
                delete_student(); 
                break;
            case 4: 
                printf("\nRemoving teacher...\n");
                pause_screen();
                delete_teacher(); 
                break;
            case 5: 
                printf("\nLoading user list...\n");
                pause_screen();
                view_all_users(); 
                break;
            case 6: 
                printf("\nPassword reset...\n");
                pause_screen();
                reset_password(); 
                break;
            case 7:
                printf("\nManaging subjects...\n");
                pause_screen();
                manageSubjects();
                break;
            case 8:
                printf("\nAssigning subject to teacher...\n");
                pause_screen();
                assignSubjectToTeacher();
                break;
            case 9:
                printf("\nEnrolling student in subject...\n");
                pause_screen();
                assignSubjectToStudent();
                break;
            case 10:
                printf("\nViewing teacher assignments...\n");
                pause_screen();
                viewTeacherAssignments();
                break;
            case 11:
                printf("\nViewing student enrollments...\n");
                pause_screen();
                viewStudentEnrollments();
                break;
            case 12: 
                printf("\nLogging out...\n");
                pause_screen();
                return;
            default:
                printf("\nInvalid option. Choose 1-12.\n");
                pause_screen();
        }
    }
}

void teacher_panel(char teacherName[]) {
    int choice;
    
    while(1) {
        clear_screen();
        printf("=========================================\n");
        printf("        Teacher Dashboard - %s        \n", teacherName);
        printf("      Academic Management Portal     \n");
        printf("=========================================\n\n");
        printf("Teacher Functions:\n\n");
        printf("  1. View Students\n");
        printf("  2. Assign Task\n");
        printf("  3. Grade Student\n");
        printf("  4. View My Assignments\n");
        printf("  5. View All Grades Report\n");
        printf("  6. Search Student by ID\n");
        printf("  7. Update Task Description\n");
        printf("  8. Delete Assignment\n");
        printf("  9. Generate Grade Summary\n");
        printf(" 10. Logout\n\n");
        printf("Enter choice (1-10): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: 
                printf("\nLoading student list...\n");
                pause_screen();
                view_students(); 
                break;
            case 2: 
                printf("\nCreating new assignment...\n");
                pause_screen();
                assign_task(teacherName); 
                break;
            case 3: 
                printf("\nGrading student work...\n");
                pause_screen();
                grade_student(teacherName); 
                break;
            case 4: 
                printf("\nLoading your assignments...\n");
                pause_screen();
                view_assignments(teacherName); 
                break;
            case 5:
                printf("\nGenerating grades report...\n");
                pause_screen();
                view_all_grades_report(teacherName);
                break;
            case 6:
                printf("\nSearching student...\n");
                pause_screen();
                search_student();
                break;
            case 7:
                printf("\nUpdating task...\n");
                pause_screen();
                update_task(teacherName);
                break;
            case 8:
                printf("\nDeleting assignment...\n");
                pause_screen();
                delete_assignment(teacherName);
                break;
            case 9:
                printf("\nGenerating grade summary...\n");
                pause_screen();
                generate_grade_summary(teacherName);
                break;
            case 10: 
                printf("\nLogging out. Thank you, %s!\n", teacherName);
                pause_screen();
                return;
            default:
                printf("\nInvalid choice. Choose 1-10.\n");
                pause_screen();
        }
    }
}

void student_panel(char studentId[]) {
    int choice;
    
    while(1) {
        clear_screen();
        printf("=========================================\n");
        printf("        Student Portal - ID: %s        \n", studentId);
        printf("      Your Personal Learning Space    \n");
        printf("=========================================\n\n");
        printf("Student Functions:\n\n");
        printf("  1. View My Profile\n");
        printf("  2. View My Tasks\n");
        printf("  3. View My Grades\n");
        printf("  4. Logout\n\n");
        printf("Enter choice (1-4): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: 
                printf("\nLoading profile...\n");
                pause_screen();
                view_profile(studentId); 
                break;
            case 2: 
                printf("\nFetching assignments...\n");
                pause_screen();
                view_my_tasks(studentId); 
                break;
            case 3: 
                printf("\nRetrieving grades...\n");
                pause_screen();
                view_my_grades(studentId); 
                break;
            case 4: 
                printf("\nLogging out...\n");
                pause_screen();
                return;
            default:
                printf("\nInvalid option. Choose 1-4.\n");
                pause_screen();
        }
    }
}

// Admin Functions Implementation
void add_student() {
    User newStudent;
    int count;
    
    printf("\n===== ADD STUDENT =====\n");
    printf("Student ID: ");
    scanf("%s", newStudent.id);
    printf("Student Name: ");
    scanf(" %[^\n]", newStudent.name);
    printf("Password: ");
    scanf("%s", newStudent.password);
    
    User *students = load_users("users_student.dat", &count);
    
    // Check if ID already exists (only if students exist)
    if(students != NULL) {
        for(int i = 0; i < count; i++) {
            if(strcmp(students[i].id, newStudent.id) == 0) {
                printf("Student ID already exists!\n");
                free(students);
                pause_screen();
                return;
            }
        }
    }
    
    // Add new student
    if(students == NULL) {
        // No existing students, create new array
        students = malloc(sizeof(User));
        if(students == NULL) {
            printf("Memory allocation failed!\n");
            pause_screen();
            return;
        }
    } else {
        // Resize existing array
        User *temp = realloc(students, (count + 1) * sizeof(User));
        if(temp == NULL) {
            printf("Memory allocation failed!\n");
            free(students);
            pause_screen();
            return;
        }
        students = temp;
    }
    
    students[count] = newStudent;
    save_users("users_student.dat", students, count + 1);
    
    printf("Student added successfully!\n");
    free(students);
    pause_screen();
}

void add_teacher() {
    User newTeacher;
    int count;
    
    printf("\n===== ADD TEACHER =====\n");
    printf("Teacher ID: ");
    scanf("%s", newTeacher.id);
    printf("Teacher Name: ");
    scanf(" %[^\n]", newTeacher.name);
    printf("Password: ");
    scanf("%s", newTeacher.password);
    
    User *teachers = load_users("users_teacher.dat", &count);
    
    // Check if ID already exists (only if teachers exist)
    if(teachers != NULL) {
        for(int i = 0; i < count; i++) {
            if(strcmp(teachers[i].id, newTeacher.id) == 0) {
                printf("Teacher ID already exists!\n");
                free(teachers);
                pause_screen();
                return;
            }
        }
    }
    
    // Add new teacher
    if(teachers == NULL) {
        // No existing teachers, create new array
        teachers = malloc(sizeof(User));
        if(teachers == NULL) {
            printf("Memory allocation failed!\n");
            pause_screen();
            return;
        }
    } else {
        // Resize existing array
        User *temp = realloc(teachers, (count + 1) * sizeof(User));
        if(temp == NULL) {
            printf("Memory allocation failed!\n");
            free(teachers);
            pause_screen();
            return;
        }
        teachers = temp;
    }
    
    teachers[count] = newTeacher;
    save_users("users_teacher.dat", teachers, count + 1);
    
    printf("Teacher added successfully!\n");
    free(teachers);
    pause_screen();
}

void delete_student() {
    char targetId[10];
    int count;
    
    printf("\n===== DELETE STUDENT =====\n");
    printf("Enter Student ID to delete: ");
    scanf("%s", targetId);
    
    User *students = load_users("users_student.dat", &count);
    if(students == NULL) {
        printf("No students found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].id, targetId) == 0) {
            // Shift remaining elements
            for(int j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }
            count--;
            found = 1;
            break;
        }
    }
    
    if(found) {
        save_users("users_student.dat", students, count);
        printf("Student deleted successfully!\n");
    } else {
        printf("Student ID not found!\n");
    }
    
    free(students);
    pause_screen();
}

void delete_teacher() {
    char targetId[10];
    int count;
    
    printf("\n===== DELETE TEACHER =====\n");
    printf("Enter Teacher ID to delete: ");
    scanf("%s", targetId);
    
    User *teachers = load_users("users_teacher.dat", &count);
    if(teachers == NULL) {
        printf("No teachers found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(teachers[i].id, targetId) == 0) {
            // Shift remaining elements
            for(int j = i; j < count - 1; j++) {
                teachers[j] = teachers[j + 1];
            }
            count--;
            found = 1;
            break;
        }
    }
    
    if(found) {
        save_users("users_teacher.dat", teachers, count);
        printf("Teacher deleted successfully!\n");
    } else {
        printf("Teacher ID not found!\n");
    }
    
    free(teachers);
    pause_screen();
}

void view_all_users() {
    int count;
    
    printf("\n===== ALL USERS =====\n");
    
    // Show students
    printf("\nSTUDENTS:\n");
    printf("%-10s %-20s\n", "ID", "Name");
    printf("------------------------------\n");
    
    User *students = load_users("users_student.dat", &count);
    if(students != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-10s %-20s\n", students[i].id, students[i].name);
        }
        free(students);
    } else {
        printf("No students found.\n");
    }
    
    // Show teachers
    printf("\nTEACHERS:\n");
    printf("%-10s %-20s\n", "ID", "Name");
    printf("------------------------------\n");
    
    User *teachers = load_users("users_teacher.dat", &count);
    if(teachers != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-10s %-20s\n", teachers[i].id, teachers[i].name);
        }
        free(teachers);
    } else {
        printf("No teachers found.\n");
    }
    
    pause_screen();
}

void reset_password() {
    char role[10], userId[10], newPassword[20];
    int count;
    
    printf("\n===== RESET PASSWORD =====\n");
    printf("User type (student/teacher): ");
    scanf("%s", role);
    printf("User ID: ");
    scanf("%s", userId);
    printf("New Password: ");
    scanf("%s", newPassword);
    
    char filename[30];
    sprintf(filename, "users_%s.dat", role);
    
    User *users = load_users(filename, &count);
    if(users == NULL) {
        printf("No users found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(users[i].id, userId) == 0) {
            strcpy(users[i].password, newPassword);
            found = 1;
            break;
        }
    }
    
    if(found) {
        save_users(filename, users, count);
        printf("Password reset successfully!\n");
    } else {
        printf("User ID not found!\n");
    }
    
    free(users);
    pause_screen();
}

// Teacher Functions Implementation
void view_students() {
    int count;
    
    printf("\n===== STUDENT LIST =====\n");
    printf("%-10s %-20s\n", "ID", "Name");
    printf("------------------------------\n");
    
    User *students = load_users("users_student.dat", &count);
    if(students != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-10s %-20s\n", students[i].id, students[i].name);
        }
        free(students);
    } else {
        printf("No students found.\n");
    }
    
    pause_screen();
}

void assign_task(char teacherName[]) {
    Task newTask;
    int count;
    
    printf("\n===== ASSIGN TASK =====\n");
    
    // Generate task ID
    generate_task_id(newTask.taskId);
    
    printf("Task ID: %s\n", newTask.taskId);
    printf("Student ID: ");
    scanf("%s", newTask.studentId);
    printf("Task Title: ");
    scanf(" %[^\n]", newTask.taskTitle);
    printf("Description: ");
    scanf(" %[^\n]", newTask.description);
    strcpy(newTask.assignedBy, teacherName);
    
    // Load existing tasks
    Task *tasks = load_tasks(&count);
    
    // Add new task
    tasks = realloc(tasks, (count + 1) * sizeof(Task));
    tasks[count] = newTask;
    save_tasks(tasks, count + 1);
    
    printf("Task assigned successfully!\n");
    if(tasks) free(tasks);
    pause_screen();
}

void grade_student(char teacherName[]) {
    Grade newGrade;
    int taskCount;
    
    printf("\n===== GRADE STUDENT =====\n");
    
    // Show available tasks
    Task *tasks = load_tasks(&taskCount);
    if(tasks == NULL || taskCount == 0) {
        printf("No tasks available to grade!\n");
        pause_screen();
        return;
    }
    
    printf("Available Tasks:\n");
    printf("%-10s %-10s %-20s\n", "Task ID", "Student", "Title");
    printf("------------------------------------------\n");
    for(int i = 0; i < taskCount; i++) {
        printf("%-10s %-10s %-20s\n", tasks[i].taskId, tasks[i].studentId, tasks[i].taskTitle);
    }
    
    printf("\nTask ID to grade: ");
    scanf("%s", newGrade.taskId);
    printf("Student ID: ");
    scanf("%s", newGrade.studentId);
    printf("Grade (A/B/C/D/F): ");
    scanf("%s", newGrade.grade);
    strcpy(newGrade.teacherName, teacherName);
    
    // Load existing grades
    int gradeCount;
    Grade *grades = load_grades(&gradeCount);
    
    // Check if grade already exists for this task and student
    int found = 0;
    for(int i = 0; i < gradeCount; i++) {
        if(strcmp(grades[i].taskId, newGrade.taskId) == 0 && 
           strcmp(grades[i].studentId, newGrade.studentId) == 0) {
            strcpy(grades[i].grade, newGrade.grade);
            strcpy(grades[i].teacherName, newGrade.teacherName);
            found = 1;
            break;
        }
    }
    
    if(!found) {
        // Add new grade
        grades = realloc(grades, (gradeCount + 1) * sizeof(Grade));
        grades[gradeCount] = newGrade;
        gradeCount++;
    }
    
    save_grades(grades, gradeCount);
    printf("Grade assigned successfully!\n");
    
    if(tasks) free(tasks);
    if(grades) free(grades);
    pause_screen();
}

void view_assignments(char teacherName[]) {
    int count;
    
    printf("\n===== MY ASSIGNMENTS =====\n");
    printf("%-10s %-10s %-20s\n", "Task ID", "Student", "Title");
    printf("------------------------------------------\n");
    
    Task *tasks = load_tasks(&count);
    if(tasks != NULL) {
        for(int i = 0; i < count; i++) {
            if(strcmp(tasks[i].assignedBy, teacherName) == 0) {
                printf("%-10s %-10s %-20s\n", tasks[i].taskId, tasks[i].studentId, tasks[i].taskTitle);
            }
        }
        free(tasks);
    } else {
        printf("No assignments found.\n");
    }
    
    pause_screen();
}

void view_all_grades_report(char teacherName[]) {
    int gradeCount, taskCount;
    
    printf("\n===== ALL GRADES REPORT =====\n");
    printf("%-10s %-10s %-8s %-15s %-20s\n", "Task ID", "Student", "Grade", "Teacher", "Task Title");
    printf("-----------------------------------------------------------------------\n");
    
    Grade *grades = load_grades(&gradeCount);
    Task *tasks = load_tasks(&taskCount);
    
    if(grades != NULL && tasks != NULL) {
        int found = 0;
        for(int i = 0; i < gradeCount; i++) {
            if(strcmp(grades[i].teacherName, teacherName) == 0) {
                // Find task title
                char taskTitle[50] = "Unknown";
                for(int j = 0; j < taskCount; j++) {
                    if(strcmp(tasks[j].taskId, grades[i].taskId) == 0) {
                        strcpy(taskTitle, tasks[j].taskTitle);
                        break;
                    }
                }
                printf("%-10s %-10s %-8s %-15s %-20s\n", 
                       grades[i].taskId, grades[i].studentId, grades[i].grade, 
                       grades[i].teacherName, taskTitle);
                found = 1;
            }
        }
        if(!found) {
            printf("No grades found for your assignments.\n");
        }
    } else {
        printf("No data available.\n");
    }
    
    if(grades) free(grades);
    if(tasks) free(tasks);
    pause_screen();
}

void search_student() {
    char searchId[10];
    int count;
    
    printf("\n===== SEARCH STUDENT =====\n");
    printf("Enter Student ID to search: ");
    scanf("%s", searchId);
    
    User *students = load_users("users_student.dat", &count);
    if(students != NULL) {
        int found = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(students[i].id, searchId) == 0) {
                printf("\nStudent Found:\n");
                printf("ID: %s\n", students[i].id);
                printf("Name: %s\n", students[i].name);
                
                // Show student's tasks
                printf("\nAssigned Tasks:\n");
                printf("%-10s %-20s %-15s\n", "Task ID", "Title", "Assigned By");
                printf("-----------------------------------------------\n");
                
                int taskCount;
                Task *tasks = load_tasks(&taskCount);
                if(tasks != NULL) {
                    int taskFound = 0;
                    for(int j = 0; j < taskCount; j++) {
                        if(strcmp(tasks[j].studentId, searchId) == 0) {
                            printf("%-10s %-20s %-15s\n", tasks[j].taskId, tasks[j].taskTitle, tasks[j].assignedBy);
                            taskFound = 1;
                        }
                    }
                    if(!taskFound) {
                        printf("No tasks assigned.\n");
                    }
                    free(tasks);
                }
                
                // Show student's grades
                printf("\nGrades:\n");
                printf("%-10s %-8s %-15s\n", "Task ID", "Grade", "Teacher");
                printf("-----------------------------------\n");
                
                int gradeCount;
                Grade *grades = load_grades(&gradeCount);
                if(grades != NULL) {
                    int gradeFound = 0;
                    for(int j = 0; j < gradeCount; j++) {
                        if(strcmp(grades[j].studentId, searchId) == 0) {
                            printf("%-10s %-8s %-15s\n", grades[j].taskId, grades[j].grade, grades[j].teacherName);
                            gradeFound = 1;
                        }
                    }
                    if(!gradeFound) {
                        printf("No grades available.\n");
                    }
                    free(grades);
                }
                
                found = 1;
                break;
            }
        }
        if(!found) {
            printf("Student ID not found!\n");
        }
        free(students);
    } else {
        printf("No students found in system!\n");
    }
    
    pause_screen();
}

void update_task(char teacherName[]) {
    char taskId[10];
    int count;
    
    printf("\n===== UPDATE TASK =====\n");
    
    // Show teacher's assignments first
    printf("Your Assignments:\n");
    printf("%-10s %-10s %-20s\n", "Task ID", "Student", "Title");
    printf("------------------------------------------\n");
    
    Task *tasks = load_tasks(&count);
    if(tasks != NULL) {
        int found = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(tasks[i].assignedBy, teacherName) == 0) {
                printf("%-10s %-10s %-20s\n", tasks[i].taskId, tasks[i].studentId, tasks[i].taskTitle);
                found = 1;
            }
        }
        if(!found) {
            printf("No assignments found.\n");
            free(tasks);
            pause_screen();
            return;
        }
    } else {
        printf("No assignments found.\n");
        pause_screen();
        return;
    }
    
    printf("\nEnter Task ID to update: ");
    scanf("%s", taskId);
    
    int taskFound = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(tasks[i].taskId, taskId) == 0 && strcmp(tasks[i].assignedBy, teacherName) == 0) {
            printf("\nCurrent Task Details:\n");
            printf("Title: %s\n", tasks[i].taskTitle);
            printf("Description: %s\n", tasks[i].description);
            
            printf("\nEnter new title (or press Enter to keep current): ");
            char newTitle[50];
            scanf(" %[^\n]", newTitle);
            if(strlen(newTitle) > 0) {
                strcpy(tasks[i].taskTitle, newTitle);
            }
            
            printf("Enter new description (or press Enter to keep current): ");
            char newDesc[100];
            scanf(" %[^\n]", newDesc);
            if(strlen(newDesc) > 0) {
                strcpy(tasks[i].description, newDesc);
            }
            
            save_tasks(tasks, count);
            printf("Task updated successfully!\n");
            taskFound = 1;
            break;
        }
    }
    
    if(!taskFound) {
        printf("Task ID not found or you don't have permission to update it!\n");
    }
    
    free(tasks);
    pause_screen();
}

void delete_assignment(char teacherName[]) {
    char taskId[10];
    int count;
    
    printf("\n===== DELETE ASSIGNMENT =====\n");
    
    // Show teacher's assignments first
    printf("Your Assignments:\n");
    printf("%-10s %-10s %-20s\n", "Task ID", "Student", "Title");
    printf("------------------------------------------\n");
    
    Task *tasks = load_tasks(&count);
    if(tasks != NULL) {
        int found = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(tasks[i].assignedBy, teacherName) == 0) {
                printf("%-10s %-10s %-20s\n", tasks[i].taskId, tasks[i].studentId, tasks[i].taskTitle);
                found = 1;
            }
        }
        if(!found) {
            printf("No assignments found.\n");
            free(tasks);
            pause_screen();
            return;
        }
    } else {
        printf("No assignments found.\n");
        pause_screen();
        return;
    }
    
    printf("\nEnter Task ID to delete: ");
    scanf("%s", taskId);
    
    int taskFound = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(tasks[i].taskId, taskId) == 0 && strcmp(tasks[i].assignedBy, teacherName) == 0) {
            printf("Are you sure you want to delete '%s'? (y/n): ", tasks[i].taskTitle);
            char confirm;
            scanf(" %c", &confirm);
            
            if(confirm == 'y' || confirm == 'Y') {
                // Shift remaining elements
                for(int j = i; j < count - 1; j++) {
                    tasks[j] = tasks[j + 1];
                }
                count--;
                save_tasks(tasks, count);
                
                // Also delete associated grades
                int gradeCount;
                Grade *grades = load_grades(&gradeCount);
                if(grades != NULL) {
                    int newGradeCount = gradeCount;
                    for(int k = 0; k < newGradeCount; k++) {
                        if(strcmp(grades[k].taskId, taskId) == 0) {
                            for(int l = k; l < newGradeCount - 1; l++) {
                                grades[l] = grades[l + 1];
                            }
                            newGradeCount--;
                            k--; // Re-check current position
                        }
                    }
                    if(newGradeCount != gradeCount) {
                        save_grades(grades, newGradeCount);
                    }
                    free(grades);
                }
                
                printf("Assignment deleted successfully!\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            taskFound = 1;
            break;
        }
    }
    
    if(!taskFound) {
        printf("Task ID not found or you don't have permission to delete it!\n");
    }
    
    free(tasks);
    pause_screen();
}

void generate_grade_summary(char teacherName[]) {
    int gradeCount, taskCount, studentCount;
    
    printf("\n===== GRADE SUMMARY REPORT =====\n");
    
    Grade *grades = load_grades(&gradeCount);
    Task *tasks = load_tasks(&taskCount);
    User *students = load_users("users_student.dat", &studentCount);
    
    if(grades == NULL || tasks == NULL) {
        printf("No data available for summary.\n");
        if(grades) free(grades);
        if(tasks) free(tasks);
        if(students) free(students);
        pause_screen();
        return;
    }
    
    // Count grades by letter for this teacher
    int gradeStats[5] = {0}; // A, B, C, D, F
    int totalGrades = 0;
    
    printf("Grade Distribution for %s:\n", teacherName);
    printf("========================================\n");
    
    for(int i = 0; i < gradeCount; i++) {
        if(strcmp(grades[i].teacherName, teacherName) == 0) {
            totalGrades++;
            switch(grades[i].grade[0]) {
                case 'A': gradeStats[0]++; break;
                case 'B': gradeStats[1]++; break;
                case 'C': gradeStats[2]++; break;
                case 'D': gradeStats[3]++; break;
                case 'F': gradeStats[4]++; break;
            }
        }
    }
    
    if(totalGrades > 0) {
        printf("A: %d (%.1f%%)\n", gradeStats[0], (gradeStats[0] * 100.0) / totalGrades);
        printf("B: %d (%.1f%%)\n", gradeStats[1], (gradeStats[1] * 100.0) / totalGrades);
        printf("C: %d (%.1f%%)\n", gradeStats[2], (gradeStats[2] * 100.0) / totalGrades);
        printf("D: %d (%.1f%%)\n", gradeStats[3], (gradeStats[3] * 100.0) / totalGrades);
        printf("F: %d (%.1f%%)\n", gradeStats[4], (gradeStats[4] * 100.0) / totalGrades);
        printf("\nTotal Grades Given: %d\n", totalGrades);
        
        // Count assignments and students
        int assignmentCount = 0;
        for(int i = 0; i < taskCount; i++) {
            if(strcmp(tasks[i].assignedBy, teacherName) == 0) {
                assignmentCount++;
            }
        }
        printf("Total Assignments Created: %d\n", assignmentCount);
        
    } else {
        printf("No grades found for your assignments.\n");
    }
      if(grades) free(grades);
    if(tasks) free(tasks);
    if(students) free(students);
    pause_screen();
}

// Subject Management Functions Implementation

void initializeSubjects() {
    Subject defaultSubjects[] = {
        {"NEP", "Nepali", 1},
        {"ENG", "English", 1},
        {"MATH", "Mathematics", 1},
        {"SCI", "Science", 1},
        {"SOC", "Social Studies", 1},
        {"COMP", "Computer", 1},
        {"OMATH", "Optional Mathematics", 1}
    };
    
    int count;
    Subject *subjects = loadSubjects(&count);
    
    if(subjects == NULL || count == 0) {
        printf("Initializing default subjects...\n");
        saveSubjects(defaultSubjects, 7);
        printf("Default subjects initialized successfully!\n");
    } else {
        printf("Subjects already exist.\n");
        free(subjects);
    }
}

void manageSubjects() {
    int choice;
    
    // Initialize subjects if not exists
    initializeSubjects();
    
    while(1) {
        clear_screen();
        printf("=========================================\n");
        printf("        Subject Management             \n");
        printf("=========================================\n\n");
        printf("Subject Options:\n\n");
        printf("  1. View All Subjects\n");
        printf("  2. Add New Subject\n");
        printf("  3. Edit Subject\n");
        printf("  4. Delete Subject\n");
        printf("  5. Back to Admin Panel\n\n");
        printf("Enter choice (1-5): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\nLoading subjects...\n");
                pause_screen();
                viewAllSubjects();
                break;
            case 2:
                printf("\nAdding new subject...\n");
                pause_screen();
                addSubject();
                break;
            case 3:
                printf("\nEditing subject...\n");
                pause_screen();
                editSubject();
                break;
            case 4:
                printf("\nDeleting subject...\n");
                pause_screen();
                deleteSubject();
                break;
            case 5:
                return;
            default:
                printf("\nInvalid option. Choose 1-5.\n");
                pause_screen();
        }
    }
}

void viewAllSubjects() {
    int count;
    
    printf("\n===== ALL SUBJECTS =====\n");
    printf("%-8s %-25s %-8s\n", "Code", "Subject Name", "Status");
    printf("------------------------------------------\n");
    
    Subject *subjects = loadSubjects(&count);
    if(subjects != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-8s %-25s %-8s\n", 
                   subjects[i].subjectCode, 
                   subjects[i].subjectName,
                   subjects[i].isActive ? "Active" : "Inactive");
        }
        free(subjects);
    } else {
        printf("No subjects found.\n");
    }
    
    pause_screen();
}

void addSubject() {
    Subject newSubject;
    int count;
    
    printf("\n===== ADD SUBJECT =====\n");
    printf("Subject Code (e.g., PHYS): ");
    scanf("%s", newSubject.subjectCode);
    printf("Subject Name: ");
    scanf(" %[^\n]", newSubject.subjectName);
    newSubject.isActive = 1;
    
    Subject *subjects = loadSubjects(&count);
    
    // Check if code already exists
    for(int i = 0; i < count; i++) {
        if(strcmp(subjects[i].subjectCode, newSubject.subjectCode) == 0) {
            printf("Subject code already exists!\n");
            if(subjects) free(subjects);
            pause_screen();
            return;
        }
    }
    
    // Add new subject
    subjects = realloc(subjects, (count + 1) * sizeof(Subject));
    subjects[count] = newSubject;
    saveSubjects(subjects, count + 1);
    
    printf("Subject added successfully!\n");
    free(subjects);
    pause_screen();
}

void editSubject() {
    char subjectCode[10];
    int count;
    
    printf("\n===== EDIT SUBJECT =====\n");
    
    // Show existing subjects
    viewAllSubjects();
    
    printf("\nEnter Subject Code to edit: ");
    scanf("%s", subjectCode);
    
    Subject *subjects = loadSubjects(&count);
    if(subjects == NULL) {
        printf("No subjects found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(subjects[i].subjectCode, subjectCode) == 0) {
            printf("\nCurrent Subject Details:\n");
            printf("Code: %s\n", subjects[i].subjectCode);
            printf("Name: %s\n", subjects[i].subjectName);
            printf("Status: %s\n", subjects[i].isActive ? "Active" : "Inactive");
            
            printf("\nEnter new name (or press Enter to keep current): ");
            char newName[30];
            scanf(" %[^\n]", newName);
            if(strlen(newName) > 0) {
                strcpy(subjects[i].subjectName, newName);
            }
            
            printf("Change status? (1=Active, 0=Inactive, -1=No change): ");
            int newStatus;
            scanf("%d", &newStatus);
            if(newStatus == 0 || newStatus == 1) {
                subjects[i].isActive = newStatus;
            }
            
            saveSubjects(subjects, count);
            printf("Subject updated successfully!\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("Subject code not found!\n");
    }
    
    free(subjects);
    pause_screen();
}

void deleteSubject() {
    char subjectCode[10];
    int count;
    
    printf("\n===== DELETE SUBJECT =====\n");
    
    // Show existing subjects
    viewAllSubjects();
    
    printf("\nEnter Subject Code to delete: ");
    scanf("%s", subjectCode);
    
    Subject *subjects = loadSubjects(&count);
    if(subjects == NULL) {
        printf("No subjects found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {        if(strcmp(subjects[i].subjectCode, subjectCode) == 0) {
            printf("Are you sure you want to delete '%s - %s'? (y/n): ", 
                   subjects[i].subjectCode, subjects[i].subjectName);
            char confirm;
            scanf(" %c", &confirm);
            
            if(confirm == 'y' || confirm == 'Y') {
                // Shift remaining elements
                for(int j = i; j < count - 1; j++) {
                    subjects[j] = subjects[j + 1];
                }
                count--;
                saveSubjects(subjects, count);
                printf("Subject deleted successfully!\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("Subject code not found!\n");
    }
    
    free(subjects);
    pause_screen();
}

void assignSubjectToTeacher() {
    char teacherId[10], teacherName[50], subjectCode[10];
    int teacherCount, subjectCount;
    
    printf("\n===== ASSIGN SUBJECT TO TEACHER =====\n");
    
    // Show available teachers
    printf("Available Teachers:\n");
    printf("%-10s %-20s\n", "ID", "Name");
    printf("------------------------------\n");
    
    User *teachers = load_users("users_teacher.dat", &teacherCount);
    if(teachers == NULL || teacherCount == 0) {
        printf("No teachers found!\n");
        pause_screen();
        return;
    }
    
    for(int i = 0; i < teacherCount; i++) {
        printf("%-10s %-20s\n", teachers[i].id, teachers[i].name);
    }
    
    printf("\nEnter Teacher ID: ");
    scanf("%s", teacherId);
    
    // Verify teacher exists and get name
    int teacherFound = 0;
    for(int i = 0; i < teacherCount; i++) {
        if(strcmp(teachers[i].id, teacherId) == 0) {
            strcpy(teacherName, teachers[i].name);
            teacherFound = 1;
            break;
        }
    }
    
    if(!teacherFound) {
        printf("Teacher ID not found!\n");
        free(teachers);
        pause_screen();
        return;
    }
    
    // Show available subjects
    printf("\nAvailable Subjects:\n");
    printf("%-8s %-25s\n", "Code", "Subject Name");
    printf("------------------------------\n");
    
    Subject *subjects = loadSubjects(&subjectCount);
    if(subjects == NULL || subjectCount == 0) {
        printf("No subjects found!\n");
        free(teachers);
        pause_screen();
        return;
    }
    
    for(int i = 0; i < subjectCount; i++) {
        if(subjects[i].isActive) {
            printf("%-8s %-25s\n", subjects[i].subjectCode, subjects[i].subjectName);
        }
    }
    
    printf("\nEnter Subject Code: ");
    scanf("%s", subjectCode);
    
    // Verify subject exists
    int subjectFound = 0;
    char subjectName[30];
    for(int i = 0; i < subjectCount; i++) {
        if(strcmp(subjects[i].subjectCode, subjectCode) == 0 && subjects[i].isActive) {
            strcpy(subjectName, subjects[i].subjectName);
            subjectFound = 1;
            break;
        }
    }
    
    if(!subjectFound) {
        printf("Subject code not found or inactive!\n");
        free(teachers);
        free(subjects);
        pause_screen();
        return;
    }
    
    // Check if assignment already exists
    int assignmentCount;
    TeacherSubject *assignments = loadTeacherSubjects(&assignmentCount);
    
    for(int i = 0; i < assignmentCount; i++) {
        if(strcmp(assignments[i].teacherId, teacherId) == 0 && 
           strcmp(assignments[i].subjectCode, subjectCode) == 0) {
            printf("Teacher is already assigned to this subject!\n");
            free(teachers);
            free(subjects);
            if(assignments) free(assignments);
            pause_screen();
            return;
        }
    }
    
    // Create new assignment
    TeacherSubject newAssignment;
    strcpy(newAssignment.teacherId, teacherId);
    strcpy(newAssignment.teacherName, teacherName);
    strcpy(newAssignment.subjectCode, subjectCode);
    strcpy(newAssignment.subjectName, subjectName);
    
    assignments = realloc(assignments, (assignmentCount + 1) * sizeof(TeacherSubject));
    assignments[assignmentCount] = newAssignment;
    saveTeacherSubjects(assignments, assignmentCount + 1);
    
    printf("Subject assigned to teacher successfully!\n");
    
    free(teachers);
    free(subjects);
    free(assignments);
    pause_screen();
}

void assignSubjectToStudent() {
    char studentId[10], studentName[50], subjectCode[10];
    int studentCount, subjectCount;
    
    printf("\n===== ENROLL STUDENT IN SUBJECT =====\n");
    
    // Show available students
    printf("Available Students:\n");
    printf("%-10s %-20s\n", "ID", "Name");
    printf("------------------------------\n");
    
    User *students = load_users("users_student.dat", &studentCount);
    if(students == NULL || studentCount == 0) {
        printf("No students found!\n");
        pause_screen();
        return;
    }
    
    for(int i = 0; i < studentCount; i++) {
        printf("%-10s %-20s\n", students[i].id, students[i].name);
    }
    
    printf("\nEnter Student ID: ");
    scanf("%s", studentId);
    
    // Verify student exists and get name
    int studentFound = 0;
    for(int i = 0; i < studentCount; i++) {
        if(strcmp(students[i].id, studentId) == 0) {
            strcpy(studentName, students[i].name);
            studentFound = 1;
            break;
        }
    }
    
    if(!studentFound) {
        printf("Student ID not found!\n");
        free(students);
        pause_screen();
        return;
    }
    
    // Show available subjects
    printf("\nAvailable Subjects:\n");
    printf("%-8s %-25s\n", "Code", "Subject Name");
    printf("------------------------------\n");
    
    Subject *subjects = loadSubjects(&subjectCount);
    if(subjects == NULL || subjectCount == 0) {
        printf("No subjects found!\n");
        free(students);
        pause_screen();
        return;
    }
    
    for(int i = 0; i < subjectCount; i++) {
        if(subjects[i].isActive) {
            printf("%-8s %-25s\n", subjects[i].subjectCode, subjects[i].subjectName);
        }
    }
    
    printf("\nEnter Subject Code: ");
    scanf("%s", subjectCode);
    
    // Verify subject exists
    int subjectFound = 0;
    char subjectName[30];
    for(int i = 0; i < subjectCount; i++) {
        if(strcmp(subjects[i].subjectCode, subjectCode) == 0 && subjects[i].isActive) {
            strcpy(subjectName, subjects[i].subjectName);
            subjectFound = 1;
            break;
        }
    }
    
    if(!subjectFound) {
        printf("Subject code not found or inactive!\n");
        free(students);
        free(subjects);
        pause_screen();
        return;
    }
      // Check if enrollment already exists
    int enrollmentCount;
    StudentSubject *enrollments = loadStudentSubjects(&enrollmentCount);
    
    for(int i = 0; i < enrollmentCount; i++) {
        if(strcmp(enrollments[i].studentId, studentId) == 0 && 
           strcmp(enrollments[i].subjectCode, subjectCode) == 0) {
            printf("Student is already enrolled in this subject!\n");
            free(students);
            free(subjects);
            if(enrollments) free(enrollments);
            pause_screen();
            return;
        }
    }
    
    // Create new enrollment
    StudentSubject newEnrollment;
    strcpy(newEnrollment.studentId, studentId);
    strcpy(newEnrollment.studentName, studentName);
    strcpy(newEnrollment.subjectCode, subjectCode);
    strcpy(newEnrollment.subjectName, subjectName);
    
    enrollments = realloc(enrollments, (enrollmentCount + 1) * sizeof(StudentSubject));
    enrollments[enrollmentCount] = newEnrollment;
    saveStudentSubjects(enrollments, enrollmentCount + 1);
    
    printf("Student enrolled in subject successfully!\n");
    
    free(students);
    free(subjects);
    free(enrollments);
    pause_screen();
}

void viewTeacherAssignments() {
    int count;
    
    printf("\n===== TEACHER SUBJECT ASSIGNMENTS =====\n");
    printf("%-10s %-20s %-8s %-25s\n", "Teacher ID", "Teacher Name", "Subject", "Subject Name");
    printf("----------------------------------------------------------------------\n");
    
    TeacherSubject *assignments = loadTeacherSubjects(&count);
    if(assignments != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-10s %-20s %-8s %-25s\n", 
                   assignments[i].teacherId, 
                   assignments[i].teacherName,
                   assignments[i].subjectCode,
                   assignments[i].subjectName);
        }
        free(assignments);
    } else {
        printf("No teacher assignments found.\n");
    }
    
    pause_screen();
}

void viewStudentEnrollments() {
    int count;
    
    printf("\n===== STUDENT SUBJECT ENROLLMENTS =====\n");
    printf("%-10s %-20s %-8s %-25s\n", "Student ID", "Student Name", "Subject", "Subject Name");
    printf("----------------------------------------------------------------------\n");
    
    StudentSubject *enrollments = loadStudentSubjects(&count);
    if(enrollments != NULL) {
        for(int i = 0; i < count; i++) {
            printf("%-10s %-20s %-8s %-25s\n", 
                   enrollments[i].studentId, 
                   enrollments[i].studentName,
                   enrollments[i].subjectCode,
                   enrollments[i].subjectName);
        }
        free(enrollments);
    } else {
        printf("No student enrollments found.\n");
    }
    
    pause_screen();
}

void removeTeacherAssignment() {
    char teacherId[10], subjectCode[10];
    int count;
    
    printf("\n===== REMOVE TEACHER ASSIGNMENT =====\n");
    
    // Show current assignments
    viewTeacherAssignments();
    
    printf("\nEnter Teacher ID: ");
    scanf("%s", teacherId);
    printf("Enter Subject Code: ");
    scanf("%s", subjectCode);
    
    TeacherSubject *assignments = loadTeacherSubjects(&count);
    if(assignments == NULL) {
        printf("No assignments found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(assignments[i].teacherId, teacherId) == 0 && 
           strcmp(assignments[i].subjectCode, subjectCode) == 0) {
            // Shift remaining elements
            for(int j = i; j < count - 1; j++) {
                assignments[j] = assignments[j + 1];
            }
            count--;
            saveTeacherSubjects(assignments, count);
            printf("Teacher assignment removed successfully!\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("Assignment not found!\n");
    }
    
    free(assignments);
    pause_screen();
}

void removeStudentEnrollment() {
    char studentId[10], subjectCode[10];
    int count;
    
    printf("\n===== REMOVE STUDENT ENROLLMENT =====\n");
    
    // Show current enrollments
    viewStudentEnrollments();
    
    printf("\nEnter Student ID: ");
    scanf("%s", studentId);
    printf("Enter Subject Code: ");
    scanf("%s", subjectCode);
    
    StudentSubject *enrollments = loadStudentSubjects(&count);
    if(enrollments == NULL) {
        printf("No enrollments found!\n");
        pause_screen();
        return;
    }
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(enrollments[i].studentId, studentId) == 0 && 
           strcmp(enrollments[i].subjectCode, subjectCode) == 0) {
            // Shift remaining elements
            for(int j = i; j < count - 1; j++) {
                enrollments[j] = enrollments[j + 1];
            }
            count--;
            saveStudentSubjects(enrollments, count);
            printf("Student enrollment removed successfully!\n");
            found = 1;
            break;
        }
    }    if(!found) {
        printf("Enrollment not found!\n");
    }
    
    free(enrollments);
    pause_screen();
}

// Subject File Operations Implementation
Subject* loadSubjects(int *count) {
    FILE *file = fopen("subjects.dat", "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    Subject *subjects = malloc(*count * sizeof(Subject));
    fread(subjects, sizeof(Subject), *count, file);
    fclose(file);
    return subjects;
}

void saveSubjects(Subject subjects[], int count) {
    FILE *file = fopen("subjects.dat", "wb");
    if(file == NULL) {
        printf("Error saving subjects!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(subjects, sizeof(Subject), count, file);
    fclose(file);
}

TeacherSubject* loadTeacherSubjects(int *count) {
    FILE *file = fopen("teacher_subjects.dat", "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    TeacherSubject *assignments = malloc(*count * sizeof(TeacherSubject));
    fread(assignments, sizeof(TeacherSubject), *count, file);
    fclose(file);
    return assignments;
}

void saveTeacherSubjects(TeacherSubject assignments[], int count) {
    FILE *file = fopen("teacher_subjects.dat", "wb");
    if(file == NULL) {
        printf("Error saving teacher assignments!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(assignments, sizeof(TeacherSubject), count, file);
    fclose(file);
}

StudentSubject* loadStudentSubjects(int *count) {
    FILE *file = fopen("student_subjects.dat", "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    StudentSubject *enrollments = malloc(*count * sizeof(StudentSubject));
    fread(enrollments, sizeof(StudentSubject), *count, file);
    fclose(file);
    return enrollments;
}

void saveStudentSubjects(StudentSubject enrollments[], int count) {
    FILE *file = fopen("student_subjects.dat", "wb");
    if(file == NULL) {
        printf("Error saving student enrollments!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(enrollments, sizeof(StudentSubject), count, file);
    fclose(file);
}

// File Operations Implementation
User* load_users(char filename[], int *count) {
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    User *users = malloc(*count * sizeof(User));
    fread(users, sizeof(User), *count, file);
    fclose(file);
    return users;
}

void save_users(char filename[], User users[], int count) {
    FILE *file = fopen(filename, "wb");
    if(file == NULL) {
        printf("Error saving users!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(users, sizeof(User), count, file);
    fclose(file);
}

Task* load_tasks(int *count) {
    FILE *file = fopen("tasks.dat", "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    Task *tasks = malloc(*count * sizeof(Task));
    fread(tasks, sizeof(Task), *count, file);
    fclose(file);
    return tasks;
}

void save_tasks(Task tasks[], int count) {
    FILE *file = fopen("tasks.dat", "wb");
    if(file == NULL) {
        printf("Error saving tasks!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(tasks, sizeof(Task), count, file);
    fclose(file);
}

Grade* load_grades(int *count) {
    FILE *file = fopen("grades.dat", "rb");
    if(file == NULL) {
        *count = 0;
        return NULL;
    }
    
    fread(count, sizeof(int), 1, file);
    if(*count == 0) {
        fclose(file);
        return NULL;
    }
    
    Grade *grades = malloc(*count * sizeof(Grade));
    fread(grades, sizeof(Grade), *count, file);
    fclose(file);
    return grades;
}

void save_grades(Grade grades[], int count) {
    FILE *file = fopen("grades.dat", "wb");
    if(file == NULL) {
        printf("Error saving grades!\n");
        return;
    }
      fwrite(&count, sizeof(int), 1, file);
    fwrite(grades, sizeof(Grade), count, file);
    fclose(file);
}

// Student Functions Implementation
void view_profile(char studentId[]) {
    int count;
    
    printf("\n===== MY PROFILE =====\n");
    
    User *students = load_users("users_student.dat", &count);
    if(students != NULL) {
        for(int i = 0; i < count; i++) {
            if(strcmp(students[i].id, studentId) == 0) {
                printf("Student ID: %s\n", students[i].id);
                printf("Name: %s\n", students[i].name);
                
                // Show enrolled subjects
                printf("\nEnrolled Subjects:\n");
                printf("%-8s %-25s\n", "Code", "Subject Name");
                printf("------------------------------\n");
                
                int enrollmentCount;
                StudentSubject *enrollments = loadStudentSubjects(&enrollmentCount);
                if(enrollments != NULL) {
                    int subjectFound = 0;
                    for(int j = 0; j < enrollmentCount; j++) {
                        if(strcmp(enrollments[j].studentId, studentId) == 0) {
                            printf("%-8s %-25s\n", enrollments[j].subjectCode, enrollments[j].subjectName);
                            subjectFound = 1;
                        }
                    }
                    if(!subjectFound) {
                        printf("No subjects enrolled.\n");
                    }
                    free(enrollments);
                }
                
                break;
            }
        }
        free(students);
    }
    
    pause_screen();
}

void view_my_tasks(char studentId[]) {
    int count;
    
    printf("\n===== MY TASKS =====\n");
    printf("%-10s %-20s %-20s\n", "Task ID", "Title", "Assigned By");
    printf("-----------------------------------------------\n");
    
    Task *tasks = load_tasks(&count);
    if(tasks != NULL) {
        int found = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(tasks[i].studentId, studentId) == 0) {
                printf("%-10s %-20s %-20s\n", tasks[i].taskId, tasks[i].taskTitle, tasks[i].assignedBy);
                printf("Description: %s\n\n", tasks[i].description);
                found = 1;
            }
        }
        if(!found) {
            printf("No tasks assigned.\n");
        }
        free(tasks);
    } else {
        printf("No tasks found.\n");
    }
    
    pause_screen();
}

void view_my_grades(char studentId[]) {
    int gradeCount, taskCount;
    
    printf("\n===== MY GRADES =====\n");
    printf("%-10s %-20s %-8s %-15s\n", "Task ID", "Task Title", "Grade", "Teacher");
    printf("--------------------------------------------------\n");
    
    Grade *grades = load_grades(&gradeCount);
    Task *tasks = load_tasks(&taskCount);
    
    if(grades != NULL && tasks != NULL) {
        int found = 0;
        for(int i = 0; i < gradeCount; i++) {
            if(strcmp(grades[i].studentId, studentId) == 0) {
                // Find task title
                char taskTitle[50] = "Unknown";
                for(int j = 0; j < taskCount; j++) {
                    if(strcmp(tasks[j].taskId, grades[i].taskId) == 0) {
                        strcpy(taskTitle, tasks[j].taskTitle);
                        break;
                    }
                }
                printf("%-10s %-20s %-8s %-15s\n", 
                       grades[i].taskId, taskTitle, grades[i].grade, grades[i].teacherName);
                found = 1;
            }
        }
        if(!found) {
            printf("No grades available.\n");
        }
    } else {
        printf("No grade data available.\n");
    }
    
    if(grades) free(grades);
    if(tasks) free(tasks);
    pause_screen();
}

// Utility Functions Implementation
void clear_screen() {
    system("cls");
}

void pause_screen() {
    printf("\nPress any key to continue...");
    getchar();
    getchar();
}

void generate_task_id(char taskId[]) {
    static int counter = 1000;
    sprintf(taskId, "T%d", counter++);
}