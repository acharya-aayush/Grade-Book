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
