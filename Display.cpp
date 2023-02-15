#include <fstream>
#include <iostream>
#include "Display.h"
using std::string;
using std::deque;
using std::cout;
using std::endl;


/* Constructor*/
Display::Display() = default;


/* Destructor*/
Display::~Display()=default;


/* Create Graph
 *
 * This function creates the individual dot files for each quarter's cumulative course flow.
 * It will send add quarter's selected courses to the current graph and produce a new picture.
 * If it is summer it creates an "invisible" summer class to hold ranking on the graph.
 * If no classes are taken over the summer quarter the graph will not change.
 *
 * @param: deque of the entire course flow and the vector containing all the course objects
 * @return: NA
 */
void Display::CreateGraph(deque<deque<Course>> mySchedule, vector<Course> catalog) {
    for(int i = 0; i < mySchedule.size(); i++) {
        string fileName = "quarter#.dot";
        fileName.replace(7,1,std::to_string(i));
        if (mySchedule[i].size()> 0){
            AddEdges(mySchedule[i], catalog);
            UpdateGraph(fileName);
        }else{
            ToSummerQ(i);
            UpdateGraph(fileName);
        }
    }
    Blank();
}


/* Update Graph
 *
 * This function outputs the nodes, edges and ranking of the created graph to its specified .dot file.
 *
 * @param: a string containing the specified file name to write the graph to
 * @return: NA
 */
void Display::UpdateGraph(const string& file_name) {
    std::ofstream file;
    file.open(file_name);
    output = "";
    output += "digraph g{\n\trankdir=LR;\n\tnode [style=filled];\n";
    output += nodes;
    output += edges;
    output += ranks;
    output += "}\n";
    file << output;
    file.close();
}


/* Add Edges
 *
 * This function adds edges from already taken classes to the new classes being added for the quarter.
 * These edges demonstrate which classes are prerequisites for the newly added classes.
 *
 * @param: deque of the quarter's classes and the vector containing all the course objects
 * @return: NA
 */
void Display::AddEdges(const deque<Course>& mySchedule, vector<Course>& catalog) {
    ranks += "\t{rank=same; ";
    for (Course curr: mySchedule) {     //get classes for quarter
        if (summer > 0)FromSummerQ(curr);
        taken.push_back(curr);
        string currName = curr.GetCourseCode();
        currName.erase(remove(currName.begin(),currName.end(),' '),currName.end());
        string color = GetColor(currName);
        nodes += "\t" + currName + "[color=" + color + "];\n";
        last = curr;
        if (constraints)ranks += currName + "; ";
        for (string s: curr._prereqs) {     //for each prereq
            for (Course t : taken){     //if taken
                string tName = t.GetCourseCode();
                tName.erase(remove(tName.begin(),tName.end(),' '),tName.end());
                if (tName == s){        //then add edge
                    string currName = curr.GetCourseCode();
                    currName.erase(remove(currName.begin(),currName.end(),' '),currName.end());
                    edges += "\t" + tName + "->" + currName + ";\n";
                }
            }
        }
    }
    ranks += "}\n";
}


/* Get Color
 *
 * This function gets the color corresponding to the class type.
 *
 * @param: a string containing the class name
 * @return: NA
 */
string Display::GetColor(string name) {
    if (name.substr(0,3) == "CSC"){
        return "cyan3";
    }else if (name.substr(0,3) == "PHY"){
        return "darkviolet";
    }else if (name.substr(0,3) == "MAT"){
        return "royalblue";
    }else if (name.substr(0,3) == "BIO"){
        return "darkgreen";
    }else if (name.substr(0,3) == "HHP"){
        return "tomato3";
    }else if (name.substr(0,3) == "CHM"){
        return "limegreen";
    }else if (name.substr(0,3) == "FCS"){
        return "darksalmon";
    }else if (name.substr(0,2) == "EE"){
        return "darkgoldenrod1";
    }
    return "gray";
}


/* From Summer Quarter
 *
 * This function creates an invisible edge.
 * This is between the invisible summer quarter class to a class being taken the next fall quarter.
 * An invisible edge is required to keep the ranking consistent for the course flow.
 *
 * @param: a Course object of a class being taken the next fall quarter
 * @return: NA
 */
void Display::FromSummerQ(Course c) {
    string name = "s" + std::to_string(summer);
    string cName = c.GetCourseCode();
    cName.erase(remove(cName.begin(),cName.end(),' '),cName.end());
    edges += "\t" + name + "->" + cName + "[style=invis];\n";
    summer = 0;
}


/* To Summer Quarter
 *
 * This function creates an invisible edge.
 * This is between a class taken the previous spring quarter to the invisible summer class.
 * An invisible edge is required to keep the ranking consistent for the course flow.
 *
 * @param: an integer containing the current quarter
 * @return: NA
 */
void Display::ToSummerQ(int quarter){
    if (quarter > 0){
        string name = "s" + std::to_string(quarter);
        string lName = last.GetCourseCode();
        lName.erase(remove(lName.begin(),lName.end(),' '),lName.end());
        nodes += "\t" + name + "[color=white, label=\"\"];\n";
        edges += "\t" + lName + "->" + name + "[style=invis];\n";
        summer = quarter;
    }else{
        summer = 0;
    }
}

/* Blank
 *
 * This function creates a blank png used in the GIF creation.
 *
 * @param: NA
 * @return: NA
 */
void Display::Blank(){
    std::ofstream file;
    file.open("blank.dot");
    output = "digraph g{\n}";
    file << output;
    file.close();
}
