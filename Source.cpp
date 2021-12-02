#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

string db = "database";

bool path_exists(const char* path);
void create_folder(const char* path);
void delete_folder(const char* path);
vector<string> read_files(const char* path);
string read_file(const char* path);
int count_lines_in_file(const char* path);
void remove_line_and_save_file(const char* path, string match);
void create_file(const char* path, const char* content);
void delete_file(const char* path);
void clear_screen();

class User;
class Student;
class Teacher;
class Evaluation;
class Semester;
class Course;
class Section;
class Attendance;
class GradesList;
class Queue;

class Queue {
public:
	string rollNumber;
	string semesterCode;
	string courseCode;
	string sectionCode;
	int slot_avail;
	Queue(string sem, string cour, string sec, string roll, int slotavail = 0) {
		semesterCode = sem;
		courseCode = cour;
		sectionCode = sec;
		slot_avail = slotavail;
		rollNumber = roll;
	}
	void setSlotFree() {
		slot_avail = 1;
		remove_line_and_save_file("database/queue.txt", rollNumber + " " + semesterCode + " " + courseCode + " " + sectionCode);
		ofstream fout("database/queue.txt", ofstream::out | ofstream::app);
		fout << rollNumber << " " << semesterCode << " " << courseCode << " " << sectionCode << " " << (int)1 << endl;
		fout.close();
	}
};

class GradesList {
public:
	string semesterCode;
	string courseCode;
	string sectionCode;
	vector<string> rollNumber;
	vector<string> grades;
	GradesList() {

	}
	GradesList(string sem, string cour, string sec) {
		semesterCode = sem;
		courseCode = cour;
		sectionCode = sec;
	}
	void addGrade(string roll, string grade) {
		int x = -1;
		for (int i = 0; i < rollNumber.size() && x == -1; i++) {
			if (rollNumber[i] == roll)
				x = i;
		}
		if (x != -1) {
			grades[x] = grade;
		}
		else {
			rollNumber.push_back(roll);
			grades.push_back(grade);
		}
	}

	void load() {
		ifstream fin("database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/grades.txt");
		while (!fin.eof()) {
			string a, b;
			fin >> a; fin >> b;
			addGrade(a, b);
		}
		fin.close();
	}

	void save() {
		ofstream fout("database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/grades.txt");
		for (int i = 0; i < rollNumber.size(); i++) {
			fout << rollNumber[i] << " " << grades[i] << endl;
		}
		fout.close();
	}

	string getGrade(string roll) {
		int x = -1;
		for (int i = 0; i < rollNumber.size() && x == -1; i++) {
			if (rollNumber[i] == roll)
				x = i;
		}

		if (x == -1)
			return "-";
		else
			return grades[x];
	}
};
class Evaluation {
public:
	string semesterCode;
	string courseCode;
	string sectionCode;
	string evaluationName;
	int totalMarks;
	vector<string> rollNumber;
	vector<float> marks;
	Evaluation(string sem, string cour, string sec, string name, int total = 0) {
		semesterCode = sem;
		courseCode = cour;
		sectionCode = sec;
		evaluationName = name;
		totalMarks = total;
	}

	void load() {
		string path = "database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/evaluations/" + evaluationName + ".txt";
		ifstream fin(path);
		fin >> totalMarks;
		string a; float b;
		while (!fin.eof()) {
			fin >> a;
			fin >> b;
			addEvaluation(a, b);
		}
		fin.close();
	}

	void save() {
		string path = "database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/evaluations/" + evaluationName + ".txt";
		ofstream fout(path);
		fout << totalMarks << endl;
		for (int i = 0; i < rollNumber.size(); i++) {
			fout << rollNumber[i] << " " << marks[i] << endl;
		};
		fout.close();
	}

	void addEvaluation(string roll, float marks) {
		int x = -1;
		for (int i = 0; i < rollNumber.size() && x == -1; i++) {
			if (rollNumber[i] == roll)
				x = i;
		}
		if (x != -1) {
			this->marks[x] = marks;
		}
		else {
			this->rollNumber.push_back(roll);
			this->marks.push_back(marks);
		}
	}
};
class Attendance {
public:
	string date;
	string semesterCode;
	string courseCode;
	string sectionCode;
	vector<string> rollNumber;
	vector<string> status;
	Attendance(string date, string semester, string course, string section) {
		this->date = date;
		this->semesterCode = semester;
		this->courseCode = course;
		this->sectionCode = section;
		this->status = status;

		/* do it other
		string path = "database/semester/"+semester+"/"+course+"/"+section+"/attendance/"+date+".txt";
		ofstream fout(path);

		fout.close();
		*/
	}
	void addAttendance(string roll, string status) {
		int x = -1;
		for (int i = 0; i < rollNumber.size() && x == -1; i++) {
			if (rollNumber[i] == roll)
				x = i;
		}
		if (x != -1) {
			this->status[x] = status;
		}
		else {
			this->rollNumber.push_back(roll);
			this->status.push_back(status);
		}
	}

	void load() {
		string path = "database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/attendance/" + date + ".txt";
		ifstream fin(path);
		string a, b;
		while (!fin.eof()) {
			fin >> a; fin >> b;
			addAttendance(a, b);
		}
		fin.close();
	}

	void save() {
		string path = "database/semester/" + semesterCode + "/" + courseCode + "/" + sectionCode + "/attendance/" + date + ".txt";
		ofstream fout(path);
		for (int i = 0; i < rollNumber.size(); i++) {
			fout << rollNumber[i] << " " << status[i] << endl;
		}
		fout.close();
	}
};

