#include <iostream>
#include <string>
#include <utility>
#include <deque>
#include <regex>
#include <list>
#include <vector>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include "Course.h"
#include "ClassesSorter.h"
#include "Display.h"
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::stoi;
using std::isdigit;
using std::size_t;
using std::list;
using std::regex_match;
using std::regex;
using std::deque;
using namespace boost;
using std::vector;
static vector<Course> catalog;
using digraph = adjacency_list<listS, vecS, directedS, Vertex, Edge>;
bool isValid(string str, int info);
void ShowAnimation(int numCourses);
bool GetAnswer();

int main() {
	::digraph g;
    	//1) Select File with the courses' constraints
	ifstream inFile;
	string fileName = "";
	cout << "Please enter the name of the text file containing your list of courses." << endl;
	cout << "If you would like to use a pre-made course flow, type \"major1.txt\" for a Computer Science";
	cout << " major, or type \"major2.txt\" for an Exercise Science major." << endl << endl;
	cout << "Your file: ";
	cin >> fileName;
	inFile.open(fileName);
	if(!inFile.is_open()) {
		cerr << "Could not open file: " << fileName << ", please try again." << endl;
		return 1;
	}
	int minCreditsPossible = 0; //Used to ensure user doesn't try to make impossible schedule - will be the largest credit amount
	vector<Course> catalog;      //list of all classes

	//Several temporary or counter variables for use when parsing
	int class_index = 0;
	string tempCourseCode, tempCourseName, tempCreditString, tempPre, tempOffered, temp;
	int tempCredits, tempCourseLevel, highestCourseLevel = 0;

	//2) Validate the format
	while(inFile.peek() != EOF) {
        	list<string> pres;

		//Parse and validate Course Code while adding in the course level
		tempCourseCode = "";
		getline(inFile, tempCourseCode, ',');
		if(!isValid(tempCourseCode, 1))
			return 1;
		tempCourseLevel = tempCourseCode[tempCourseCode.find_first_of("0123456789")] - 48;
		if(tempCourseLevel > highestCourseLevel)
			highestCourseLevel = tempCourseLevel;

		//Parse and validate the course name
		tempCourseName = "";
		getline(inFile, tempCourseName, ',');
		if(!isValid(tempCourseName, 2))
			return 1;

		//Parse and validate the credit amount, updating the min credits per quarter 
		tempCreditString = "";
		getline(inFile, tempCreditString, ',');
		if(!isValid(tempCreditString, 3))
			return 1;
		boost::replace_all(tempCreditString, " ", "");
		tempCredits = stoi(tempCreditString);
		if(tempCredits > minCreditsPossible)
			minCreditsPossible = tempCredits;

		//Parsing the prerequisites while validating
		tempPre = "";
		getline(inFile, tempPre, ']');
		if(!isValid(tempPre, 4))
			return 1;
		boost::replace_all(tempPre, " ", "");
		tempPre = tempPre.substr(1);

		//If valid then break them up and add them to the list
		if(tempPre.length() < 2) {
        	}else if(tempPre.find_first_of(",") == -1) {
            		pres.push_back(tempPre);
        	}else {
			while(tempPre.find_first_of(",") != -1) {
				int index = tempPre.find_first_of(",");
				temp = tempPre.substr(0, index);
				tempPre = tempPre.substr(index + 1);
                		pres.push_back(temp);
			}
			pres.push_back(tempPre);
		}


		//Parse and validate the course offerings
		tempOffered = "";
		getline(inFile, tempOffered, ']');
		boost::replace_all(tempOffered, " ", "");
		if(!isValid(tempOffered.substr(1), 5))
			return 1;


		//Create a temporary course with all the information we just parsed in
		Course tempCourse(tempCourseCode, tempCourseName, tempCredits);
		for(int i = 0; i < tempOffered.length(); i++) {
			if(isdigit(tempOffered[i])) {
				tempCourse._offered[tempOffered[i] - 48] =  tempOffered[i] - 48;
			}
		}

		for (const string& p : pres){
		    tempCourse._prereqs.push_back(p);
		}

		tempCourse.SetCourseLevel(tempCourseLevel);
		tempCourse.index = class_index;
        	catalog.push_back(tempCourse); //Add this course to the catalog
        	boost::add_vertex(Vertex{tempCourse}, g); //Add this as a vertex in the graph
            	class_index++; //Increment the index to keep everything consistent with the vertex index

        	getline(inFile, tempCourseCode);
	}

	inFile.close();
	cout << endl << endl;
	//3) Create the graph in memory
	for(Course c : catalog){            //for each course
        if (!c._prereqs.empty()){       //if prereqs
            for (const string& p : c._prereqs) {       //get each prereq
                int p_pos = -1;
                int c_pos = -1;
		//Find the link to the prereq and add it in the graph
                for (auto vd : boost::make_iterator_range(vertices(g))){
                    string code = g[vd].c.GetCourseCode();
                    code.erase(remove(code.begin(),code.end(),' '),code.end());
                    string c_code = c.GetCourseCode();
                    c_code.erase(remove(c_code.begin(),c_code.end(),' '),c_code.end());

                    if (code == p && p_pos == -1){
                        p_pos = vd;
                    }
                    if (code == c_code && c_pos == -1){
                        c_pos = vd;
                    }
                }
		if(p_pos > -1 && c_pos > -1) {
                	boost::add_edge(p_pos,c_pos, Edge{}, g);        //Add edge from prereq to class
			g[c_pos].c._prereqIDs.push_back(p_pos);
		} else {
			cerr << "Prerequisite " << p << " for " << c.GetCourseCode() << " could not be added." << endl;
			cerr << "Please check for typos." << endl;
		}
            }
	    }
	}

	//Checking if a DAG - if not then this won't make sense to make a schedule from
	vector<int> checkingDAG(g.m_vertices.size());
	try {
		topological_sort(g, std::back_inserter(checkingDAG));
	} catch(...) {
		cerr << endl;
		cerr << "Whoops! It looks like your course progression isn't valid. Please read over your";
		cerr << " classes list and double check that there are no two classes that have each";
		cerr << " other marked as a prerequisite and try again." << endl;
		return 1;
	}

	//4) Request the user the max credits per quarter
	string maxCredStr;
	int maxCred;
	bool valid = false;
	do {
		cout << "What is your maximum credits per quarter? -";
		cout << " Integer at least as large as your highest credit class (" << minCreditsPossible;
		cout << ")" << endl;
		cin >> maxCredStr;
		cout << endl;
		if(maxCredStr.find_first_not_of("0123456789") == -1 && stoi(maxCredStr) >= minCreditsPossible) {
			valid = true;

		}  else {
			cout << "Whoops! It seems like that is not a valid credit amount." << endl;
			cout << "Please ensure you enter an integer at least as large as your highest credit class.";
			cout << endl;
		}

	} while(!valid);
	maxCred = stoi(maxCredStr);

	//5) Request user starting quarter
	string start_q_str;
	int startQ;
	bool validQ = false;
	do {
		cout << "What is your starting quarter? - Integer [0-3] for [Summer, Fall, Winter, Spring]" << endl;
    		cin >> start_q_str;
		cout << endl;
		if(start_q_str.find_first_of("0123") == -1 || start_q_str.length() != 1) {
			cout << "Whoops! It seems like that is not a valid starting quarter." << endl;
			cout << "Please ensure you enter an integer from 0 to 3.";
			cout << endl;
		} else {
			validQ = true;
		}
		cout << endl;
	}while(!validQ);
	startQ = stoi(start_q_str);


	//6) Course flow

	//Uncomment this code if you wish to double check the graph is looking good - it will show vertices and edges
	//boost::write_graphviz(cout, g, [&] (auto& out, auto v){
	//                        cout << ": ";
	//			g[v].c.Print();
	//                    });
    	//cout << std::flush;

	//Create a ClassesSorter object with the graph we just made
	ClassesSorter mySorter(g);
	mySorter.CreateSchedule(startQ, maxCred, highestCourseLevel); //Creates the schedule in the object instance
	deque<deque<Course>> mySchedule = mySorter.ViewSchedule(); //Access the constrainted schedule
	deque<Course> noConstraints = mySorter.ViewNoConstraints(); //Access the loose schedule

	cout << endl << endl;

	//Output schedule with no constraints
	cout << "Here is an example schedule with no constraints!";
	cout << " That basically means this is just a potential ordering you can take all your classes in." << endl;
	cout << endl;
	int counter = 1;
	for(Course c : noConstraints) {
		cout << counter << ". " << c.GetCourseCode() << endl;
		counter++;
	}
	cout << endl << endl;

	//Output the schedule with constraints
	cout << "And now here is an example taking into account your starting quarter and credit max!" << endl;
	cout << "NOTE: There will be some light quarters here, but getting a degree is more than just your major";
	cout << " specific classes! Those light quarters leave room to fill out all of your gen eds, and of course,";
	cout << " this is just one rendition of what your schedule can look like." << endl;

	int yearCounter = 1;
	for(int i = 0; i < mySchedule.size(); i++) {

		if(i % 4 == 0) {
			cout << endl;
			cout << "Year " << yearCounter << endl;
			cout << "Summer Quarter" << endl;
			yearCounter++;
		} else if(i % 4 == 1) {
			cout << "Fall Quarter" << endl;
		} else if(i % 4 == 2) {
			cout << "Winter Quarter" << endl;
		} else {
			cout << "Spring Quarter" << endl;
		}
		for(Course c : mySchedule[i]) {
			cout << " - " << c.GetCourseCode() << endl;
		}
	}


    //Both of the following Display classes create .dot files, which hold a visualization of the graph and can be converted to pngs

    //Create the output for quarterly graphs visualization - constraints
    Display constraintsGraph;
    constraintsGraph.CreateGraph(mySchedule, catalog);

    //Graphical output for without constraints
    Display normalGraph;
    normalGraph.constraints = false;
    normalGraph.AddEdges(noConstraints, catalog);
    normalGraph.UpdateGraph("noConstraints.dot");

    cout << endl << endl;

    //Ask the user what they want to see and show them everything (creates images and gif)
    ShowAnimation(mySchedule.size());

    cout << "Thank you for using our program, we hope you have a great day!" << endl;


    return 0;
}


