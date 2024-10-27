#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

//Create a structure that will hold the course information
struct Course {
	string courseId;
	string title;
	vector<string> prerequisites;

	Course() {}
	Course(string id, string t, vector<string> prereqs) : courseId(id), title(t), prerequisites(prereqs) {}
};

//create a hash table class that will store course information

class CourseHashTable {
private:
	unordered_map<string, Course> courses;

public:
	//this is the function to insert courses into the hash table
	void InsertCourse(const Course& course) {
		courses[course.courseId] = course;
	}
	//funciton to search for a course by its courseId
	Course SearchCourse(string courseId) {
		if (courses.find(courseId) != courses.end()) {
			return courses[courseId];
		}
		return Course(); // return empty course if a match is not found
	}
	//function to grab all course ID's in the hash table
	vector<Course> GetAllCourses() {
		vector<Course> courseList;
		for (auto const& pair : courses) {
			courseList.push_back(pair.second);
		}
		return courseList;
	}
};
//function to allow courses to be loaded from the provided CSV file
void LoadCoursesFromFile(CourseHashTable& courseTable) {
	string filename = "CS 300 ABCU_Advising_Program_Input.csv";
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "There was an error attempting to open the file" << endl;
		return;
	}
	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string courseId, title, prereq;
		vector<string> prerequisites;
		getline(ss, courseId, ',');
		getline(ss, title, ',');

		while (getline(ss, prereq, ',')) {
			prerequisites.push_back(prereq);
		}
		Course course(courseId, title, prerequisites);
		courseTable.InsertCourse(course);
	}
	file.close();
	cout << "Courses loaded from file successfully." << endl;
}
//function to print out course list from the hash table alphanumerically
void PrintCourseList(CourseHashTable& courseTable) {
	vector<Course> courses = courseTable.GetAllCourses();
	//sort courses by their ID
	sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
		return a.courseId < b.courseId;
		});
	//printing the sorted list
	for (const auto& course : courses) {
		cout << course.courseId << ": " << course.title << endl;
	}
}
//function to print all courses along with their prerequisites
void PrintCourseInformation(CourseHashTable& courseTable, string courseId) {
	Course course = courseTable.SearchCourse(courseId);
	if (course.courseId.empty()) {
		cout << "Error: No courses found." << endl;
		return;
	}
	cout << "Course: " << course.courseId << ": " << course.title << endl;
	if (!course.prerequisites.empty()) {
		cout << "Prerequisites: ";
		for (const auto& prereq : course.prerequisites) {
			cout << prereq << " ";
		}
		cout << endl;
	}
	else {
		cout << "No prerequisites." << endl;
	}
}
int main() {
	CourseHashTable courseTable;
	int choice;
	string courseId;

	do {
		//display the menu
		cout << "Menu:" << endl;
		cout << "1. Load Courses" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course Information" << endl;
		cout << "9. Exit the program" << endl;
		cout << "Enter an option from the list: ";
		cin >> choice;
		
		switch (choice) {
		case 1:
			//Load the courses from the file
			LoadCoursesFromFile(courseTable);
			break;
		case 2:
			PrintCourseList(courseTable);
			break;
		case 3:
			cout << "Enter a course ID to search for: ";
			cin >> courseId;
			PrintCourseInformation(courseTable, courseId);
			break;
		case 9:
			cout << "Exiting program now." << endl;
			break;
		default:
			cout << "Invalid choice, please enter a number from the list" << endl;
			break;
		}
		
	} while (choice != 9);
	return 0;
}