#ifndef COURSE_H
#define COURSE_H
#include <string>
#include <list>
#include <vector>
using std::size_t;
using std::string;
using std::list;
using std::vector;

class Course {
	string _courseCode;
	string _courseName;
	int _credits;
	int _quarterScheduled = -1;
	int _courseLevel;


public:
    	bool _taken;
    	list<string> _prereqs;
	vector<int> _prereqIDs;
	int _offered[4] = {-1,-1,-1,-1};
    	int index;
	Course();
    	Course(string courseCode, string courseName, int credits);
	~Course();
	string GetCourseCode();
	string GetCourseName();
	int GetCredits();
	void SetTaken();
	bool IsTaken();
	void Print();
	void PrintFull();
	void SetScheduled(int quarterNum);
	int GetScheduled();
	int SetCourseLevel(int level);
	int GetCourseLevel();
};



#endif /* COURSE_H */
