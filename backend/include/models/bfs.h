#include "../includes.h"
#include "../database/databaseManager.h"
#include "hash.h"

/************************************************
 * bfs.h
 * ----------------------------------------------
 * Part B header file for breath first search.
 * Will go to all neighboring cities first
 * starting from closest to farthest
 ***********************************************/
#ifndef BFS_H
#define BFS_H

struct DistanceNode
{
    Distance data;
    int  level;
    int  distanceFromLast;

    DistanceNode(Distance data) : data(data), level(0), distanceFromLast(0) {}
    DistanceNode(Distance data, int level, int distanceFromLast) : data(data), level(level), distanceFromLast(distanceFromLast) {}

};

struct DistanceComparator
{
    bool operator()(const DistanceNode& distanceOne, const DistanceNode& distanceTwo) const
    {
        if (distanceOne.level == distanceTwo.level)
        {
            return distanceOne.distanceFromLast > distanceTwo.distanceFromLast;
        }

        return distanceOne.level > distanceTwo.level;
    }
};

class AdjacencyMatrix
{
public:
    AdjacencyMatrix();
    ~AdjacencyMatrix();

    void printMatrix();

    void addEdge(City originVertex, City destinationVertex, int distance);
    void removeEdge(City originVertex, City destinationVertex);

    void recieveElements(vector<string> cities);

    void BFS(City originCity);
    
private:

    void printPath(int cityAIndex, int cityBIndex);
    void initializeMatrix(int vertexCount);
    void verticesToMatrix();

    vector<Distance>        distances;
    DoubleHashTable<string> vertices;
    vector<vector<int>>     matrix;
};

#endif