class User {
public:
	string name;
	vector<Semester*> semesters; //section is a reference in it
	int findSemester(vector<Semester*> semesters, string name);
	int findCourse(vector<Course*> courses, string name);
	int findSection(vector<Section*> sections, string name);
	bool selectSemCourSec(vector<Semester*> semesters, Semester*& sem, Course*& cour, Section*& sec);
	void loadData(string semesterCode, string courseCode, string courseName, Section* sec, bool is_student);
};
class Student : public User {
public:
	string rollNumber;
	Student(string roll) {
		rollNumber = roll;
	}
	Student(string roll, string fullName) {
		rollNumber = roll;
		name = fullName;
	}
	Student(string roll, vector<Semester*> vec) {
		rollNumber = roll;
		semesters = vec;
	}
	bool isStudentEnrolledInAnySectionOfThisCourse(string sem, string cour);
	bool addCourse(Semester* sem, Course* cour, Section* sec);
	bool dropCourse(string& a, string& b, string& c);
	bool withdrawCourse(string& a, string& b, string& c);
	void viewAttendance();
	void viewTranscript();
};
class Teacher : public User {
public:
	string employeeID;
	string name;
	Teacher(string id, string fullname) {
		employeeID = id, name = fullname;
	}
	void addAttendance();
	void addEvaluation();
	void assignMarks();
	void displayMarksSheet();
	void assignGrade();

};

class Section {
public:
	string sectionCode;
	int maxStudents;
	int currStudents;

	vector<Evaluation*> evaluations;
	vector<Attendance*> attendance;
	vector<Student*> students; //reference
	GradesList gradesList;

	Section(string code, int curr = 0, int max = 50) {
		sectionCode = code; currStudents = curr; maxStudents = max;
	}
	Section(string code, vector<Student*> studentsList, int curr = 0, int max = 50) {
		sectionCode = code; currStudents = curr; maxStudents = max;
		students = studentsList;
	}
	void addStudent(Student* s);
	void addAttendance(string semesterCode, string courseCode);
	void createEvaluation(Evaluation* e);
	void addMarksToEvaluation();
	void displayMarksSheet();
	void assignGrade();
	~Section();
};
class Course {
public:
	string courseCode;
	string courseName;
	vector<Section*> sections;
	Course(string code, string name) {
		courseCode = code;
		courseName = name;
	}
};
class Semester {
public:
	string semesterName;
	vector<Course*> courses;
	Semester(string s) {
		semesterName = s;
	}
};



