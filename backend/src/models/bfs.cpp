#include "bfs.h"

AdjacencyMatrix::AdjacencyMatrix()
{
    InitializeMatrix();
}


AdjacencyMatrix::~AdjacencyMatrix()
{

}


void AdjacencyMatrix::PrintMatrix()
{
    cout << "[AdjacencyMatrix::PrintMatrix()]" << endl;

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


void AdjacencyMatrix::AddEdge(City originVertex, City destinationVertex, int distance)
{
    matrix[originVertex][destinationVertex] = distance;
}


void AdjacencyMatrix::RemoveEdge(City originVertex, City destinationVertex)
{
    matrix[originVertex][destinationVertex] = 0;
}


void AdjacencyMatrix::BFS(City origin)
{
    cout << "[AdjacencyMatrix::BFS]" << endl;

    vector<bool> visited(NumberOfCities, false);
    vector<int> cityLevels(NumberOfCities, -1);

    // Min-heap: (distance, city)
    priority_queue<CityNode, vector<CityNode>, CityComparator> queue;
    int totalDistance = 0;
    int level = -1;

    queue.push(CityNode(origin));

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
                PrintPath(currentCity, neighbor);
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
                PrintPath(currentCity, neighbor);
            }
        }
    }

    cout << "Total distance: " << totalDistance << endl;
}

void AdjacencyMatrix::PrintPath(int cityA, int cityB)
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


void AdjacencyMatrix::InitializeMatrix()
{
    cout << "[AdjacencyMatrix::InitializeMatrix()]" << endl;

    // Initialize matrix to 0
    matrix.resize(NumberOfCities);
    for (int i = 0; i < NumberOfCities; ++i)
    {
        for (int j = 0; j < NumberOfCities; ++j)
        {
            matrix[i].push_back(0);
        }
    }

    AddEdge(Seattle, SanFrancisco, 807);
    AddEdge(Seattle, Denver, 1331);

    AddEdge(SanFrancisco, LosAngeles, 381);

    AddEdge(LosAngeles, Denver, 1015);

    AddEdge(Denver, Chicago, 1003);
    AddEdge(Denver, SanFrancisco, 1267);

    AddEdge(KansasCity, Chicago,    533);
    AddEdge(KansasCity, NewYork,    1260);
    AddEdge(KansasCity, Atlanta,    864);
    AddEdge(KansasCity, Denver,     599);
    AddEdge(KansasCity, LosAngeles, 1663);

    AddEdge(Dallas, Atlanta,    781);
    AddEdge(Dallas, KansasCity, 496);

    AddEdge(Houston, Atlanta, 810);
    AddEdge(Houston, Dallas,  239);

    AddEdge(Atlanta, Miami, 661);

    AddEdge(NewYork, Atlanta, 888);
    AddEdge(NewYork, Chicago, 787);

    AddEdge(Chicago, Boston,  983);
    AddEdge(Chicago, Seattle, 2097);

    AddEdge(Boston, NewYork, 214);

    AddEdge(Miami, Houston, 1187);
}