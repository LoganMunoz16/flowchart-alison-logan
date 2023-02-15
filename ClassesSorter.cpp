#include "ClassesSorter.h"
#include <string>
#include <deque>
#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include "Course.h"
#include <boost/graph/adjacency_list.hpp>
using std::list;
using std::deque;
using std::size_t;
using std::string;
using std::cout;
using std::endl;
using std::stack;
using boost::tie;
using boost::adjacent_vertices;
typedef boost::graph_traits<::digraph>::adjacency_iterator AdjacencyIterator;


/* Constructor
 *
 * This takes in a graph and sets the course and copy graph to that input.
 * The copy will be destroyed as we schedule courses and the course will be kept for data reference.
 * @param: A directed graph representative of the courses needed.
 * @return: NA
 */
ClassesSorter::ClassesSorter(::digraph& g) {
	_courseGraph = g;
	_copyGraph = g;
}

/* Destructor
 * Nothing special here since nothing is dynamically allocated by hand - hence default
 */
ClassesSorter::~ClassesSorter() = default;


/* Create Schedule
 *
 * This is essentially a broken up iterative version of a DFS.
 * We find courses of increasing level that have no prereqs (or prereqs taken) and run a DFS on them.
 * The catch is, we only take paths where we are within our course level, so we avoid having high level classes oddly early.
 * This means we iterate through all 1000 level, then 2000 level, etc.
 *
 * @param: An int for the following aspects: starting quarter, maximum credits, and highest class course level
 * @return: NA
 */
void ClassesSorter::CreateSchedule(int startQ, int credMax, int highestCourseLevel) {
	//Creating two iterators to iterate through adjacent notes
	AdjacencyIterator neighbor;
        AdjacencyIterator neighbor_end;	

	//For each course level up to and including your highest
	for(int i = 0; i <= highestCourseLevel; i++) {
		//Create a deque of courses to run the DFS on and schedule
		deque<int> toSchedule = FindCourses(i);

		//For each course found
		for(int v : toSchedule) {
			//The rest is a basic iterative DFS with a few add-ons
			stack<int> myStack;
			myStack.push(v);

			while(!myStack.empty()) {
				int w = myStack.top();
				myStack.pop();
				//This is akin to marking it as visited - but in this case we schedule it!
                		ScheduleWithConstraints(w, startQ, credMax);	

				//Add to no constraints course list
				_noConstraintsSch.push_back(_courseGraph[w].c); 

				//This is boost-style iterator syntax - it basically just loops through adjacent vertices
			      	for(tie(neighbor, neighbor_end) = adjacent_vertices(w, _courseGraph);
				    neighbor != neighbor_end; neighbor++) {
					Course temp = _courseGraph[*neighbor].c;
					//If the course is higher than we want, already taken, or doesn't have prereqs met, skip it
					if (temp.GetCourseLevel() > i || temp.IsTaken() || !PrereqsTaken(temp)) {
						continue;
					} else {
						myStack.push(temp.index);
					}
				}	
			}
		}
	}
}

/* Prereqs Taken
 *
 * Loops through the prereqs of the given course to see if all are taken. If so returns true, if not, returns false.
 *
 * @param: A course to check the prereqs for
 * @return: boolean for if the prereqs are taken or not
 */
bool ClassesSorter::PrereqsTaken(Course c) {
	int mySize = c._prereqIDs.size();
	for (int i = 0; i < mySize; i++) {
		if(!_courseGraph[c._prereqIDs.at(i)].c.IsTaken())
			return false;
	}
	return true;

}

/* Schedule With Constraints
 *
 * At it's core, this schedules the given course when it is able to be scheduled.
 * This is taking into account the starting quarter, the maximum credits per quarter, 
 * when it is offered, and also when its prereqs are scheduled.
 *
 * More detail will be given step by step, but generally the algorithm looks for the earliest
 * quarter that is after all the prereqs are already scheduled and places this class there.
 *
 * @param: ints for the following items: the graph index of the course to schedule, the starting quarter, and the max credits
 * @return: NA
 */
void ClassesSorter::ScheduleWithConstraints(int graphIndex, int startQ, int maxCredits) {
	Course temp = _courseGraph[graphIndex].c;
	for(int i = 0; i < 4; i++) {
		if(temp._offered[i] > -1 && CheckUD(graphIndex))
			temp._offered[i] = temp._offered[i] + 4;
	}

	//Gets the quarter of the latest prereq scheduled
	int latestPrereq = LatestScheduledPre(graphIndex);
	
	//Continue searching while we haven't found a spot
	while(!_courseGraph[graphIndex].c.IsTaken()) {
		
		//Loops through Summer, Fall, Winter, Spring - denoted by 0, 1, 2, 3 respectively
		for(int i = 0; i < 4; i++) {
			//In our Course object, the offering is a -1 if not offered that quarter, so we can skip those
			if (temp._offered[i] < 0) {
				continue;

			//Is the quarter we are looking to schedule this at beyond the latest prereq?	
			} else if (temp._offered[i] <= latestPrereq) {
				//If not, add multiples of 4 until we are far enough out - adding 4 moves a full year but keeps quarter type
				temp._offered[i] = temp._offered[i] + (4 * (((latestPrereq - temp._offered[i]) / 4) + 1));
				//The math for this was figured out through experimentation, and helps cut back on the times this loops runs

			//We have a viable quarter, now would we go over our credit amount? Or is this before we are starting? Is it an UD?	
			} else if(!CheckCredits(temp._offered[i], temp.GetCredits(), maxCredits) || temp._offered[i] < startQ) {
			       //If so go to the next year
				temp._offered[i] = temp._offered[i] + 4;			
			
			} else {
				//If these are equal, we can do a normal push_back to add the new quarter with the course in it
				if (_schedule.size() == temp._offered[i]) {
					deque<Course> tempC;
					tempC.push_back(_courseGraph[graphIndex].c);
					_schedule.push_back(tempC);
				
				//If we are lacking in size, extend the deque as needed and then push_back
				} else if (_schedule.size() < temp._offered[i]) {
					ExtendDeque(temp._offered[i]); //Extends deque to needed size
					deque<Course> tempC;
					tempC.push_back(_courseGraph[graphIndex].c);
					_schedule.push_back(tempC);

				//Otherwise we are in a quarter that already exists, so just add the course there!
				} else {
					_schedule[temp._offered[i]].push_back(_courseGraph[graphIndex].c);

				}
				//Mark this course as taken and record when it was scheduled
				_courseGraph[graphIndex].c.SetTaken();
				_courseGraph[graphIndex].c.SetScheduled(temp._offered[i]);
				break;
			}
		}
	}
	//Safely remove this course from the copy graph (we aren't touching the other one as it is used for data reference)
	RemoveSafely(graphIndex);
}