//FORWARD DECLARATIONS
//User
int User::findSemester(vector<Semester*> semesters, string name) {
	int res = -1;
	for (int i = 0; i < semesters.size() && res == -1; i++) {
		if (semesters[i]->semesterName == name) {
			res = i;
		}
	}
	return res;
}
int User::findCourse(vector<Course*> courses, string name) {
	int res = -1;
	for (int i = 0; i < courses.size() && res == -1; i++) {
		if (courses[i]->courseCode == name || courses[i]->courseName == name) {
			res = i;
		}
	}
	return res;
}
int User::findSection(vector<Section*> sections, string name) {
	int res = -1;
	for (int i = 0; i < sections.size() && res == -1; i++) {
		if (sections[i]->sectionCode == name) {
			res = i;
		}
	}
	return res;
}
bool User::selectSemCourSec(vector<Semester*> semesters, Semester*& sem, Course*& cour, Section*& sec) {
	if (semesters.size() == 0) {
		cout << "You are not enrolled in any semester." << endl;
		system("pause");
		return false;
	}

	int a, b, c;
	for (int i = 0; i < semesters.size(); i++) {
		cout << i << ". " << semesters[i]->semesterName << endl;
	}
	cout << "Select semester: "; cin >> a;
	if (a > semesters.size() - 1)
		return false;

	if (semesters[a]->courses.size() == 0) {
		cout << "You are not enrolled in any course in this semester." << endl;
		system("pause");
		return false;
	}
	for (int i = 0; i < semesters[a]->courses.size(); i++) {
		cout << i << ". " << semesters[a]->courses[i]->courseCode << " - " << semesters[a]->courses[i]->courseName << endl;
	}
	cout << "Select course: "; cin >> b;
	if (b > semesters[a]->courses.size() - 1)
		return false;

	if (semesters[a]->courses[b]->sections.size() == 0) {
		cout << "You are not enrolled in any section in this course." << endl;
		system("pause");
		return false;
	}
	for (int i = 0; i < semesters[a]->courses[b]->sections.size(); i++) {
		cout << i << ". " << semesters[a]->courses[b]->sections[i]->sectionCode << endl;
	}
	cout << "Select section: "; cin >> c;
	if (c > semesters[a]->courses[b]->sections.size() - 1)
		return false;

	sem = semesters[a];
	cour = sem->courses[b];
	sec = cour->sections[c];

	return true;
}
void User::loadData(string semesterCode, string courseCode, string courseName, Section* sec, bool is_student = true) {
	int x;
	x = findSemester(semesters, semesterCode);
	if (x == -1) {
		Semester* obj = new Semester(semesterCode);
		semesters.push_back(obj);
		x = semesters.size() - 1;
	}
	int y;
	y = findCourse(semesters[x]->courses, courseCode);
	if (y == -1) {
		Course* obj2 = new Course(courseCode, courseName);
		obj2->sections.push_back(sec);
		semesters[x]->courses.push_back(obj2);
		y = semesters[x]->courses.size() - 1;
	}
	else {
		if (is_student)
			cout << "Error: Student " << name << " is already enrolled in " << courseName << endl;
		else {
			semesters[x]->courses[y]->sections.push_back(sec);
		}
	}
}
//Student
bool Student::isStudentEnrolledInAnySectionOfThisCourse(string sem, string cour) {
	int x = findSemester(semesters, sem);
	if (x == -1) {
		return false;
	}
	int y = findCourse(semesters[x]->courses, cour);
	if (y == -1) {
		return false;
	}
	return true;
}
bool Student::addCourse(Semester* sem, Course* cour, Section* sec) {
	if (sec->currStudents >= sec->maxStudents) {
		cout << "Sorry! No seats available, but you can add yourself in a queue." << endl;
		system("pause");
		return false;
	};
	int x;
	x = findSemester(semesters, sem->semesterName);
	if (x == -1) {
		Semester* obj = new Semester(sem->semesterName);
		semesters.push_back(obj);
		x = semesters.size() - 1;
	}
	int y;
	y = findCourse(semesters[x]->courses, cour->courseCode);
	if (y == -1) {
		Course* obj2 = new Course(cour->courseCode, cour->courseName);
		obj2->sections.push_back(sec);
		semesters[x]->courses.push_back(obj2);
		y = semesters[x]->courses.size() - 1;

		sec->currStudents++;

		ofstream fout("database/registered-courses/" + rollNumber + ".txt", ofstream::out | ofstream::app);
		fout << semesters[x]->semesterName; fout << " ";
		fout << semesters[x]->courses[y]->courseCode; fout << " ";
		fout << semesters[x]->courses[y]->sections[0]->sectionCode;
		fout << endl;
		fout.close();

		fout.open("database/semester/" + semesters[x]->semesterName + "/" + semesters[x]->courses[y]->courseCode + "/" + semesters[x]->courses[y]->sections[0]->sectionCode + "/students.txt",
			ofstream::out | ofstream::app);
		fout << rollNumber << endl;
		fout.close();

		return true;
	}
	else {
		cout << "You are already enrolled in a section for this course." << endl;
		return false;
	}
};
bool Student::dropCourse(string& a, string& b, string& c) {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return false;
	}

	int x;
	x = findSemester(semesters, sem->semesterName);
	if (x == -1) {
		return false;
	}
	int y;
	y = findCourse(semesters[x]->courses, cour->courseCode);
	if (y == -1) {
		return false;
	}

	sec->currStudents--;

	a = sem->semesterName; b = cour->courseCode; c = sec->sectionCode;

	remove_line_and_save_file(("database/registered-courses/" + rollNumber + ".txt").c_str(),
		sem->semesterName + " " + cour->courseCode + " " + sec->sectionCode);

	remove_line_and_save_file(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/students.txt").c_str(),
		rollNumber);

	remove_line_and_save_file(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/grades.txt").c_str(),
		rollNumber);

	//remove attendance
	vector<string> attendance_days = read_files(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/attendance/").c_str());
	for (int i = 0; i < attendance_days.size(); i++) {
		remove_line_and_save_file(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/attendance/" + attendance_days[i] + ".txt").c_str(),
			rollNumber);
	}

	//remove evaluations
	vector<string> evaluation_names = read_files(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/evaluations/").c_str());
	for (int i = 0; i < evaluation_names.size(); i++) {
		remove_line_and_save_file(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/evaluations/" + evaluation_names[i] + ".txt").c_str(),
			rollNumber);
	}

	delete semesters[x]->courses[y];
	semesters[x]->courses.erase(semesters[x]->courses.begin() + y);
	if (semesters[x]->courses.size() == 0) {
		delete semesters[x];
		semesters.erase(semesters.begin() + x);
	}

	return true;
};
bool Student::withdrawCourse(string& a, string& b, string& c) {
	return dropCourse(a, b, c);
};
void Student::viewAttendance() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}

	cout << "------------Attendance------------" << endl;
	for (int i = 0; i < sec->attendance.size(); i++) {
		Attendance* at = sec->attendance[i];
		bool flag = false;
		for (int j = 0; j < at->rollNumber.size() && !flag; j++) {
			if (at->rollNumber[j] == rollNumber) {
				flag = true;
				cout << at->date << "\t" << at->status[j] << endl;
			}
		}
	}

	cout << endl;
}
void Student::viewTranscript() {
	cout << "------------Transcript------------" << endl;
	for (int i = 0; i < semesters.size(); i++) {
		cout << semesters[i]->semesterName << endl;
		for (int j = 0; j < semesters[i]->courses.size(); j++) {
			bool flag = false;
			string grade = "-";
			for (int k = 0; !flag && k < semesters[i]->courses[j]->sections[0]->gradesList.rollNumber.size(); k++) {
				if (rollNumber == semesters[i]->courses[j]->sections[0]->gradesList.rollNumber[k]) {
					flag = true;
					grade = semesters[i]->courses[j]->sections[0]->gradesList.grades[k];
				}
			}

			cout << "\t" << semesters[i]->courses[j]->courseCode << "\t" << semesters[i]->courses[j]->courseName
				<< "\t(" << semesters[i]->courses[j]->sections[0]->sectionCode << ")\t"
				<< grade << endl;
		}
		cout << endl;
	}

};
//Teacher
void Teacher::addAttendance() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}

	sec->addAttendance(sem->semesterName, cour->courseCode);
}
void Teacher::addEvaluation() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}

	string name;
	cout << "Evaluation Name: "; cin >> name;
	Evaluation* e = new Evaluation(sem->semesterName, cour->courseCode, sec->sectionCode, name);
	sec->createEvaluation(e);
}
void Teacher::assignMarks() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}

	sec->addMarksToEvaluation();

	/*
	int opt;

	vector<string> evaluation_names = read_files(("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/evaluations/").c_str());
	for (int i = 0; i < evaluation_names.size(); i++) {
		cout << i << ". " << evaluation_names[i] << endl;
	}
	cout << "Your selection: "; cin >> opt;
	if (opt < evaluation_names.size()) {
		ofstream fout("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/evaluations/" + evaluation_names[opt] + ".txt");
		cout << "What are total marks for this evaluation?" << endl;
		cin >> opt;
		fout << opt << endl;
		cout << "Add marks for the following students in your evaluation called " << evaluation_names[opt] << endl;

		ifstream fout2("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/students.txt");
		string line;
		while (getline(fout2, line)) {
			cout << line << ": ";
			float marks;
			cin >> marks;
			fout << line << " " << marks << endl;
		}
		fout2.close();

		fout.close();
	}
	*/
}
void Teacher::displayMarksSheet() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}
	sec->displayMarksSheet();
}
void Teacher::assignGrade() {
	Semester* sem;
	Course* cour;
	Section* sec;
	if (!(selectSemCourSec(semesters, sem, cour, sec))) {
		return;
	}

	sec->assignGrade();

	/*
	cout << "Add grades for the following students in your section. " << endl;
	ofstream fout("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/grades.txt");
	ifstream fout2("database/semester/" + sem->semesterName + "/" + cour->courseCode + "/" + sec->sectionCode + "/students.txt");
	string line;
	while (getline(fout2, line)) {
		cout << line << ": ";
		string grade;
		cin >> grade;
		fout << line << " " << grade << endl;
	}
	fout2.close();
	fout.close();
	*/
}
//Section
void Section::addStudent(Student* s) {
	bool flag = false;
	for (int i = 0; i < students.size() && !flag; i++) {
		if (students[i] == s)
			flag = true;
	}
	if (!flag) {
		students.push_back(s);
	}
}
void Section::addAttendance(string semesterCode, string courseCode) {
	if (students.size() == 0) {
		cout << "There are no students in this section." << endl;
		return;
	}
	string date;
	cout << "Date (mm-dd-yyyy): ";
	cin >> date;
	cout << "Enter P for Present and A for Absent for the following students in your class..." << endl;
	Attendance* obj = new Attendance(date, semesterCode, courseCode, sectionCode);
	for (int i = 0; i < students.size(); i++) {
		cout << students[i]->rollNumber << ": ";
		string temp;
		cin >> temp;
		if (temp != "P" && temp != "p")
			temp = "A";
		else
			temp = "P";
		obj->addAttendance(students[i]->rollNumber, temp);
	};
	obj->save();
	attendance.push_back(obj);
}
void Section::createEvaluation(Evaluation* e) {
	bool flag = false;
	for (int i = 0; i < evaluations.size() && !flag; i++) {
		if (evaluations[i]->evaluationName == e->evaluationName)
			flag = true;
	}
	if (flag) {
		cout << "An evaluation with this name already exists: " << e->evaluationName << endl;
		delete e;
	}
	else {
		evaluations.push_back(e);
		//save db
		e->save();
	}
}
void Section::addMarksToEvaluation() {
	if (students.size() == 0) {
		cout << "There are no students in this section." << endl;
		return;
	}

	cout << "Please select one of the following evaluations: " << endl;
	for (int i = 0; i < evaluations.size(); i++) {
		cout << i << ". " << evaluations[i]->evaluationName << endl;
	}
	cout << "Your answer: ";
	int x;
	cin >> x;
	if (x > evaluations.size() - 1)
		return;
	Evaluation* e = evaluations[x];
	cout << "What are the total marks for this evaluation? Answer: ";
	cin >> e->totalMarks;
	cout << "Add marks for the following students in the evaluation: ";
	for (int i = 0; i < students.size(); i++) {
		cout << students[i]->rollNumber << ": ";
		float temp; cin >> temp;

		e->addEvaluation(students[i]->rollNumber, temp);
	};
	cout << "Saving..." << endl;

	//save db
	e->save();
	cout << "Done" << endl;
}
void Section::displayMarksSheet() {
	cout << "------------Marks Sheet------------" << endl;
	for (int i = 0; i < evaluations.size(); i++) {
		Evaluation* e = evaluations[i];
		cout << "Evaluation: " << e->evaluationName << endl;
		for (int j = 0; j < e->rollNumber.size(); j++) {
			cout << "\t" << e->rollNumber[j] << " -> " << e->marks[j] << " out of " << e->totalMarks << endl;
		}
		cout << endl;
	}
}
void Section::assignGrade() {
	if (students.size() == 0) {
		cout << "There are no students in this section." << endl;
		return;
	}

	cout << "Add grades for the following students in your section. " << endl;
	for (int i = 0; i < students.size(); i++) {
		cout << students[i]->rollNumber << ": ";
		string grade;
		cin >> grade;
		gradesList.addGrade(students[i]->rollNumber, grade);
	};
	cout << "Saving..." << endl;
	gradesList.save();
	cout << "Done." << endl;
}
Section::~Section() {
	int s = attendance.size();
	for (int i = 0; i < s; i++) {
		if (attendance[i] != nullptr)
			delete attendance[i];
	}
}

