#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ACCOUNTS 150
#define MAX_NAME 60
#define MAX_USER 32
#define MAX_PASS 32
#define MAX_PROGRAM 16
#define MAX_CLASSES 40
#define MAX_CLASS_NAME 80

typedef enum { ROLE_STUDENT, ROLE_TEACHER, ROLE_REGISTRAR, ROLE_ADMIN } Role;

typedef struct {
	char username[MAX_USER];
	char password[MAX_PASS];
	char fullname[MAX_NAME];
	Role role;
	char program[MAX_PROGRAM]; /* only for students */
	int logged_in;
	char classes[MAX_CLASSES][MAX_CLASS_NAME];
	int class_count;
	int time_in;
} Account;

/* Global storage */
Account accounts[MAX_ACCOUNTS];
int account_count = 0;

/* Function Prototypes */
void clearScreen();
void pause_wait();
void seed_demo();
void main_menu();
void create_account();
void login();
int find_account(const char *username);
void forgot_password();
void about_school();

/* Role menus */
void student_menu(int idx);
void teacher_menu(int idx);
void registrar_menu(int idx);
void admin_menu(int idx);

/* Student functions */
void student_profile(int idx);
void student_view_schedule(int idx);
void student_enroll(int idx);
void student_request_certificate(int idx);

/* Teacher functions */
void teacher_time(int idx);
void teacher_attendance(int idx);
void College_teacher_schedule(int idx);
void teacher_class_record(int idx);

/* Registrar functions */
void registrar_time(int idx);
void registrar_view_students();

/* Admin functions */
void admin_time(int idx);
void admin_list_accounts();
void admin_manage_faculty();

/* ---------------- IMPLEMENTATION ---------------- */

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void pause_wait() {
	printf("\nPress Enter to continue...");
	getchar();
}

int main(void) {
	seed_demo();
	main_menu();
	return 0;
}

void seed_demo() {
	/* admin */
	strcpy(accounts[0].username, "admin");
	strcpy(accounts[0].password, "admin");
	strcpy(accounts[0].fullname, "System Admin");
	accounts[0].role = ROLE_ADMIN;
	account_count++;

	/* registrar */
	strcpy(accounts[1].username, "registrar");
	strcpy(accounts[1].password, "regis");
	strcpy(accounts[1].fullname, "School Registrar");
	accounts[1].role = ROLE_REGISTRAR;
	account_count++;

	/* teacher */
	strcpy(accounts[2].username, "teacher");
	strcpy(accounts[2].password, "teacher");
	strcpy(accounts[2].fullname, "COLLEGE TEACHER");
	accounts[2].role = ROLE_TEACHER;
	account_count++;

	/* student (BSCS) */
	strcpy(accounts[3].username, "bscs1");
	strcpy(accounts[3].password, "bscs1");
	strcpy(accounts[3].fullname, "BSCS Student");
	accounts[3].role = ROLE_STUDENT;
	strcpy(accounts[3].program, "BSCS");
	accounts[3].class_count = 0;
	account_count++;

	/* example BSOA student */
	strcpy(accounts[4].username, "bsoa1");
	strcpy(accounts[4].password, "bsoa");
	strcpy(accounts[4].fullname, "BSOA Student");
	accounts[4].role = ROLE_STUDENT;
	strcpy(accounts[4].program, "BSOA");
	accounts[4].class_count = 0;
	account_count++;

	/* example BTVTED student */
	strcpy(accounts[5].username, "btvted1");
	strcpy(accounts[5].password, "btvted");
	strcpy(accounts[5].fullname, "BTVTED Student");
	accounts[5].role = ROLE_STUDENT;
	strcpy(accounts[5].program, "BTVTED");
	accounts[5].class_count = 0;
	account_count++;
}

void main_menu() {
	int running = 1;
	while (running) {
		clearScreen();
		printf("|=== SCHOOL MANAGEMENT SYSTEM  ===|\n");
		printf("| 1) Login                        |\n");
		printf("| 2) Create account               |\n");
		printf("| 3) Forgot password              |\n");
		printf("| 4) About                        |\n");
		printf("| 5) Exit                         |\n");
		printf("|---------------------------------|\n");
		printf("Choose: ");
		int opt = 0;
		if (scanf("%d", &opt) != 1) {
			while(getchar()!='\n');
			opt = 0;
		}
		while(getchar()!='\n');
		switch(opt) {
			case 1:
				login();
				break;
			case 2:
				create_account();
				break;
			case 3:
				forgot_password();
				break;
			case 4:
				about_school();
				break;
			case 5:
				running = 0;
				break;
			default:
				printf("Invalid choice.\n");
				break;
		}
		if (running) pause_wait();
	}
	printf("\nExiting... Goodbye Baby ko!\n");
}

