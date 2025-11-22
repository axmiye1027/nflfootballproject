#include "models/bfs.h"

AdjacencyMatrix::AdjacencyMatrix()
{

}


AdjacencyMatrix::~AdjacencyMatrix()
{

}


void AdjacencyMatrix::printMatrix()
{
    cout << "[AdjacencyMatrix::printMatrix()]" << endl;

    string output;

    cout << setw(7) << " ";
    for (int i = 0; i < NumberOfCities; ++i)
    {
        cout << setw(7) << left << i << " ";
    }

    cout << endl;

    for (int i = 0; i < NumberOfCities; ++i)
    {
        cout << setw(7) << left << i;
        for (int j = 0; j < NumberOfCities; ++j)
        {
            output = "[" + to_string(matrix[i][j]) + "]";

            cout << setw(7) << left << output << " ";
        }

        cout << endl << endl;
    }
}


void AdjacencyMatrix::addEdge(City originVertex, City destinationVertex, int distance)
{
    // Indirected Graph -- Symmetrical Matrix
    matrix[originVertex][destinationVertex] = distance;
    matrix[destinationVertex][originVertex] = distance;
}


void AdjacencyMatrix::removeEdge(City originVertex, City destinationVertex)
{
    // Indirected Graph -- Symmetrical Matrix
    matrix[originVertex][destinationVertex] = 0;
    matrix[destinationVertex][originVertex] = 0;
}


void AdjacencyMatrix::recieveElements(vector<string> cities)
{
    for (int i = 0; i < cities.size(); ++i)
    {
        vertices.insert(cities[i], i);
    }
}


void AdjacencyMatrix::BFS(City origin)
{
    cout << "[AdjacencyMatrix::BFS]" << endl;

    vector<bool> visited(NumberOfCities, false);
    vector<int> cityLevels(NumberOfCities, -1);

    priority_queue<DistanceNode, vector<DistanceNode>, DistanceComparator> queue;
    int totalDistance = 0;
    int level = -1;

    queue.push(DistanceNode(origin));

    while (!queue.empty())
    {
        ++level;

        int currentCity = queue.top().city;
        queue.pop();

        if (visited[currentCity])
        {
            continue;
        }
        visited[currentCity] = true;

        for (int neighbor = 0; neighbor < NumberOfCities; ++neighbor)
        {
            int weight = matrix[currentCity][neighbor];
            if (weight == 0) 
            {
                continue; // no edge
            }

            if (!visited[neighbor])
            {
                queue.push(CityNode(City(neighbor), level, weight));
                cityLevels[neighbor] = level;

                cout << "Discovery Edge: ";
                printPath(currentCity, neighbor);
                totalDistance += weight;
            }
            else
            {
                // Already visited — classify the edge type
                if (cityLevels[neighbor] > cityLevels[currentCity])
                {
                    cout << "Forward Edge: ";
                }
                else if (cityLevels[neighbor] < cityLevels[currentCity])
                {
                    cout << "Back Edge: ";
                }
                else if (cityLevels[neighbor] == cityLevels[currentCity])
                {
                   cout << "Cross Edge: ";
                }
                else
                {
                    cout << "Unknown Edge Type: ";
                }
                printPath(currentCity, neighbor);
            }
        }
    }

    cout << "Total distance: " << totalDistance << endl;
}

void AdjacencyMatrix::printPath(int cityA, int cityB)
{
    string output = "";

    switch(cityA)
    {
        case Seattle:       output += "Seattle";
            break;
        case SanFrancisco:  output += "San Francisco";
            break;
        case LosAngeles:    output += "Los Angeles";
            break;
        case Denver:        output += "Denver";
            break;
        case Chicago:       output += "Chicago";
            break;
        case KansasCity:    output += "Kansas City";
            break;
        case Dallas:        output += "Dallas";
            break;
        case Houston:       output += "Houston";
            break;
        case Boston:        output += "Boston";
            break;
        case NewYork:       output += "New York";
            break;
        case Atlanta:       output += "Atlanta";
            break;
        case Miami:         output += "Miami";
            break;
    }

    output += " -> ";

    switch(cityB)
    {
        case Seattle:       output += "Seattle";
            break;
        case SanFrancisco:  output += "San Francisco";
            break;
        case LosAngeles:    output += "Los Angeles";
            break;
        case Denver:        output += "Denver";
            break;
        case Chicago:       output += "Chicago";
            break;
        case KansasCity:    output += "Kansas City";
            break;
        case Dallas:        output += "Dallas";
            break;
        case Houston:       output += "Houston";
            break;
        case Boston:        output += "Boston";
            break;
        case NewYork:       output += "New York";
            break;
        case Atlanta:       output += "Atlanta";
            break;
        case Miami:         output += "Miami";
            break;
    }

    cout << output << endl;
}


void AdjacencyMatrix::initializeMatrix(int vertexCount)
{
    cout << "[AdjacencyMatrix::initializeMatrix()]" << endl;

    // Initialize matrix to 0
    matrix.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i)
    {
        for (int j = 0; j < vertexCount; ++j)
        {
            matrix[i].push_back(0);
        }
    }
}

void AdjacencyMatrix::verticesToMatrix()
{
    int vertexCount = vertices.getSize();

    initializeMatrix(vertexCount);

    for (int i = 0; i < distances.size(); ++i)
    {
        int locationA = vertices[distances[i].locationA];
        int locationB = vertices[distances[i].locationB];

        addEdge(locationA, locationB, distances[i].distanceKm);
    }
}