#include "../includes.h"
#include "../database/databaseManager.h"
#include "hash.h"
#include "stadium.h"

#ifndef BFS_H
#define BFS_H

struct DistanceNode
{
    string city;
    int    level;
    int    distanceFromLast;

    DistanceNode(string city) : city(city), level(0), distanceFromLast(0) {}
    DistanceNode(string city, int level, int distanceFromLast) : city(city), level(level), distanceFromLast(distanceFromLast) {}
};

struct PathNode
{
    vector<string> path;
    int            totalDist;

    PathNode() : totalDist{0} {}
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

    // void addEdge(int locationA, int locationB, int distance); // maybe useful?
    void addEdge(Distance distance); 
    void addEdge(string locationA, string locationB, int distance);
    void removeEdge(string locationA, string locationB);

    void setDistances(vector<Distance> distances);
    void populateVertices(DoubleHashTable<Stadium> stadiums);

    int bfs(string originCity);
    vector<PathNode> dijkstra(const string& startVertex);
    int mst(const string& startVertex);
    
    vector<Distance> getDistanceVector();

private:

    void printPath(int cityAIndex, int cityBIndex);
    void printPathExt(vector<int> previous, int cityAIndex, int cityBIndex);
    void printPathExtRecursion(vector<int> previous, int cityAIndex, int cityBIndex);

    void printCityName(int cityIndex);

    void initializeMatrix(int vertexCount);
    void verticesToMatrix();

    vector<Distance>        distances;
    DoubleHashTable<string> vertices;
    vector<vector<int>>     matrix;
};

#endif