//LMS 
class LMS {
	bool is_student; //false = is_teacher
	int idx;
	int reg_status;
	int withdraw_status;
	vector<Student*> students;
	vector<Teacher*> teachers;
	vector<Semester*> semesters;
	vector<Queue*> queue;

	int findQueueAllStudent(string sem, string cour, string sec, int param = -1) {
		int pos = -1;
		for (int i = 0; i < queue.size() && pos == -1; i++) {
			if (param == -1) {
				if (queue[i]->semesterCode == sem && queue[i]->courseCode == cour && queue[i]->sectionCode == sec) {
					pos = i;
				}
			}
			else {
				if (queue[i]->semesterCode == sem && queue[i]->courseCode == cour && queue[i]->sectionCode == sec && queue[i]->slot_avail == param) {
					pos = i;
				}
			}
		}
		return pos;
	}
	int findQueue(string sem, string cour, string sec, string roll, int param = -1) {
		int pos = -1;
		for (int i = 0; i < queue.size() && pos == -1; i++) {
			if (param == -1) {
				if (queue[i]->semesterCode == sem && queue[i]->courseCode == cour && queue[i]->sectionCode == sec && queue[i]->rollNumber == roll) {
					pos = i;
				}
			}
			else {
				if (queue[i]->semesterCode == sem && queue[i]->courseCode == cour && queue[i]->sectionCode == sec && queue[i]->rollNumber == roll && queue[i]->slot_avail == param) {
					pos = i;
				}
			}
		}
		return pos;
	}
	void addQueue(string sem, string cour, string sec, string roll) {
		Queue* q = new Queue(sem, cour, sec, roll);
		queue.push_back(q);
		ofstream fout("database/queue.txt", ofstream::out | ofstream::app);
		fout << roll << " " << sem << " " << cour << " " << sec << " " << (int)0 << endl;
		fout.close();
	}
	void removeQueue(int pos) {
		if (pos != -1) {
			remove_line_and_save_file("database/queue.txt", queue[pos]->rollNumber + " " + queue[pos]->semesterCode + " " + queue[pos]->courseCode + " " + queue[pos]->sectionCode);
			delete queue[pos];
			queue.erase(queue.begin() + pos);
		}
	}
	void removeQueue(string sem, string cour, string sec, string roll) {
		int pos = findQueue(sem, cour, sec, roll);
		if (pos != -1)
			removeQueue(pos);
	}
	void showQueue(string roll) {
		if (queue.size() > 0) {
			vector<int> v;
			for (int i = 0; i < queue.size(); i++) {
				if (queue[i]->rollNumber == roll && queue[i]->slot_avail == 1) {
					v.push_back(i);
					cout << "Notification: Slot free for: " << queue[i]->semesterCode << " " << queue[i]->courseCode << " " << queue[i]->sectionCode << endl;
				}
			}

			for (int i = 0; i < v.size(); i++) {
				string a = queue[v[i]]->semesterCode, b = queue[v[i]]->courseCode, c = queue[v[i]]->sectionCode;
				removeQueue(v[i]);
				int pos = findQueueAllStudent(a, b, c, 0);
				if (pos != -1)
					queue[pos]->setSlotFree();
			}
			cout << endl;
		}
	}


