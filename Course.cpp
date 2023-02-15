#include "Course.h"
#include <string>
#include <iostream>
#include <array>
using std::size_t;
using std::string;
using std::cout;
using std::endl;

//Note that this class is more so for the data structure rather than the methods, so comments will be minimal.

//Default Constructor
Course::Course() {

}

//Constructor to initialize a Course object with the code, name, and credit amount
Course::Course(string courseCode, string courseName, int credits) {
	_courseCode = courseCode;
	_courseName = courseName;
	_credits = credits;
	_taken = false;
}

//Default destructor
Course::~Course() = default;

//Getter for the course code
string Course::GetCourseCode() {
	return _courseCode;
}

//Getter for the course name
string Course::GetCourseName() {
	return _courseName;
}

//Getter for the credits
int Course::GetCredits() {
	return _credits;
}

//Setter for the course being taken
void Course::SetTaken() {
	_taken = true;
}

//Getter checking if taken
bool Course::IsTaken() {
	return _taken;
}

//A limited and neater print of the data
void Course::Print() {
	cout << endl;
	cout << "Course Code: " << _courseCode << endl;
	cout << "Course Name:" << _courseName << endl;
	cout << "Credits: " << _credits << endl;
	string prereqList = "";
	for(string s : _prereqs) {
		prereqList += s.substr(0, s.find_first_of("0123456789"));
		prereqList += " ";
		prereqList += s.substr(s.find_first_of("0123456789"));
		prereqList += ", ";
	}
	if(prereqList.size() < 2) {
		prereqList = "None";	
	} else {
		prereqList = prereqList.substr(0, prereqList.length() - 2);
	}
	cout << "Prereqs: " << prereqList << endl;

}

//A rougher print of all the data for testing purposes 
void Course::PrintFull() {
	cout << "Course Code: " << _courseCode << endl;
	cout << "Course Name: " << _courseName << endl;
	cout << "Course Level: " << _courseLevel << endl;
	cout << "Credits: " << _credits << endl;
	cout << "Offered: ";
	for (int i = 0; i < 4; i++) {
		cout << _offered[i] << ", ";
	}
	cout << endl;
	cout << "Prereqs: ";
	for(string s : _prereqs) {
		cout << s << ",";
	}

	cout << endl;
	cout << "PrereqIDs: ";
	for(int i : _prereqIDs)
		cout << i << ",";
	cout << endl << endl;
}

//Setter for when the course is scheduled
void Course::SetScheduled(int quarterNum) {
	_quarterScheduled = quarterNum;
}

//Getter for when the course is scheduled
int Course::GetScheduled() {
	return _quarterScheduled;
}

//Setter for the course level
int Course::SetCourseLevel(int level) {
	_courseLevel = level;
}

//Getter for the course level
int Course::GetCourseLevel() {
	return _courseLevel;
}