//Validates file input of course information
bool isValid (string str, int info) {
	string temp = str;
	boost::replace_all(temp, " ", ""); //Getting rid of spaces to make validating more simple
	string courseCodeRegex = "^([A-Z]){2,4}(\\d{4})([LF]{0,1})";
	string prereqRegex = "^[\\[](" + courseCodeRegex.substr(1) + ")*([,]" +
	       			courseCodeRegex.substr(1) + ")*";
	if(info == 1) { //If this is a course code
		if(!regex_match(temp, regex(courseCodeRegex))) {
			cerr << "Whoops! It looks like one of your course codes is not formatted"
				" correctly, please fix this and try again." << endl;
			cerr << "Incorrect text: " << str << endl;
			cerr << "Course Codes require 2 to 4 capital letters followed by 4 digits"
				" plus potentially an L or F." << endl;
			return false;
		}
	} else if(info == 2) { //If this is a course name
		if(!regex_match(temp, regex("^[/&\\w]*"))) {
			cerr << "Whoops! It looks like one of your course names is not formatted"
				" correctly, please fix this and try again." << endl;
			cerr << "Incorrect text: " << str << endl;
			cerr << "Course names require text containing only letters and numbers." << endl;
			return false;
		}
	} else if(info == 3) { //If this is the credits
		if(!regex_match(temp, regex("^[1-9]\\d*"))) {
			cerr << "Whoops! It looks like one of your course credits is not formatted"
				" correctly, please fix this and try again." << endl;
			cerr << "Incorrect text: " << str << endl;
			cerr << "Course credits must be an integer greater than or equal to 0." << endl;
			return false;
		}
	} else if(info == 4) { //If this is the prerequisite list
		if(!regex_match(temp, regex(prereqRegex))) {
			cerr << "Whoops! It looks like one of your prerequisites lists is not formatted"
				" correctly, please fix this and try again." << endl;
			cerr << "Incorrect text: " << str << "]" << endl;
			cerr << "Prerequisites must be contained in brackets [] and given"
				" as a comma separated list of course codes." << endl;
			return false;
		}
	} else if(info == 5) { //If this is the list of offerings
		if(!regex_match(temp, regex("^[\\[]([0123])([,]([0123]))*"))) {
			cerr << "Whoops! It looks like one of your quarters offered lists is not"
				" formatted correctly, please fix this and try again." << endl;
			cerr << "Incorrect text: " << str << "]" << endl;
			cerr << "Course offerings must be contained in brackets [] and given"
				" as a comma separated list of quarter numbers." << endl;
			return false;
		}
	} else { //If for some reason we pass none of those things
		cerr << "Incorrect information type passed" << endl;
		return false;
	}
	return true;

}

