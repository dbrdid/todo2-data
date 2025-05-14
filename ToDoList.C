#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // access() 함수 사용 //05월 10일 추가

#define MAX_TASKS 100
#define MAX_STR 100

// ✅ Task 구조체 정의
// 작업의 카테고리, 제목, 날짜, 완료 여부, 우선순위를 저장
typedef struct {
    char category[MAX_STR];
    char title[MAX_STR];
    char date[11];
    int completed;
    int priority; // ✅ 우선순위: 1(낮음) ~ 5(매우 높음)
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;

// 날짜를 비교를 위해 정수로 변환 (예: 2024-05-07 -> 20240507)
int convertDateToInt(const char* date) {
    int year, month, day;
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    return year * 10000 + month * 100 + day;
}

// 날짜 기준 정렬
void sortTasksByDate() {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = i + 1; j < taskCount; j++) {
            if (convertDateToInt(tasks[i].date) > convertDateToInt(tasks[j].date)) {
                Task temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

// 전체 작업 보기
void viewTasks() {
    printf("\n===== 전체 작업 목록 =====\n");
    for (int i = 0; i < taskCount; i++) {
        printf("[%d] [%s] (우선:%d) %s - %s %s\n", i + 1,
            tasks[i].category, tasks[i].priority,
            tasks[i].title, tasks[i].date,
            tasks[i].completed ? "(완료)" : "");
    }
}

// 날짜순으로 작업 보기
void viewTasksByDate() {
    sortTasksByDate();
    printf("\n===== 날짜순 작업 목록 =====\n");
    for (int i = 0; i < taskCount; i++) {
        printf("[%d] [%s] (우선:%d) %s - %s %s\n", i + 1,
            tasks[i].category, tasks[i].priority,
            tasks[i].title, tasks[i].date,
            tasks[i].completed ? "(완료)" : "");
    }
}

// 달력형 보기 (같은 날짜끼리 묶음)
void viewTasksByCalendar() {
    sortTasksByDate();
    printf("\n===== 달력형 작업 뷰 =====\n");
    char prevDate[11] = "";
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(prevDate, tasks[i].date) != 0) {
            printf("\n📅 %s\n", tasks[i].date);
            strcpy(prevDate, tasks[i].date);
        }
        printf("  - [%s] (우선:%d) %s %s\n",
            tasks[i].category, tasks[i].priority,
            tasks[i].title, tasks[i].completed ? "(완료)" : "");
    }
}
// 날짜 형식 유효성 검사 (YYYY-MM-DD)
int isValidDateFormat(const char* date) {
    int year, month, day;
    
    // 형식 맞는지 확인
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3)
        return 0;

    // 길이 체크
    if (strlen(date) != 10)
        return 0;

    // 월, 일 범위 체크
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    return 1;
}


// 카테고리로 필터링하여 보기
void viewTasksByCategory() {
    char searchCategory[MAX_STR];
    printf("검색할 카테고리 입력: ");
    scanf("%s", searchCategory);

    printf("\n===== %s 카테고리 작업 =====\n", searchCategory);
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].category, searchCategory) == 0) {
            printf("[%d] (우선:%d) %s - %s %s\n", i + 1,
                tasks[i].priority,
                tasks[i].title,
                tasks[i].date,
                tasks[i].completed ? "(완료)" : "");
        }
    }
}

// 제목 또는 카테고리 검색 (문자열 포함)
void searchTasks() {
    char keyword[MAX_STR];
    printf("검색 키워드 입력 (제목 또는 카테고리): ");
    scanf("%s", keyword);

    printf("\n===== 검색 결과 =====\n");
    for (int i = 0; i < taskCount; i++) {
        if (strstr(tasks[i].title, keyword) || strstr(tasks[i].category, keyword)) {
            printf("[%d] [%s] (우선:%d) %s - %s %s\n", i + 1,
                tasks[i].category, tasks[i].priority,
                tasks[i].title, tasks[i].date,
                tasks[i].completed ? "(완료)" : "");
        }
    }
}
// 작업을 파일에 저장   05월 10일 추가
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("파일 열기 실패\n");
        return;
    }
    for (int i = 0; i < taskCount; i++) {
        fprintf(fp, "%s %s %s %d %d\n",
            tasks[i].category, tasks[i].title,
            tasks[i].date, tasks[i].completed,
            tasks[i].priority);
    }
    fclose(fp);
}