	bool selectSemCourSec(Semester*& sem, Course*& cour, Section*& sec) {
		int a, b, c;
		for (int i = 0; i < semesters.size(); i++) {
			cout << i << ". " << semesters[i]->semesterName << endl;
		}
		cout << "Select semester: "; cin >> a;
		if (a > semesters.size() - 1)
			return false;
		for (int i = 0; i < semesters[a]->courses.size(); i++) {
			cout << i << ". " << semesters[a]->courses[i]->courseCode << " - " << semesters[a]->courses[i]->courseName << endl;
		}
		cout << "Select course: "; cin >> b;
		if (b > semesters[a]->courses.size() - 1)
			return false;
		for (int i = 0; i < semesters[a]->courses[b]->sections.size(); i++) {
			cout << i << ". " << semesters[a]->courses[b]->sections[i]->sectionCode << endl;
		}
		cout << "Select section: "; cin >> c;
		if (c > semesters[a]->courses[b]->sections.size() - 1)
			return false;

		sem = semesters[a];
		cour = sem->courses[b];
		sec = cour->sections[c];

		return true;
	}

	Student* getStudentPointerByRoll(string roll) {
		Student* s = nullptr;
		for (int i = 0; i < students.size(); i++) {
			if (students[i]->rollNumber == roll) {
				s = students[i];
				break;
			}
		}
		return s;
	}
	Semester* getSemesterByCode(string semesterCode) {
		Semester* s = nullptr;
		for (int i = 0; i < semesters.size(); i++) {
			if (semesters[i]->semesterName == semesterCode) {
				s = semesters[i];
				break;
			}
		}
		return s;
	}
	Course* getCourseInSemester(string semesterCode, string courseCode) {
		Semester* ss = getSemesterByCode(semesterCode);
		Course* s = nullptr;
		if (ss != nullptr) {
			for (int i = 0; i < ss->courses.size(); i++) {
				if (ss->courses[i]->courseCode == courseCode) {
					s = ss->courses[i];
					break;
				}
			}
		}
		return s;
	}
	Section* getSectionInCourse(string semesterCode, string courseCode, string sectionCode) {
		Course* ss = getCourseInSemester(semesterCode, courseCode);

		Section* s = nullptr;
		if (ss != nullptr) {
			for (int i = 0; i < ss->sections.size(); i++) {
				if (ss->sections[i]->sectionCode == sectionCode) {
					s = ss->sections[i];
					break;
				}
			}
		}
		return s;
	}

