#include "../includes.h"

/************************************************
 * bfs.h
 * ----------------------------------------------
 * Part B header file for breath first search.
 * Will go to all neighboring cities first
 * starting from closest to farthest
 ***********************************************/
#ifndef BFS_H
#define BFS_H

enum City { Seattle = 0, SanFrancisco, LosAngeles, Denver, Chicago, KansasCity, Dallas, Houston, Boston, NewYork, Atlanta, Miami, NumberOfCities };

struct CityNode
{
    City city;
    int  level;
    int  distanceFromLast;

    CityNode(City city) : city(city), level(0), distanceFromLast(0) {}
    CityNode(City city, int level, int distanceFromLast) : city(city), level(level), distanceFromLast(distanceFromLast) {}

};

struct CityComparator
{
    bool operator()(const CityNode& cityOne, const CityNode& cityTwo) const
    {
        if (cityOne.level == cityTwo.level)
        {
            return cityOne.distanceFromLast > cityTwo.distanceFromLast;
        }

        return cityOne.level > cityTwo.level;
    }
};

class AdjacencyMatrix
{
public:
    AdjacencyMatrix();
    ~AdjacencyMatrix();

    void PrintMatrix();

    void AddEdge(City originVertex, City destinationVertex, int distance);
    void RemoveEdge(City originVertex, City destinationVertex);

    void BFS(City originCity);
    
private:

    void PrintPath(int cityAIndex, int cityBIndex);
    void InitializeMatrix();

    vector<vector<int>> matrix; // Using int bc of 13 Graphs Part 1 - Slide 40
};

#endif