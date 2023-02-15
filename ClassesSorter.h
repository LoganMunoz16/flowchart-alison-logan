#ifndef CLASSESSORTER_H
#define CLASSESSORTER_H
#include <string>
#include "Course.h"
#include <deque>
#include <list>
#include <boost/graph/adjacency_list.hpp>
using std::list;
using std::size_t;
using std::string;
using std::deque;
using namespace boost;
struct Vertex {Course c;};
struct Edge {int weight;};
using digraph = adjacency_list<listS, vecS, directedS, Vertex, Edge>;


class ClassesSorter {
	deque<deque<Course>> _schedule;
	deque<Course> _noConstraintsSch;
	::digraph _courseGraph;
	::digraph _copyGraph;

public:
	ClassesSorter(::digraph& g);
	~ClassesSorter();
	void CreateSchedule(int startQ, int credMax, int highestCourseLevel);
	bool PrereqsTaken(Course c);
	void ScheduleWithConstraints(int graphIndex, int startQ, int maxCredits);
	int LatestScheduledPre(int graphIndex);
	void ExtendDeque(int desiredQuarter);
	bool CheckCredits(int desiredQ, int courseCredit, int maxCred);
	deque<int> FindCourses(int level);
	deque<deque<Course>> ViewSchedule();
	deque<Course> ViewNoConstraints();
    	void RemoveSafely(int index);
	bool CheckUD(int index);


    };



#endif /* CLASSESSORTER_H */