	void explore(bool debug_semester = true, bool debug_students = true, bool debug_teachers = true) {
		if (debug_semester) {
			cout << "----Semesters----" << endl;
			for (int i = 0; i < semesters.size(); i++) {
				cout << semesters[i]->semesterName << endl;
				for (int j = 0; j < semesters[i]->courses.size(); j++) {
					cout << "\t" << semesters[i]->courses[j]->courseCode << " " << semesters[i]->courses[j]->courseName << endl;
					for (int k = 0; k < semesters[i]->courses[j]->sections.size(); k++) {
						Section* s = semesters[i]->courses[j]->sections[k];
						cout << "\t\t" << s->sectionCode << " " << s->currStudents << " Students-Enrolled " << s->maxStudents << " Max-Seats" << endl;

						cout << "\t\t\tAttendance" << endl;
						if (s->attendance.size() == 0)
							cout << "\t\t\t(None)" << endl;
						for (int l = 0; l < s->attendance.size(); l++) {
							Attendance* a = s->attendance[l];
							cout << "\t\t\tDate: " << a->date << endl;
							for (int m = 0; m < a->rollNumber.size(); m++) {
								cout << "\t\t\t\t" << a->rollNumber[m] << " " << a->status[m] << endl;
							}
						}

						cout << "\t\t\tEvaluations" << endl;
						if (s->evaluations.size() == 0)
							cout << "\t\t\t(None)" << endl;
						for (int l = 0; l < s->evaluations.size(); l++) {
							Evaluation* e = s->evaluations[l];
							cout << "\t\t\tName: " << e->evaluationName << endl;
							for (int m = 0; m < e->rollNumber.size(); m++) {
								cout << "\t\t\t\t" << e->rollNumber[m] << " " << e->marks[m] << " /" << e->totalMarks << endl;
							}
						}

						cout << "\t\t\tGrades" << endl;
						if (s->gradesList.rollNumber.size() == 0)
							cout << "\t\t\t(None)" << endl;
						for (int l = 0; l < s->gradesList.rollNumber.size(); l++) {
							cout << "\t\t\t" << s->gradesList.rollNumber[l] << " " << s->gradesList.grades[l] << endl;
						}

					}
				}
			}
		}
		if (debug_students) {
			cout << "----Students----" << endl;
			for (int i = 0; i < students.size(); i++) {
				cout << students[i]->rollNumber << " " << students[i]->name << endl;
				for (int j = 0; j < students[i]->semesters.size(); j++) {
					Semester* s = students[i]->semesters[j];
					for (int k = 0; k < s->courses.size(); k++) {
						cout << "\t" << s->semesterName << " " << s->courses[k]->courseCode << " " << s->courses[k]->courseName << " " << s->courses[k]->sections[0]->sectionCode << endl;
					}
				};
			};
		}

		if (debug_teachers) {
			cout << "----Teachers----" << endl;
			for (int i = 0; i < teachers.size(); i++) {
				cout << teachers[i]->employeeID << " " << teachers[i]->name << endl;
				for (int j = 0; j < teachers[i]->semesters.size(); j++) {
					Semester* s = teachers[i]->semesters[j];
					for (int k = 0; k < s->courses.size(); k++) {
						cout << "\t" << s->semesterName << " " << s->courses[k]->courseCode << " " << s->courses[k]->courseName << " | ";
						for (int l = 0; l < s->courses[k]->sections.size(); l++) {
							cout << s->courses[k]->sections[l]->sectionCode << " ";
						}
						cout << endl;
					}
				};
			};
		}
	}