/* Latest Scheduled Prereq
 *
 * Loops through the prereqs for this course and finds the one scheduled the latest (max int)
 *
 * @param: int for the graph index
 * @return: an int representing the latest prereq
 */
int ClassesSorter::LatestScheduledPre(int graphIndex) {
	int latest = -1;
	int mySize = _courseGraph[graphIndex].c._prereqIDs.size();
	for(int i = 0; i < mySize; i++) {
		if (_courseGraph[_courseGraph[graphIndex].c._prereqIDs.at(i)].c.GetScheduled() > latest)
			latest = _courseGraph[_courseGraph[graphIndex].c._prereqIDs.at(i)].c.GetScheduled();
	}
	return latest;
}

/* Extend Deque
 *
 * Calculates how many quarters needed to be added to the schedule and adds them.
 *
 * @param: int representing the desired quarter in the schedule.
 * @return: NA
 */
void ClassesSorter::ExtendDeque(int desiredQuarter) {
	int quartersToAdd = desiredQuarter - _schedule.size();
	for(int i = 0; i < quartersToAdd; i++) {
		deque<Course> temp;
		_schedule.push_back(temp);
	}
}

/* Check Credits
 *
 * Calculates what the credit total would be for the given quarter and returns true if this course can fit
 *
 * @param: ints representings the quarter to schedule at, the credits your course would have, and the max credits possible
 * @return: boolean true if scheduling is ok, and false if not
 */
bool ClassesSorter::CheckCredits(int desiredQ, int courseCredit, int maxCred) {
	int currentCredits = 0;
	
	//If we will end up adding a new quarter anyway, then this is definitly alright
	if(_schedule.size() <= desiredQ)
		return true;

	//Sum up what the credits currently are in that quarter
	for(Course c : _schedule[desiredQ]) {
		currentCredits += c.GetCredits();

	}
	
	//Return false if adding these credits takes us over our max
	if(currentCredits + courseCredit > maxCred)
		return false;
	else
		return true;
}

/* Find Courses
 *
 * Finds courses of the given level that have no more prereqs needed (or none to begin with)
 * This makes use of the copy graph which deletes prereqs as they are taken.
 *
 * @param: int for the level of course you want
 * @return: deque filled with the courses that are viable
 */
deque<int> ClassesSorter::FindCourses(int level) {
	deque<int> noPrereqs;
	//For each vertex still in the graph
	for(int i = 0; i < _copyGraph.m_vertices.size(); i++) {
		Vertex v = _copyGraph[i];

		//If the vertex has no more prereqs and is the right level, add it
		if(v.c._prereqIDs.empty() && v.c.GetCourseLevel() == level) {
			noPrereqs.push_back(v.c.index);
		} 
	}

	return noPrereqs;
}

/* Remove Safely
 *
 * Removes the given course from the copy graph and takes care of removing it from other courses prereqs too.
 * This helps to preserve all the data, and assists with finding which courses to take in the above method.
 *
 * @param: int for the index of the vertex in the *original* graph
 * @return: NA
 */
void ClassesSorter::RemoveSafely(int index) {
	
	//When we removes them boost compensates by shifting the indeces of the rest, so we have to watch for that.
	int shiftedIndex = 0;

	//Go through all vertices
	for(int i = 0; i < _copyGraph.m_vertices.size(); i++) {
		if(_copyGraph[i].c.index == index) {
			shiftedIndex = i; //grab the index that matches this course but in the copy graph
		}

		//If this course has the one we are deleting in its prereqs, remove it
		for(int j = 0; j < _copyGraph[i].c._prereqIDs.size(); j++) {
			if(_copyGraph[i].c._prereqIDs.at(j) == index) {
				_copyGraph[i].c._prereqIDs.erase(_copyGraph[i].c._prereqIDs.begin()+j);
			}
		}
	}

	//Officially remove this vertex from the copy graph.
	boost::remove_vertex(shiftedIndex, _copyGraph);
}

/* View Schedule
 *
 * Just returns the deque containing the schedule - done to keep it private and protected again it getting messed up
 *
 * @param: NA
 * @return: returns the schedule you created!
 */
deque<deque<Course>> ClassesSorter::ViewSchedule() {
	return _schedule;
}

/* View Schedule With No Constraints
 *
 * Just returns the deque containing the schedule with no constraints
 * 	 done to keep it private and protected again it getting messed up
 *
 * @param: NA
 * @return: returns the schedule you created!
 */
deque<Course> ClassesSorter::ViewNoConstraints() {
	return _noConstraintsSch;
}

bool ClassesSorter::CheckUD(int index) {
	if(_courseGraph[index].c.GetCourseLevel() >= 3 && _courseGraph[index].c._prereqs.size() == 0)
		return true;
	else
		return false;
}
