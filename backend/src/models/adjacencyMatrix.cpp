#include "models/adjacencyMatrix.h"

AdjacencyMatrix::AdjacencyMatrix()
{

}


AdjacencyMatrix::~AdjacencyMatrix()
{

}


void AdjacencyMatrix::printMatrix()
{
    cout << "[AdjacencyMatrix::printMatrix()]" << endl;

    const int SPACING_SIZE = 6;

    string output;

    cout << setw(SPACING_SIZE) << " ";
    for (int i = 1; i < matrix.size(); ++i)
    {
        cout << setw(SPACING_SIZE) << left << i << " ";
    }

    cout << endl;

    for (int i = 1; i < matrix.size(); ++i)
    {
        cout << setw(SPACING_SIZE) << left << i;
        for (int j = 1; j < matrix.size(); ++j)
        {
            output = "[" + to_string(matrix[i][j]) + "]";

            cout << setw(SPACING_SIZE) << left << output << " ";
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
    set<string> uniqueStadiums;
    
    for (const auto& distance : distances)
    {
        uniqueStadiums.insert(distance.locationA);
        uniqueStadiums.insert(distance.locationB);
    }
    
    cout << "Found " << uniqueStadiums.size() << " unique stadiums in distances" << endl;

    int index = 0;
    for (const auto& stadiumName : uniqueStadiums)
    {
        cout << "insert vertices: " << stadiumName << endl;
        vertices.insert(stadiumName, index++);
    }

    initializeMatrix(vertices.getCount());
    verticesToMatrix();
}

int AdjacencyMatrix::bfs(string origin)
{
    cout << "[AdjacencyMatrix::BFS]" << endl;

    size_t matrixSize = matrix.size();

    vector<bool> visited(matrixSize, false);
    
    priority_queue<DistanceNode, vector<DistanceNode>, DistanceComparator> queue;
    int totalDistance = 0;

    int startCity = vertices[origin];
    queue.push(DistanceNode(origin, 0, 0));
    visited[startCity] = true;

    while (!queue.empty())
    {
        DistanceNode current = queue.top();
        queue.pop();
        
        int currentCity = vertices[current.city];
        
        vector<pair<int, int>> neighbors;
        
        for (int neighbor = 0; neighbor < matrixSize; ++neighbor)
        {
            int weight = matrix[currentCity][neighbor];
            if (weight == 0 || visited[neighbor]) 
            {
                continue;
            }
            
            neighbors.push_back({neighbor, weight});
        }
        
        sort(neighbors.begin(), neighbors.end(), 
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second < b.second;
             });
        
        for (const auto& [neighbor, weight] : neighbors)
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push(DistanceNode(vertices.get(neighbor), 0, weight));
                
                cout << "Discovery Edge: ";
                printPath(currentCity, neighbor);
                totalDistance += weight;
            }
        }
    }

    cout << "Total distance: " << totalDistance << endl;
    return totalDistance;
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
    matrix.resize(vertexCount + 1);
    for (int i = 0; i <= vertexCount; ++i)
    {
        for (int j = 0; j <= vertexCount; ++j)
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

vector<PathReturn> AdjacencyMatrix::dijkstra(const string& startVertex)
{
    cout << "[AdjacencyMatrix::Dijkstra]" << endl;

    const int NUMBER_OF_CITIES = vertices.getCount();
    const int INF = 999999;

    vector<PathReturn> paths(NUMBER_OF_CITIES); // records all the paths found and the distance
    vector<int>        distances(NUMBER_OF_CITIES, INF);
    vector<int>        previous(NUMBER_OF_CITIES, -1); 
    vector<bool>       cityVisited(NUMBER_OF_CITIES, false);
    
    int originVertex = vertices[startVertex];

    if(originVertex == -1 || originVertex >= NUMBER_OF_CITIES)
    {
        cout << "Error: Starting vertex '" << startVertex << "' not found!" << endl;
        return paths;
    }

    // for(int i = 0; i < NUMBER_OF_CITIES; ++i)
    // {
    //     distances[i] = INF;
    //     previous[i] = -1; 
    // }
    
    distances[originVertex] = 0; // Distance to origin is 0
    
    for(int count = 0; count < NUMBER_OF_CITIES; ++count)
    {
        // Find unvisited city with minimum distance
        int minDistance = INF;
        int currentCity = -1;
        
        for(int j = 0; j < NUMBER_OF_CITIES; ++j)
        {
            if(!cityVisited[j] && distances[j] < minDistance)
            {
                minDistance = distances[j];
                currentCity = j;
            }
        }
        
        // If no reachable city found
        if(currentCity == -1)
        {
            break;
        }
        
        // Mark current city as visited
        cityVisited[currentCity] = true;
        
        // Update distances to neighbors
        for(int n = 0; n < NUMBER_OF_CITIES; ++n)
        {
            // Check if there's an edge and city is unvisited
            if(!cityVisited[n] && matrix[currentCity][n] != 0 && matrix[currentCity][n] != INF)
            {
                int newDistance = distances[currentCity] + matrix[currentCity][n];
                
                // If we found a shorter path
                if(newDistance < distances[n])
                {
                    distances[n] = newDistance;
                    previous[n]  = currentCity;
                }
            }
        }
    }
    
    string output; // Makes the output more pretty)))

    cout << endl << "Shortest distances from city " << originVertex << ": " << endl;
    for(int i = 0; i < NUMBER_OF_CITIES; ++i)
    {
        output = "To city " + to_string(i);
        cout << left << setw(10) << output << ": ";

        if(distances[i] == INF)
        {
            cout << "No path" << endl;
        }
        else
        {
            paths[i].distanceTraveled = distances[i]; // add the path distance

            cout << left << setw(5) << distances[i] << " | Path: ";
            printPathExt(previous, originVertex, i);
            cout << endl;

            int iterator = i;

            paths[i].path.push_back(vertices.get(iterator));

            while(previous[iterator] != -1)
            {
                paths[i].path.push_back(vertices.get(previous[iterator]));

                iterator = previous[iterator];
            }

            reverse(paths[i].path.begin(), paths[i].path.end());
        }
    }
  
    return paths;
}

// MST using Prim-Jarnik's method
int AdjacencyMatrix::mst(const string& startVertex)
{
    cout << "\n\n[AdjacencyMatrix::MST]" << endl;
    
    const int NUMBER_OF_CITIES = vertices.getCount();
    const int INF = 999999;

    vector<int>  key(NUMBER_OF_CITIES, INF);         
    vector<int>  parent(NUMBER_OF_CITIES, -1);      
    vector<bool> visited(NUMBER_OF_CITIES, false);    

    int originVertex = vertices[startVertex];

    if(originVertex == -1 || originVertex >= NUMBER_OF_CITIES)
    {
        cout << "Error: Starting vertex '" << startVertex << "' not found!" << endl;
        return 1;
    }

    // Initialize all arrays
    // for (int i = 0; i < NUMBER_OF_CITIES; i++)
    // {
    //     key[i]     = INF;
    //     parent[i]  = -1;
    //     visited[i] = false;
    // }

    // Start MST from the chosen origin city
    key[originVertex] = 0;

    // Build MST (NUMBER_OF_CITIES - 1 edges)
    for (int count = 0; count < NUMBER_OF_CITIES - 1; count++)
    {
        // Pick unvisited city with minimum key value
        int minKey = INF;
        int u      = -1;

        for (int v = 0; v < NUMBER_OF_CITIES; v++)
        {
            if (!visited[v] && key[v] < minKey)
            {
                minKey = key[v];
                u      = v;
            }
        }

        // Check if graph is disconnected
        if (u == -1)
        {
            cout << "\nWarning: Graph is disconnected. MST is incomplete.\n";
            break;
        }

        // Add chosen city to MST
        visited[u] = true;

        // Update neighbors
        for (int v = 0; v < NUMBER_OF_CITIES; v++)
        {
            int weight = matrix[u][v];

            if (weight != 0 && !visited[v] && weight < key[v])
            {
                key[v]    = weight;
                parent[v] = u;
            }
        }
    }

    // Print MST
    cout << "\nMinimum Spanning Tree starting at ";
    printCityName(originVertex);
    cout << ":\n\n";

    int totalWeight = 0;

    for (int i = 0; i < NUMBER_OF_CITIES; i++)
    {
        if (parent[i] != -1)
        {
            printCityName(parent[i]);
            cout << "  ->  ";
            printCityName(i);
            cout << "   (";
            cout << matrix[i][parent[i]] << " miles)\n";

            totalWeight += matrix[i][parent[i]];
        }
    }

    cout << "\nTotal MST Mileage: " << totalWeight << " miles\n";
    return totalWeight;

}

void AdjacencyMatrix::printPathExt(vector<int> previous, int cityA, int cityB)
{
    printCityName(cityA);
    
    if(cityA != cityB)
    {
        printPathExtRecursion(previous, cityB, cityA);
    }
}

void AdjacencyMatrix::printPathExtRecursion(vector<int> previous, int currentCity, int originCity)
{
    if(currentCity == originCity)
    {
        return;
    }

    if(previous[currentCity] == -1)
    {
        return;
    }
    
    printPathExtRecursion(previous, previous[currentCity], originCity);
    
    cout << " -> ";
    printCityName(currentCity);
}

void AdjacencyMatrix::printCityName(int cityIndex)
{
    cout << vertices.get(cityIndex);

    // switch(cityIndex)
    // {
    //     case Seattle:       cout << "Seattle"; break;
    //     case SanFrancisco:  cout << "San Francisco"; break;
    //     case LosAngeles:    cout << "Los Angeles"; break;
    //     case Denver:        cout << "Denver"; break;
    //     case Chicago:       cout << "Chicago"; break;
    //     case KansasCity:    cout << "Kansas City"; break;
    //     case Dallas:        cout << "Dallas"; break;
    //     case Houston:       cout << "Houston"; break;
    //     case Boston:        cout << "Boston"; break;
    //     case NewYork:       cout << "New York"; break;
    //     case Atlanta:       cout << "Atlanta"; break;
    //     case Miami:         cout << "Miami"; break;
    // }
}

vector<Distance> AdjacencyMatrix::getDistanceVector()
{
    return distances;
}