int find_account(const char *username) {
	for (int i = 0; i < account_count; ++i)
		if (strcmp(accounts[i].username, username) == 0)
			return i;
	return -1;
}

void create_account() {
	clearScreen();
	if (account_count >= MAX_ACCOUNTS) {
		printf("Account limit reached.\n");
		return;
	}

	Account a;
	memset(&a, 0, sizeof(Account));
	printf("--- CREATE ACCOUNT ---\n");
	printf("Username: ");
	scanf("%31s", a.username);
	while(getchar()!='\n');

	if (find_account(a.username) != -1) {
		printf("Username already exists!\n");
		return;
	}

	printf("Full name: ");
	fgets(a.fullname, MAX_NAME, stdin);
	if (a.fullname[strlen(a.fullname)-1]=='\n') a.fullname[strlen(a.fullname)-1]='\0';

	printf("Password: ");
	scanf("%31s", a.password);
	while(getchar()!='\n');

	printf("Pleas Select Your Role Please");
	printf(" 1. Student   ");
	printf(" 2. Teacher   ");
	printf(" 3. Registrar ");
	printf(" 4. Admin     :");
	int r;
	scanf("%d", &r);
	while(getchar()!='\n');
	if (r < 1 || r > 4) {
		printf("Invalid role.\n");
		return;
	}

	a.role = (Role)(r-1);
	a.class_count = 0;
	a.time_in = 0;

	if (a.role == ROLE_STUDENT) {
		printf("Program (BSCS/BSOA/BTVTED): ");
		scanf("%15s", a.program);
		while(getchar()!='\n');
	}

	accounts[account_count++] = a;
	printf("Account successfully created for %s!\n", a.fullname);
}

void login() {
	clearScreen();
	char uname[MAX_USER], pass[MAX_PASS];
	printf("---- LOGIN ---\n");
	printf("Username: ");
	scanf("%31s", uname);
	printf("Password: ");
	scanf("%31s", pass);
	while(getchar()!='\n');

	int idx = find_account(uname);
	if (idx == -1) {
		printf("Account not found!\n");
		return;
	}
	if (strcmp(accounts[idx].password, pass) != 0) {
		printf("Incorrect password.\n");
		return;
	}

	clearScreen();
	printf("Welcome, %s!\n", accounts[idx].fullname);

	switch(accounts[idx].role) {
		case ROLE_STUDENT:
			student_menu(idx);
			break;
		case ROLE_TEACHER:
			teacher_menu(idx);
			break;
		case ROLE_REGISTRAR:
			registrar_menu(idx);
			break;
		case ROLE_ADMIN:
			admin_menu(idx);
			break;
		default:
			printf("Unknown role.\n");
			break;
	}
}

void forgot_password() {
	clearScreen();
	char uname[MAX_USER];
	printf("--- FORGOT PASSWORD ---\n");
	printf("Enter username: ");
	scanf("%31s", uname);
	while(getchar()!='\n');

	int idx = find_account(uname);
	if (idx == -1) {
		printf("Username not found.\n");
		return;
	}

	printf("Enter new password: ");
	scanf("%31s", accounts[idx].password);
	while(getchar()!='\n');
	printf("Password updated successfully!\n");
}

void about_school() {
	clearScreen();
	printf("--- ABOUT SCHOOL ---\n");
	printf("History: Established in 1990 as a center for innovation.\n");
	printf("Mission: Philippine Technological Institute of Science, Arts and Trade, Inc. (PHILTECH) is dedicated to give quality education to the development of every Filipino. It aims to Hone Students who are values oriented and phiysically, socially, academically, socially and spiritually committed to the achievement of life-long learning and service to the nation.\n");
	printf("Vision: PHILTECH shall emerge as one of the educational institutions in the Philippines that can provide academic, appropriate techonology and prduce graduates who are globally competetive and shall serve as pillar in this challenging world.\n");
	printf("Goals: Academic excellence, leadership, and service.\n");
}

