#ifndef FLOWCHART_ALISON_LOGAN_DISPLAY_H
#define FLOWCHART_ALISON_LOGAN_DISPLAY_H
#include <boost/graph/adjacency_list.hpp>
#include "Course.h"
#include "ClassesSorter.h"

using std::string;
using std::deque;
using namespace boost;
using digraph = adjacency_list<listS, vecS, directedS, Vertex, Edge>;


class Display {
    string output;
    string edges;
    string nodes;
    string ranks;
    int summer;
    Course last;

public:
    vector <Course> taken;
    bool constraints = true;
    Display();
    ~Display();
    void CreateGraph(deque<deque<Course>> mySchedule,vector<Course> catalog);
    void UpdateGraph(const string& file);
    void AddEdges(const deque<Course>& mySchedule, vector<Course>& catalog);
    string GetColor(string name);
    void ToSummerQ(int quarter);
    void FromSummerQ(Course c);
    void Blank();
};


#endif //FLOWCHART_ALISON_LOGAN_DISPLAY_H