	void screen_student_add_queue() {
		Semester* sem;
		Course* cour;
		Section* sec;
		if ((selectSemCourSec(sem, cour, sec))) {
			bool flag = students[idx]->isStudentEnrolledInAnySectionOfThisCourse(sem->semesterName, cour->courseCode);
			if (flag) {
				cout << "Sorry, you are already enrolled in a section for this course." << endl;
			}
			else {
				int pos = findQueue(sem->semesterName, cour->courseCode, sec->sectionCode, students[idx]->rollNumber);
				if (pos == -1)
					addQueue(sem->semesterName, cour->courseCode, sec->sectionCode, students[idx]->rollNumber);
				else
					cout << "You are already enrolled in the queue for this section." << endl;
			}
		}
		else {
			cout << "Invalid entry." << endl;
		}
		system("pause");
		screen_student();
	}
	void screen_student_add_course() {
		Semester* sem;
		Course* cour;
		Section* sec;
		if ((selectSemCourSec(sem, cour, sec))) {
			bool flag = students[idx]->addCourse(sem, cour, sec);
			if (flag) {
				//remove him from queue of all sections in this course
				for (int i = 0; i < cour->sections.size(); i++) {
					removeQueue(sem->semesterName, cour->courseCode, cour->sections[i]->sectionCode, students[idx]->rollNumber);
				}
			}
		}
		else {
			cout << "Invalid entry." << endl;
		}
		system("pause");
		screen_student();
	}
	void screen_student_remove_course() {
		string sem, cour, sec;
		bool flag = students[idx]->dropCourse(sem, cour, sec);
		if (flag) {
			//slot free for one person in this queue.
			int pos = findQueueAllStudent(sem, cour, sec);
			if (pos != -1)
				queue[pos]->setSlotFree();
		}
		system("pause");
		screen_student();
	}
	void screen_student_view_offered_courses() {
		clear_screen();
		cout << "----Semesters----" << endl;
		for (int i = 0; i < semesters.size(); i++) {
			cout << semesters[i]->semesterName << endl;
			for (int j = 0; j < semesters[i]->courses.size(); j++) {
				cout << "\t" << semesters[i]->courses[j]->courseCode << " " << semesters[i]->courses[j]->courseName << endl;
				for (int k = 0; k < semesters[i]->courses[j]->sections.size(); k++) {
					Section* s = semesters[i]->courses[j]->sections[k];
					cout << "\t\t" << s->sectionCode << endl;
				}
			}
		}

		system("pause");
		screen_student();
	}
	void screen_student() {
		clear_screen();
		cout << "Logged in as student: " << students[idx]->name << " (" << students[idx]->rollNumber << ")" << endl << endl;

		showQueue(students[idx]->rollNumber);

		int mode;
		cout << "Please select one of the following options" << endl << endl;
		cout << "1. Add Course" << endl;
		cout << "2. Drop Course" << endl;
		cout << "3. Withdraw Course" << endl;
		cout << "4. View Offered Courses" << endl;
		cout << "5. View Attendance" << endl;
		cout << "6. View Transcript" << endl;
		cout << "7. Add Queue" << endl;
		cout << "8. Go Back" << endl;
		cout << endl << "Your answer (1 - 8): ";
		cin >> mode;
		if (mode == 1) {
			if (reg_status == 1) {
				screen_student_add_course();
			}
			else {
				cout << "Registration period is not active." << endl;
				system("pause");
				screen_student();
			}
		}
		else if (mode == 2) {
			if (reg_status == 1) {
				screen_student_remove_course();
			}
			else {
				cout << "Registration period is not active." << endl;
				system("pause");
				screen_student();
			}
		}
		else if (mode == 3) {
			if (withdraw_status == 1) {
				screen_student_remove_course();
			}
			else {
				cout << "Withdraw is not allowed now." << endl;
				system("pause");
				screen_student();
			}
		}
		else if (mode == 4) {
			screen_student_view_offered_courses();
		}
		else if (mode == 5) {
			students[idx]->viewAttendance();
			system("pause");
			screen_student();
		}
		else if (mode == 6) {
			students[idx]->viewTranscript();
			system("pause");
			screen_student();
		}
		else if (mode == 7) {
			screen_student_add_queue();
		}
		else if (mode == 8) {
			screen_home();
		}
		else {
			clear_screen();
		}

	}
	void screen_teacher() {
		clear_screen();
		cout << "Logged in as teacher: " << teachers[idx]->name << " (" << teachers[idx]->employeeID << ")" << endl << endl;

		int mode;
		cout << "Please select one of the following options" << endl << endl;
		cout << "1. Add Attendance" << endl;
		cout << "2. Create Evaluation" << endl;
		cout << "3. Add Marks In Evaluation" << endl;
		cout << "4. Display Marks Sheet" << endl;
		cout << "5. Add Grade" << endl;
		cout << "6. Go Back" << endl;
		cout << endl << "Your answer (1 - 6): ";
		cin >> mode;
		if (mode == 1) {
			teachers[idx]->addAttendance();
			system("pause");
			screen_teacher();
		}
		else if (mode == 2) {
			teachers[idx]->addEvaluation();
			system("pause");
			screen_teacher();
		}
		else if (mode == 3) {
			teachers[idx]->assignMarks();
			system("pause");
			screen_teacher();
		}
		else if (mode == 4) {
			teachers[idx]->displayMarksSheet();
			system("pause");
			screen_teacher();
		}
		else if (mode == 5) {
			teachers[idx]->assignGrade();
			system("pause");
			screen_teacher();
		}
		else if (mode == 6) {
			screen_home();
		}
		else {
			clear_screen();
		}
	}
	void screen_home() {
		clear_screen();
		int mode;
		cout << "How would you like to proceed?" << endl << endl;
		cout << "1. Login as Student" << endl;
		cout << "2. Login as Teacher" << endl;
		cout << "3. Debug System" << endl;
		cout << "Your answer: ";
		cin >> mode;
		idx = 0;
		if (mode == 1) {
			is_student = true;
			for (int i = 0; i < students.size(); i++) {
				cout << i << ". " << students[i]->name << " (" << students[i]->rollNumber << ")" << endl;
			}
			cout << "Your answer: ";
			cin >> idx;
			if (idx > students.size() - 1) {
				screen_home();
			}
			else {
				screen_student();
			}
		}
		else if (mode == 2) {
			is_student = false;
			for (int i = 0; i < teachers.size(); i++) {
				cout << i << ". " << teachers[i]->name << " (" << teachers[i]->employeeID << ")" << endl;
			}
			cout << "Your answer: ";
			cin >> idx;
			if (idx > teachers.size() - 1) {
				screen_home();
			}
			else {
				screen_teacher();
			}
		}
		else if (mode == 3) {
			debug();
			cout << endl;
			system("pause");
			screen_home();
		}
	}

public:
	LMS() {
		is_student = true;
		idx = 0;
		reg_status = 0;
		withdraw_status = 0;
	}
	~LMS() {
		for (int i = 0; i < semesters.size(); i++) {
			for (int j = 0; j < semesters[i]->courses.size(); j++) {
				for (int k = 0; k < semesters[i]->courses[j]->sections.size(); k++) {
					delete semesters[i]->courses[j]->sections[k];
				}
				delete semesters[i]->courses[j];
			}
			delete semesters[i];
		}
	}
	void debug() {
		explore(true, true, true);
	}
	void init() {
		vector<string> files_semester = read_files("database/semester");
		for (int i = 0; i < files_semester.size(); i++) {
			Semester* obj = new Semester(files_semester[i]);
			semesters.push_back(obj);
			vector<string> files_semester_courses = read_files(("database/semester/" + files_semester[i]).c_str());
			for (int j = 0; j < files_semester_courses.size(); j++) {
				Course* obj2 = new Course(files_semester_courses[j], read_file(("database/semester/" + files_semester[i] + "/" + files_semester_courses[j] + "/name.txt").c_str()));
				obj->courses.push_back(obj2);
				ifstream fin(("database/semester/" + obj->semesterName + "/" + obj2->courseCode + "/sections.txt").c_str());
				while (!fin.eof()) {
					string s; int a, b;
					fin >> s; fin >> b;
					a = count_lines_in_file(("database/semester/" + obj->semesterName + "/" + obj2->courseCode + "/" + s + "/students.txt").c_str());
					Section* obj3 = new Section(s, a, b);
					obj2->sections.push_back(obj3);

					vector<string> arr = read_files(("database/semester/" + obj->semesterName + "/" + obj2->courseCode + "/" + obj3->sectionCode + "/attendance").c_str());
					for (int k = 0; k < arr.size(); k++) {
						Attendance* attendance = new Attendance(arr[k], obj->semesterName, obj2->courseCode, obj3->sectionCode);
						attendance->load();
						obj3->attendance.push_back(attendance);
					}

					arr = read_files(("database/semester/" + files_semester[i] + "/" + files_semester_courses[j] + "/" + obj3->sectionCode + "/evaluations").c_str());
					for (int k = 0; k < arr.size(); k++) {
						Evaluation* evaluation = new Evaluation(obj->semesterName, obj2->courseCode, obj3->sectionCode, arr[i]);
						evaluation->load();
						obj3->evaluations.push_back(evaluation);
					}

					obj3->gradesList.semesterCode = obj->semesterName;
					obj3->gradesList.courseCode = obj2->courseCode;
					obj3->gradesList.sectionCode = obj3->sectionCode;
					obj3->gradesList.load();

				};
			};
		};

		vector<string> files_students = read_files("database/students");
		for (int i = 0; i < files_students.size(); i++) {
			Student* s = new Student(files_students[i], read_file(("database/students/" + files_students[i] + ".txt").c_str()));
			students.push_back(s);

			ifstream fin("database/registered-courses/" + s->rollNumber + ".txt");
			while (!fin.eof()) {
				string semesterName, courseCode, courseName, sectionCode;
				fin >> semesterName;
				fin >> courseCode;
				fin >> sectionCode;
				courseName = read_file(("database/semester/" + semesterName + "/" + courseCode + "/name.txt").c_str());

				Section* section = getSectionInCourse(semesterName, courseCode, sectionCode);
				if (section != nullptr) {
					section->students.push_back(s);
					s->loadData(semesterName, courseCode, courseName, section);
				}
			}
			fin.close();
		}

		vector<string> files_teachers = read_files("database/teacher");
		for (int i = 0; i < files_teachers.size(); i++) {
			Teacher* t = new Teacher(files_teachers[i], read_file(("database/teacher/" + files_teachers[i] + "/name.txt").c_str()));
			teachers.push_back(t);
			ifstream fin("database/teacher/" + t->employeeID + "/teaches.txt");
			while (!fin.eof()) {
				string semesterCode, courseCode, sectionCode;
				fin >> semesterCode; fin >> courseCode; fin >> sectionCode;
				string courseName = read_file(("database/semester/" + semesterCode + "/" + courseCode + "/name.txt").c_str());
				Section* section = getSectionInCourse(semesterCode, courseCode, sectionCode);
				if (section != nullptr)
					t->loadData(semesterCode, courseCode, courseName, section, false);
			};
			fin.close();
		}

		ifstream fin("database/registration-status.txt");
		fin >> reg_status;
		fin.close();
		fin.open("database/withdraw-status.txt");
		fin >> withdraw_status;
		fin.close();
		if (reg_status != 1)
			reg_status = 0;
		if (withdraw_status != 1)
			withdraw_status = 0;

		//load queue if any
		fin.open("database/queue.txt");
		while (!fin.eof()) {
			string a, b, c, d;
			int e;
			fin >> a >> b >> c >> d >> e;
			Queue* q = new Queue(b, c, d, a, e);
			queue.push_back(q);
		}
		fin.close();
	}
	void start() {
		screen_home();
	}
};

