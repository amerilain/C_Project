#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_COURSES 10
#define MAX_STUDENTS 50
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 200

typedef struct {
    char name[MAX_NAME_LENGTH];
    int grade;
} Student;

typedef struct {
    char title[MAX_NAME_LENGTH];
    int studentCount;
    Student students[MAX_STUDENTS];
} Course;

// Function declarations
bool isValidGrade(int grade);
double calculateCourseAverage(Course *course);
double calculateStudentAverage(const char *studentName, Course courses[], int courseCount);
int isBlank(const char *str);
void printStudentReport(const char *studentName, Course courses[], int courseCount);

int main() {
    Course courses[MAX_COURSES];
    int courseCount = 0;

    char filename[MAX_NAME_LENGTH];
    printf("Please enter the filename containing course information (e.g., student_db.txt): ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    Course *currentCourse = NULL;
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        if (isBlank(line)) continue;

        if (line[0] == '[') {
            if (courseCount >= MAX_COURSES) {
                printf("Error: Maximum number of courses reached.\n");
                break;
            }
            sscanf(line, "[%[^]]", courses[courseCount].title);
            courses[courseCount].studentCount = 0;
            currentCourse = &courses[courseCount];
            courseCount++;
        } else if (currentCourse) {
            Student student;
            if (sscanf(line, "%[^:]: %d", student.name, &student.grade) != 2) {
                printf("Error on line %d: Incorrectly formatted student data.\n", lineNumber);
                continue;
            }
            if (!isValidGrade(student.grade)) {
                printf("Error on line %d: Invalid grade.\n", lineNumber);
                continue;
            }
            if (currentCourse->studentCount < MAX_STUDENTS) {
                currentCourse->students[currentCourse->studentCount] = student;
                currentCourse->studentCount++;
            } else {
                printf("Error: Maximum number of students for a course reached.\n");
            }
        } else {
            printf("Error on line %d: Student data found before course title.\n", lineNumber);
        }
    }
    fclose(file);

    while (1) {
        char studentName[MAX_NAME_LENGTH];
        printf("\nEnter student's full name (e.g., Sarah Johnson) or 'stop' to exit: ");
        scanf(" %[^\n]", studentName);  // Read entire line until newline

        if (strcmp(studentName, "stop") == 0) {
            break;
        }

        printStudentReport(studentName, courses, courseCount);
    }

    for (int i = 0; i < courseCount; i++) {
        printf("Average grade for course %s: %.2f\n", courses[i].title, calculateCourseAverage(&courses[i]));
    }

    return 0;
}

// Function definitions

bool isValidGrade(int grade) {
    return grade >= 0 && grade <= 5;
}

double calculateCourseAverage(Course *course) {
    int total = 0;
    for (int i = 0; i < course->studentCount; i++) {
        total += course->students[i].grade;
    }
    return (double)total / course->studentCount;
}

double calculateStudentAverage(const char *studentName, Course courses[], int courseCount) {
    int total = 0;
    int count = 0;
    for (int i = 0; i < courseCount; i++) {
        for (int j = 0; j < courses[i].studentCount; j++) {
            if (strcmp(courses[i].students[j].name, studentName) == 0) {
                total += courses[i].students[j].grade;
                count++;
            }
        }
    }
    return count > 0 ? (double)total / count : 0;
}

int isBlank(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

void printStudentReport(const char *studentName, Course courses[], int courseCount) {
    printf("\nReport for %s:\n", studentName);
    printf("%-25s %s\n", "Course", "Grade");
    printf("------------------------- ------\n");
    bool found = false;
    for (int i = 0; i < courseCount; i++) {
        for (int j = 0; j < courses[i].studentCount; j++) {
            if (strcmp(courses[i].students[j].name, studentName) == 0) {
                printf("%-25s %d\n", courses[i].title, courses[i].students[j].grade);
                found = true;
            }
        }
    }
    if (!found) {
        printf("No courses found for student %s\n", studentName);
    }
    printf("Average grade for student %s: %.2f\n", studentName, calculateStudentAverage(studentName, courses, courseCount));
}