/* Show Animation
 *
 * This function does a lot of work with the Ubuntu commandline by entering special commands within the C++ code here.
 * It takes all of the .dot files that the Display class creates and converts them all into pngs.
 * These pngs are then put into a ImageMagick converters which strings them into a gif to show the animation!
 * There is also code to let the user see the final product of both schedules, and save those as a separate jpg if they want to see it again
 *
 * @params: int for the number of quarters used in this schedule - this helps when knowing how many .dot files to convert
 * @returns: NA
 */
void ShowAnimation(int numQuarters) {
	cout << "Before we finish, we have two questions: " << endl;
	cout << "First - would you like to see a graphical visualization of what this course schedule would look like?" << endl;
	cout << "If so, 2 jpgs will be opened for you to view the schedule with and without constraints." << endl; 
	cout << "If you do this, please ensure you have the following packages installed:" << endl;
	cout << " - xdg-utils" << endl;
	cout << " - graphviz" << endl;
	cout << " - imagemagick" << endl;
	cout << "Please view the GitHub Readme for more information on how to do that." << endl;
	cout << "One final note: The visualization for the graph with no constraints takes on an automatic grouping based on how many classes are dependent on each other.";
	cout << " This means that it likely will not be in the same exact order as the text representation. That being said, this would still be a viable order";
	cout << " in which you could take these classes." << endl;
	cout << endl;
	cout << "View course graph? [Y/n] ";
	if(GetAnswer()) {
		cout << "Here you go!" << endl;
		cout << "Your images will be saved in this directory as \"mySchedule.jpg\" and \"noConstraintsSchedule.jpg\"" << endl;
		string myCommand = "dot -Tpng -o 0000.png quarter" + std::to_string(numQuarters - 1) + ".dot";
		system(myCommand.c_str()); //Converts the file containing the full constrained schedule to a png
		system("dot -Tpng -o temporary.png noConstraints.dot"); //Converts the one without constraints to png
		system("convert 0000.png mySchedule.jpg"); //Converts to jpg
		system("convert temporary.png noConstraintsSchedule.jpg"); //Converts to jpg
		system("xdg-open mySchedule.jpg 2> /dev/null"); //Opens the jpg for the user to see
		system("xdg-open noConstraintsSchedule.jpg 2> /dev/null"); //Same as above
		system("find -type f -iname '*.png' -delete"); //Deletes all pngs to ensure there is no overlap between runs of the program
	} else {
		cout << "That is alright!" << endl;
	}
	cout << endl << endl;
	cout << "Second - would you like to see an animated gif of your graph being created?" << endl;
	cout << "If so, a gif with that animation will be created and opened for you to view." << endl;
        cout << "Note that this will NOT animate the visualization without constraints, as that wouldn't be very interesting to see." << endl;	
	cout << "If you do this, please ensure you have the following packages installed:" << endl;
	cout << " - xdg-utils" << endl;
	cout << " - graphviz" << endl;
	cout << " - imagemagick" << endl;
	cout << "Once more, please view the GitHub Readme for more information on how to do this." << endl;
	cout << "View animation? [Y/n] ";
	if(GetAnswer()) {
		cout << "Good choice! Enjoy!" << endl;
		cout << "Your animation will be saved in this directory as \"scheduleAnimation.gif\"" << endl;
		for(int i = 0; i < numQuarters; i++) { //Loops through all .dot files created and makes each into a separate png
			string myCommandA;
			string myCommandB;
			if(i < 10) {
				myCommandA = "dot -Tpng -o 00" + std::to_string(i) + "0.png quarter" + std::to_string(i) + ".dot";
				myCommandB = "dot -Tpng -o 00" + std::to_string(i) + "5.png blank.dot";
			} else {
				myCommandA = "dot -Tpng -o 0" + std::to_string(i) + "0.png quarter" + std::to_string(i) + ".dot";
				myCommandB = "dot -Tpng -o 0" + std::to_string(i) + "5.png blank.dot";
			}
			myCommandA = myCommandA + " 2> /dev/null"; 
			myCommandB = myCommandB + " 2> /dev/null";
			system(myCommandA.c_str()); //Both of these comands convert them - the 2> /dev/null suppresses error messages that pop up despite everything working
			system(myCommandB.c_str());
		}
		system("convert -delay 240 -loop 0 *.png -scale 1080x1080 scheduleAnimation.gif"); //This combines all the pngs together into a gif
		std::this_thread::sleep_for(std::chrono::seconds(1)); //This gives the computer a second to do the above step
		system("xdg-open scheduleAnimation.gif 2> /dev/null"); //This opens the gif
		system("find -type f -iname '*.png' -delete"); //This deletes all of those pngs that we just made

	} else {
		cout << "Alright!" << endl;
	}
	system("find -type f -iname '*.dot' -delete"); //Deletes all the .dot to remove overlap issues between program runs

}

//This simplifies the times where we have to make sure the user answers "Y" or "N" and not something else
//Keeps asking the user for input until Y or N and returns bool respectively T/F
bool GetAnswer() {
	string decision = "";
    	cin >> decision;
    	while(decision.find_first_of("YyNn") != 0 && decision.length() != 1) {
		cout << "Whoops! It looks like you didn't answer [Y/n]. Please try again: ";
		cin >> decision;
    	}

    	if(decision.find_first_of("Yy") == 0) {
		cout << endl;
		return true;
    	} else {
		return false;
    	}
}
