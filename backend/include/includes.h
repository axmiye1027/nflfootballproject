#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>    // sort
#include <iomanip>      // setw, left
#include <unordered_map>
#include <set>
#include <deque>
#include <limits>
using namespace std;

struct AdjacencyNode
{
    string cityName;
    int    distanceFromLast;

    AdjacencyNode() : cityName{""}, distanceFromLast{0} {}
    AdjacencyNode(const string& name, int dist) : cityName{name}, distanceFromLast{dist} {}
};

struct PathReturn 
{
    vector<string> path;
    int            distanceTraveled;

    PathReturn() : distanceTraveled{0} {}
    PathReturn(vector<string> path, int distanceTraveled) : path(path), distanceTraveled(distanceTraveled) {}
};