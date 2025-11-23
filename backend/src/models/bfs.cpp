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
    for (int i = 0; i < matrix.size(); ++i)
    {
        cout << setw(7) << left << i << " ";
    }

    cout << endl;

    for (int i = 0; i < matrix.size(); ++i)
    {
        cout << setw(7) << left << i;
        for (int j = 0; j < matrix.size(); ++j)
        {
            output = "[" + to_string(matrix[i][j]) + "]";

            cout << setw(7) << left << output << " ";
        }

        cout << endl << endl;
    }
}


// void AdjacencyMatrix::addEdge(int locationA, int locationB, int distance)
// {
//     // Indirected Graph -- Symmetrical Matrix
//     matrix[locationA][locationB] = distance;
//     matrix[locationB][locationA] = distance;
// }

void AdjacencyMatrix::addEdge(Distance distance)
{
    addEdge(distance.locationA, distance.locationB, distance.distanceKm);
}

void AdjacencyMatrix::addEdge(string locationA, string locationB, int distance)
{
    int originVertex      = vertices[locationA];
    int destinationVertex = vertices[locationB];

    // Indirected Graph -- Symmetrical Matrix
    matrix[originVertex][destinationVertex] = distance;
    matrix[destinationVertex][originVertex] = distance;
}


void AdjacencyMatrix::removeEdge(string locationA, string locationB)
{
    int originVertex      = vertices[locationA];
    int destinationVertex = vertices[locationB];

    // Indirected Graph -- Symmetrical Matrix
    matrix[originVertex][destinationVertex] = 0;
    matrix[destinationVertex][originVertex] = 0;
}


void AdjacencyMatrix::setDistances(vector<Distance> distances)
{
    this->distances = distances;
}


void AdjacencyMatrix::populateVertices(DoubleHashTable<Stadium> stadiums)
{
    for (int i = 0; i < stadiums.getCount(); ++i)
    {
        vertices.insert(stadiums.get(i).getStadiumName(), stadiums.get(i).getStadiumId());
    }

    initializeMatrix(vertices.getCount());
    verticesToMatrix();
}


void AdjacencyMatrix::BFS(string origin)
{
    cout << "[AdjacencyMatrix::BFS]" << endl;

    size_t matrixSize = matrix.size();

    vector<bool> visited(matrixSize, false);
    vector<int> cityLevels(matrixSize, -1);

    priority_queue<DistanceNode, vector<DistanceNode>, DistanceComparator> queue;
    int totalDistance = 0;
    int level = -1;

    queue.push(DistanceNode(origin));

    while (!queue.empty())
    {
        ++level;

        int currentCity = vertices[queue.top().city];
        queue.pop();

        if (visited[currentCity])
        {
            continue;
        }
        visited[currentCity] = true;

        for (int neighbor = 0; neighbor < matrixSize; ++neighbor)
        {
            int weight = matrix[currentCity][neighbor];
            if (weight == 0) 
            {
                continue; // no edge
            }

            if (!visited[neighbor])
            {
                queue.push(DistanceNode(vertices.get(neighbor), level, weight));
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

    output += vertices.get(cityA);

    output += " -> ";

    output += vertices.get(cityB);

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
    int vertexCount = vertices.getCount();

    for (int i = 0; i < distances.size(); ++i)
    {
        addEdge(distances[i]);
    }
}