int main() {
	LMS lms;
	lms.init();
	lms.start();
}


bool path_exists(const char* path) {
	return fs::exists(path);
}
void create_folder(const char* path) {
	if (!path_exists(path)) {
		fs::create_directory(path);
	}
}
void delete_folder(const char* path) {
	if (path_exists(path)) {
		fs::remove_all(path);
	}
}

string read_file(const char* path) {
	ifstream fin(path);
	string s;
	s.assign((istreambuf_iterator<char>(fin)), (istreambuf_iterator<char>()));
	fin.close();
	return s;
}
vector<string> read_lines_from_file(const char* path) {
	vector<string> v;
	ifstream fin(path);
	string line;
	while (getline(fin, line)) {
		v.push_back(line);
	}
	fin.close();
	return v;
}
vector<string> read_files(const char* path) {
	vector<string> v;

	for (const auto& file : fs::directory_iterator(path))
		v.push_back(fs::path(file.path().string()).stem().string());

	return v;
}
void create_file(const char* path, const char* content) {
	if (!path_exists(path)) {
		ofstream f(path);
		if (content != nullptr) {
			f << content;
		}
		f.close();
	}
}
void delete_file(const char* path) {
	if (path_exists(path)) {
		remove(path);
	};
}
int count_lines_in_file(const char* path) {
	int x = 0;
	if (path_exists(path)) {
		ifstream fin(path);
		string line;
		while (getline(fin, line)) {
			x++;
		}
		fin.close();
	}
	return x;
}
void remove_line_and_save_file(const char* path, string match) {
	if (path_exists(path)) {
		string line;
		string s = "";
		ifstream fin(path);
		while (getline(fin, line)) {
			if (line == match || line.find(match) != string::npos) {
				//found if line matches or contains this as substring
			}
			else {
				s += line + "\n";
			}
		}
		fin.close();
		ofstream fout(path);
		fout << s;
		fin.close();
	};
}
void clear_screen() {
	system("cls");
}