/* ---------------- STUDENT ---------------- */

void student_menu(int idx) {
	int run = 1;
	while (run) {
		clearScreen();
		printf("--- STUDENT MENU (%s) ---\n", accounts[idx].fullname);
		printf("1) Profile\n");
		printf("2) View Schedule\n");
		printf("3) Enroll Class\n");
		printf("4) Request Certificate\n");
		printf("5) Sign Out\n");
		printf("Choose: ");
		int opt;
		scanf("%d", &opt);
		while(getchar()!='\n');
		switch(opt) {
			case 1:
				student_profile(idx);
				break;
			case 2:
				student_view_schedule(idx);
				break;
			case 3:
				student_enroll(idx);
				break;
			case 4:
				student_request_certificate(idx);
				break;
			case 5:
				run = 0;
				break;
			default:
				printf("Invalid choice.\n");
				break;
		}
		if (run) pause_wait();
	}
}

void student_profile(int idx) {
	clearScreen();
	printf("--- PROFILE ---\n");
	printf("Name: %s\nUsername: %s\nProgram: %s\n", accounts[idx].fullname, accounts[idx].username, accounts[idx].program);
	printf("Change password? (y/n): ");
	char ch = getchar();
	while(getchar()!='\n');
	if (tolower(ch)=='y') {
		char newpass[MAX_PASS];
		printf("New password: ");
		scanf("%31s", newpass);
		while(getchar()!='\n');
		strcpy(accounts[idx].password, newpass);
		printf("Password changed.\n");
	}
}

/* ------------------ SCHEDULES (BSOA / BSCS / BTVTED) ------------------ */