// JSON 포맷으로 저장
void saveTasksAsJSON() {
    FILE *fp = fopen("tasks.json", "w");
    if (access("tasks.txt", F_OK) == 0) {
        rename("tasks.txt", "tasks_backup.txt");
    }
    saveToFile("tasks.txt");
    if (!fp) {
        printf("파일 저장 실패\n");
        return;
    }
    fprintf(fp, "[\n");
    for (int i = 0; i < taskCount; i++) {
        fprintf(fp, "  {\"category\": \"%s\", \"title\": \"%s\", \"date\": \"%s\", \"completed\": %d, \"priority\": %d}%s\n",
            tasks[i].category, tasks[i].title, tasks[i].date, tasks[i].completed, tasks[i].priority,
            (i < taskCount - 1) ? "," : "");
    }
    fprintf(fp, "]\n");
    fclose(fp);
    printf("JSON 형식으로 저장 완료\n");
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("더 이상 작업을 추가할 수 없습니다.\n");
        return;
    }

    printf("카테고리 입력: ");
    scanf("%s", tasks[taskCount].category);
    getchar(); // 입력 버퍼에 남은 개행 제거

    printf("제목 입력: ");
    fgets(tasks[taskCount].title, MAX_STR, stdin);
    tasks[taskCount].title[strcspn(tasks[taskCount].title, "\n")] = '\0'; // 개행 제거

    char dateInput[20];
    while (1) {
        printf("날짜 입력 (YYYY-MM-DD): ");
        scanf("%s", dateInput);
        if (isValidDateFormat(dateInput)) {
            strcpy(tasks[taskCount].date, dateInput);
            break;
        } else {
            printf("❌ 날짜 형식이 올바르지 않습니다. 예: 2025-05-07\n");
        }
    }

    while (1) {
        printf("우선순위 입력 (1~5): ");
        scanf("%d", &tasks[taskCount].priority);
        if (tasks[taskCount].priority >= 1 && tasks[taskCount].priority <= 5)
            break;
        else
            printf("❌ 우선순위는 1에서 5 사이여야 합니다.\n");
    }

    tasks[taskCount].completed = 0;
    taskCount++;
    printf("작업이 추가되었습니다.\n");
}
    


// 완료 표시
void markTaskCompleted() {
    int num;
    viewTasks();
    printf("완료할 작업 번호를 입력하세요: ");
    scanf("%d", &num);
    if (num < 1 || num > taskCount) {
        printf("잘못된 번호입니다.\n");
        return;
    }
    tasks[num - 1].completed = 1;
    printf("작업이 완료 처리되었습니다.\n");
}

// 보기 메뉴 (서브메뉴)
void viewMenu() {
    int sub;
    printf("\n[보기 메뉴]\n1. 전체 보기\n2. 날짜순 보기\n3. 달력형 보기\n4. 카테고리별 보기\n5. 검색\n선택: ");
    scanf("%d", &sub);
    switch (sub) {
        case 1: viewTasks(); break;
        case 2: viewTasksByDate(); break;
        case 3: viewTasksByCalendar(); break;
        case 4: viewTasksByCategory(); break;
        case 5: searchTasks(); break;
        default: printf("잘못된 선택입니다.\n");
    }
}

// 메인 함수
int main() {
    int choice;
    while (1) {
        printf("\n==== TODO LIST MENU ===="
            "\n1. 작업 추가"
            "\n2. 작업 보기"
            "\n3. 작업 완료 처리"
            "\n10. JSON으로 저장"
            "\n0. 종료"
            "\n선택: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addTask(); break;
            case 2: viewMenu(); break; // ✅ 서브메뉴 호출
            case 3: markTaskCompleted(); break;
            case 10: saveTasksAsJSON(); break;
            case 0: exit(0);
            default: printf("잘못된 입력입니다.\n");
        }
    }
    https://github.com/dbrdid/todo2-data/blob/main/tasks.json
    return 0;
}