void student_view_schedule(int idx) {
	clearScreen();
	printf("--- WEEKLY CLASS SCHEDULE ---\n\n");

	if (strcmp(accounts[idx].program, "BSOA") == 0 || strcmp(accounts[idx].program, "BSOA1M1") == 0) {
		printf("PROGRAM: BSOA 1st Year - Bachelor of Science in Office Administration\n\n");
		printf("|----------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("| TIME         | MON                | TUE                  | WED                | THU                | FRI                | SAT                |\n");
		printf("|----------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|  7:00 - 8:00 |                    |                      |                    |                    |                    |                    |\n");
		printf("|  8:00 - 8:45 |                    |                      |                    |                    |                    | NSTP1 404 MAIN     |\n");
		printf("|  8:45 - 9:00 |                                   B R E A K T I M E            |                    |                    |                    |\n");
		printf("|  9:00 - 10:00|                    |                      |                    |                    |                    | NSTP1 404 MAIN     |\n");
		printf("| 10:00 - 10:45|                    |                      |                    |                    |                    | NSTP1 404 MAIN     |\n");
		printf("| 10:45 - 11:15|                                   L U N C H   B R E A K        |                    |                    |                    |\n");
		printf("| 11:15 - 12:00|                    | GE104 403 MAIN       |                    |                    |                    |                    |\n");
		printf("| 12:00 - 1:00 |                    |                      |                    |                    |                    |                    |\n");
		printf("|  1:00 - 2:00 | GE104 104-ANNEX    | OACC102 LEC 411-MAIN |                    | GE2 408-MAIN       |                    |                    |\n");
		printf("|  2:00 - 2:45 |                    | OACC102 LEC 411-MAIN |                    | GE2 408 MAIN       |                    | GE 101 404 MAIN    |\n");
		printf("|  2:45 - 3:00 |                                   B R E A K T I M E                                                                           |\n");
		printf("|  3:00 - 4:00 |                    | OACC102 LAB CL2-MAIN |                    | GE2 408-MAIN       |                    | GE 101 404 MAIN    |\n");
		printf("|  4:00 - 4:45 | OACC101 102-ANNEX  | OACC102 LAB CL2-MAIN |                    | PATHFIT1 408-MAIN  |                    | GE 101 404 MAIN    |\n");
		printf("|  4:45 - 5:15 |                                   B R E A K T I M E                                                                           |\n");
		printf("|  5:15 - 6:00 | OACC101 102-ANNEX  | OACC102 LAB CL2-MAIN |                    | PATHFIT1 408-MAIN  |                    |                    |\n");
		printf("|  6:00 - 7:00 |                    | OACC102 LAB CL2-MAIN |                    | PLS 408 MAIN       |                    |                    |\n");
		printf("|----------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("\nSUBJECTS:\n");
		printf(" GE101    - Understanding the Self                      | Mr. Vele\n");
		printf(" GE104    - Readings in Philippine History              | Mr. Lacerna\n");
		printf(" GE2      - Mathematics in the Modern World             | Ms. Igharas\n");
		printf(" OACC101  - Foundations of Shorthand                    | Ms. Octavo\n");
		printf(" OACC102  - Keyboarding and Documents Processing        | Ms. Rendora\n");
		printf(" PATHFIT1 - Movement Competency Training                | Ms. Rendora\n");
		printf(" PLS      - PhilTech Life and Spirit                    | Mr. Atienza\n");
		printf(" NSTP1    - National Service Training Program I (CWTS)  | Mr. Atienza\n");
	}

	else if (strcmp(accounts[idx].program, "BSCS") == 0) {
		printf("PROGRAM: BSCS - Bachelor of Science in Computer Science\n\n");
		printf("|---------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("| TIME         | MON                | TUE                  | WED                | THU               | FRI                | SAT                |\n");
		printf("|---------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("| 7:00 - 8:00  |                    |                      |                    |                   |                    |                    |\n");
		printf("| 8:00 - 8:45  |                    |                      |                    |                   |                    |                    |\n");
		printf("| 8:45 - 9:00  |                                B R E A K T I M E                                                                             |\n");
		printf("| 9:00 - 10:00 |                    |                      |                    |                   |                    |                    |\n");
		printf("| 10:00 - 10:45|                    |                      |                    |                   |                    |                    |\n");
		printf("| 10:45 - 11:15|                                L U N C H   B R E A K                                                                         |\n");
		printf("| 11:15 - 12:00| CC111 LEC 405 MAIN |                      |                    |                   | CC112 LEC 405 MAIN | GE111 - 406 MAIN   |\n");
		printf("| 12:00 - 1:00 | CC112 CL1 MAIN     |                      |                    |                   | CC111 LEC 405 MAIN | GE111 - 406 MAIN   |\n");
		printf("| 1:00 - 2:00  |                    | GE112 - 405 MAIN     |                    |                   | GE113 405 MAIN     | NSTP1 - 406 MAIN   |\n");
		printf("| 2:00 - 2:45  | CC112 CL1 MAIN     |                      |                    |                   |                    |                    |\n");
		printf("| 2:45 - 3:00  |                                   B R E A K T I M E                                                                          |\n");
		printf("| 3:00 - 4:00  |                    | GE112 - 405 MAIN     |                    |                   | CC111 LAB 406 MAIN | NSTP1 - 406 MAIN   |\n");
		printf("| 4:00 - 4:45  | PLS 105 - ANNEX    | GE112 - 405 MAIN     |                    |                   | CC111 LAB 406 MAIN | NSTP1 - 406 MAIN   |\n");
		printf("| 4:45 - 5:15  |                                   B R E A K T I M E                                                                          |\n");
		printf("| 5:15 - 6:00  | GE113 105 - ANNEX  | PATHFIT 1 - 405 MAIN |                    |                   | CC111 LAB 406 MAIN |                    |\n");
		printf("| 6:00 - 7:00  | GE113 105 - ANEX   | PATHFIT 1 - 405 MAIN |                    |                   |                    |                    |\n");
		printf("|---------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("\nSUBJECTCODE | SUBJECT DESCRIP:\n");
		printf(" GE 111       - Understanding the self                               | Mr. Vele\n");
		printf(" GE 112       - Mathematics In the World                             | Mr. Rodrigues\n");
		printf(" CC 111       - Introduction To Computing With Productivity tools    | Mr. Jimenez\n");
		printf(" CC 112       - Fundamentals of Programming 1                        | Mr. Jimenez\n");
		printf(" PLS 1        - PHILTECH life and Spirit                             | Ms. Rendora\n");
		printf(" GE 113       - Readings in the Philippine History                   | Mr. Lancerna\n");
		printf(" PATHFIT 1    - Movement Competency Training                         | Ms. Rendora\n");
		printf(" NSTP 1       - National Service Training Program 1 (CWTS)           | Mr. Atenza\n");
	}

	else if (strcmp(accounts[idx].program, "BTVTED") == 0 || strcmp(accounts[idx].program, "EDUC1M1") == 0) {
		printf("PROGRAM: BTVTED 1st Year - Bachelor in Technical Vocational Teacher Education Major in FSM\n\n");
		printf("|-------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|  TIME        | MON                | TUE                 | WED                | THU                | FRI                | SAT			    |\n");
		printf("|-------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|  7:00 - 8:00 |                    |                     |                    |                    |                    |                  |\n");
		printf("|  8:00 - 8:45 | TLE101 LAB CL2-MAIN|                     |                    |                    |                    | FCC101 403-MAIN  |\n");
		printf("|  8:45 - 9:00 |                                   B R E A K T I M E                                                                        |\n");
		printf("|  9:00 - 10:00| TLE101 LAB CL2-MAIN|                     |                    |                    |                    | FCC101 403-MAIN  |\n");
		printf("| 10:00 - 10:45|                    |                     |                    |                    |                    | FCC101 403-MAIN  |\n");
		printf("| 10:45 - 11:15|                                   L U N C H   B R E A K                                                                    |\n");
		printf("| 11:15 - 12:00|                    |                     | GE102 104-ANNEX    |                    |                    | NSTP1 403-MAIN   |\n");
		printf("| 12:00 - 1:00 | TLE102 482-MAIN    | GE101 403-MAIN      |                    |                    |                    |                  |\n");
		printf("|  1:00 - 2:00 |                    |                     |                    |                    |                    | NSTP1 403-MAIN   |\n");
		printf("|  2:00 - 2:45 |                    |                     | PATHFIT1 104-ANNEX |                    |                    | FCC102 403-MAIN  |\n");
		printf("|  2:45 - 3:00 |                                   B R E A K T I M E                                                                        |\n");
		printf("|  3:00 - 4:00 | PLS102 ANNEX       | TLE101 LEC 403-MAIN | PATHFIT1 104-ANNEX |                    |                    | FCC102 403-MAIN  |\n");
		printf("|  4:00 - 4:45 |                    |                     | GE103 104-ANNEX    |                    |                    | FCC102 403-MAIN  |\n");
		printf("|  4:45 - 5:15 |                                   B R E A K T I M E                                                                        |\n");
		printf("|  5:15 - 6:00 |                    |                     | GE103 104-ANNEX    |                    |                    |                  |\n");
		printf("|  6:00 - 7:00 |                    |                     |                    |                    |                    |                  |\n");
		printf("|-------------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("\n SUBJECT CODE     | SUBJECTS:\n");
		printf(" GE101     - Understanding the Self\n");
		printf(" GE102     - Readings in Philippine History\n");
		printf(" GE103     - Purposive Communication\n");
		printf(" PLS102    - PhilTech Life and Spirit\n");
		printf(" PATHFIT1  - Movement Competency Training\n");
		printf(" TLE101    - Basic Drafting and Workshop Practice\n");
		printf(" TLE102    - Basic Electronics\n");
		printf(" FCC101    - Fundamentals of Food Chemistry\n");
		printf(" FCC102    - Fundamentals of Food Processing\n");
		printf(" NSTP1     - National Service Training Program I (CWTS)\n");
	}

	else {
		printf("No schedule available for your program yet.\n");
	}

	printf("\n(Note: This schedule is fixed per program.)\n");
}


void student_enroll(int idx) {
	clearScreen();
	if (accounts[idx].class_count >= MAX_CLASSES) {
		printf("Class list full.\n");
		return;
	}
	printf("Enter class name: ");
	fgets(accounts[idx].classes[accounts[idx].class_count], MAX_CLASS_NAME, stdin);
	if (accounts[idx].classes[accounts[idx].class_count][strlen(accounts[idx].classes[accounts[idx].class_count])-1]=='\n')
		accounts[idx].classes[accounts[idx].class_count][strlen(accounts[idx].classes[accounts[idx].class_count])-1]='\0';
	accounts[idx].class_count++;
	printf("Class added!\n");
}

void student_request_certificate(int idx) {
	clearScreen();
	printf("Enter purpose of certificate: ");
	char purpose[100];
	fgets(purpose, sizeof(purpose), stdin);
	if (purpose[strlen(purpose)-1]=='\n') purpose[strlen(purpose)-1]='\0';
	printf("Certificate request submitted: %s\n", purpose);
}

/* ---------------- TEACHER ---------------- */
void teacher_menu(int idx) {
	int run = 1;
	while (run) {
		clearScreen();
		printf("--- TEACHER MENU (%s) ---\n", accounts[idx].fullname);
		printf("1) Time In/Out\n2) Class Attendance\n3) Schedule\n4) Class Record\n5) Sign Out\nChoose: ");
		int opt;
		scanf("%d",&opt);
		while(getchar()!='\n');
		switch(opt) {
			case 1:
				teacher_time(idx);
				break;
			case 2:
				teacher_attendance(idx);
				break;
			case 3:
				College_teacher_schedule(idx);
				break;
			case 4:
				teacher_class_record(idx);
				break;
			case 5:
				run = 0;
				break;
			default:
				printf("Invalid choice.\n");
				break;
		}
		if (run) pause_wait();
	}
}
  
  
void teacher_time(int idx) {
	clearScreen();
	printf("1) Time In\n2) Time Out\nChoose: ");
	int o;
	scanf("%d",&o);
	while(getchar()!='\n');
	if (o==1) {
		accounts[idx].time_in=1;
		printf("Time In recorded.\n");
	} else if (o==2) {
		accounts[idx].time_in=0;
		printf("Time Out recorded.\n");
	} else printf("Invalid.\n");
}

void teacher_attendance(int idx) {
	clearScreen();
	printf("Enter class name: ");
	char cls[100];
	fgets(cls, sizeof(cls), stdin);
	if (cls[strlen(cls)-1]=='\n') cls[strlen(cls)-1]='\0';
	printf("Enter student usernames present: ");
	char list[200];
	fgets(list, sizeof(list), stdin);
	if (list[strlen(list)-1]=='\n') list[strlen(list)-1]='\0';
	printf("Attendance saved for %s: %s\n", cls, list);
}

/* --------- SCHEDULE COLLEGE TEACHER --------- */

void College_teacher_schedule(int idx) {
	clearScreen();
	printf("------- COLLEGE TEACHER SCHEDULE ----------\n\n");
	
	if (strcmp(accounts[idx].program, "teacher") == 0 || strcmp (accounts[idx].program, "Teacher") == 0) {
		printf("PROGRAM: SIR CARL JIMENEZ\n\n");
		printf("|---------------------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|      TIME	     |      MON            |      TUE        |       WED       |      THUS        |      FRI             |        SAT       |\n");
		printf("|----------------|---------------------|-----------------|-----------------|------------------|----------------------|------------------|\n");
		printf("|  7:00 - 8:00   |                     |                 |                 |                  |                      |                  |\n");
		printf("|  8:00 - 8:45   |                     |                 |                 |                  |                      |                  |\n");
		printf("|  8:45 - 9:00   |                                B R E A K T I M E                                                                     |\n");
		printf("|  9:00 - 10:00  |                     |                 |                 |                  |                      |                  |\n");
		printf("| 10:00 - 10:45  |                     |                 |                 |                  |                      |                  |\n");
		printf("| 10:45 - 11:15  |                               L U N C H   B R E A K                                                                  |\n");
		printf("| 11:15 - 12:00  | CC111 LEC 405 MAIN  |                 |                 |                  | CC112 LEC 405 MAIN   |                  |\n");
		printf("| 12:00 - 1:00   | CC112 CL1 MAIN      |                 |                 |                  | CC112 LEC 405 MAIN   |                  |\n");
		printf("| 1:00  - 2:00   | CC112 CL1 MAIN      |                 |                 |                  | CC111 LEC 405 MAIN   |                  |\n");
		printf("| 2:00  - 2:45   | CC112 CL1 MAIN      |                 |                 |                  |                      |                  |\n");
		printf("| 2:45  - 3:00   |                                B R E A K T I M E                                                                     |\n");
		printf("| 3:00  - 4:00   |                     |                 |                 |                  | CC111 LAB 406 MAIN   |                  |\n");
		printf("| 4:00  - 4:45   |                     |                 |                 |                  | CC111 LAB 406 MAIN   |                  |\n");
		printf("| 4:45  - 5:15   |                                B R E A K T I M E                                                                     |\n");
		printf("| 5:15  - 6:00   |                     |                 |                 |                  | CC111 LAB 406 MAIN   |                  |\n");
		printf("| 6:00  - 7:00   |                     |                 |                 |                  |                      |                  |\n");
		printf("|--------------------------------------------------------------------------------------------------------------------------------------|\n");
	}
	
}


void teacher_class_record(int idx) {
	clearScreen();
	printf("Class record area (for grades, notes, etc.)\n");
}

/* ---------------- REGISTRAR ---------------- */

void registrar_menu(int idx) {
	int run = 1;
	while (run) {
		clearScreen();
		printf("--- REGISTRAR MENU (%s) ---\n", accounts[idx].fullname);
		printf("1) Time In/Out\n2) View Student Records\n3) Sign Out\nChoose: ");
		int opt;
		scanf("%d",&opt);
		while(getchar()!='\n');
		switch(opt) {
			case 1:
				registrar_time(idx);
				break;
			case 2:
				registrar_view_students();
				break;
			case 3:
				run = 0;
				break;
			default:
				printf("Invalid.\n");
				break;
		}
		if (run) pause_wait();
	}
}

void registrar_time(int idx) {
	clearScreen();
	printf("1) Time In\n2) Time Out\nChoose: ");
	int o;
	scanf("%d",&o);
	while(getchar()!='\n');
	if (o==1) {
		accounts[idx].time_in=1;
		printf("Time In recorded.\n");
	} else if (o==2) {
		accounts[idx].time_in=0;
		printf("Time Out recorded.\n");
	} else printf("Invalid.\n");
}

void registrar_view_students() {
	clearScreen();
	printf("--- STUDENT RECORDS ---\n");
	int found=0;
	for (int i=0; i<account_count; i++) {

		if (accounts[i].role == ROLE_STUDENT) {
			printf("Name: %s | Program: %s | Classes: %d\n", accounts[i].fullname, accounts[i].program, accounts[i].class_count);
			found++;
		}
	}
	if (!found) printf("No student records found.\n");
}

/* ---------------- ADMIN ---------------- */

void admin_menu(int idx) {
	int run = 1;
	while (run) {
		clearScreen();
		printf("--- ADMIN MENU (%s) ---\n", accounts[idx].fullname);
		printf("1) Time In/Out\n2) List All Accounts\n3) Manage Faculty\n4) Sign Out\nChoose: ");
		int opt;
		scanf("%d",&opt);
		while(getchar()!='\n');
		switch(opt) {
			case 1:
				admin_time(idx);
				break;
			case 2:
				admin_list_accounts();
				break;
			case 3:
				admin_manage_faculty();
				break;
			case 4:
				run = 0;
				break;
			default:
				printf("Invalid.\n");
				break;
		}
		if (run) pause_wait();
	}
}

void admin_time(int idx) {
	clearScreen();
	printf("1) Time In\n2) Time Out\nChoose: ");
	int o;
	scanf("%d",&o);
	while(getchar()!='\n');
	if (o==1) {
		accounts[idx].time_in=1;
		printf("Admin Time In recorded.\n");
	} else if (o==2) {
		accounts[idx].time_in=0;
		printf("Admin Time Out recorded.\n");
	} else printf("Invalid.\n");
}

void admin_list_accounts() {
	clearScreen();
	printf("--- ALL ACCOUNTS ---\n");
	for (int i=0; i<account_count; i++) {
		const char *r = accounts[i].role==ROLE_STUDENT?"Student":accounts[i].role==ROLE_TEACHER?"Teacher":accounts[i].role==ROLE_REGISTRAR?"Registrar":"Admin";
		printf("%d) %s | %s | %s\n", i+1, accounts[i].fullname, accounts[i].username, r);
	}
}


void admin_manage_faculty() {
	clearScreen();
	printf("--- FACULTY LIST ---\n");
	int found=0;
	for (int i=0; i<account_count; i++) {
		if (accounts[i].role==ROLE_TEACHER) {
			printf("Teacher: %s | Username: %s\n", accounts[i].fullname, accounts[i].username);
			found++;
		}
	}
	if (!found) printf("No faculty found.\n");
}
