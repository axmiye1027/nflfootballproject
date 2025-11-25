#include "models/adjacencyList.h"

AdjacencyList::AdjacencyList() : totalDist{0}
{

}

AdjacencyList::~AdjacencyList()
{
    edge* iterator;
    edge* temp;

    // goes through each index
    for(int i = 0; i < list.size(); ++i)
    {
        iterator = list[i];

        // goes through each edge
        while(iterator != nullptr)
        {
            temp     = iterator;
            iterator = iterator->next;

            delete temp;
        }
    }
}

void AdjacencyList::populateList(vector<edge>& edges) // edging
{
    for (int i = 0; i < edges.size(); ++i)
    {
        addEdge(edges[i]);
    }
}

void AdjacencyList::addEdge(edge& vertexEdge)
{
    edge* newEdge;
    edge* iterator;

    newEdge = new edge(vertexEdge);

    // check list for an index that stores the origin vertex
    for(int i = 0; i < list.size(); ++i)
    {
        if(vertexEdge.originVertex == list[i]->originVertex)
        {
            iterator = list[i];

            if(iterator->distance > newEdge->distance)
            {
                edge* head;
                head = list[i];

                newEdge->next = head;
                list[i]       = newEdge;
            }
            else
            {
                // finds position that has a bigger distance than new edge
                while(iterator->next != nullptr && iterator->next->distance < newEdge->distance)
                {
                    iterator = iterator->next;
                }
                newEdge->next = iterator->next;
                // add the edge to index
                iterator->next = newEdge;
            }

            return;
        }
    }

    // add new index to list and store edge if origin vertex doesn't exist
    list.push_back(newEdge);
}

void AdjacencyList::removeEdge(string originVertex, string destinationVertex)
{
    edge* iterator;
    edge* temp;

    // looks for originVertex
    for(int i = 0; i < list.size(); ++i)
    {
        if(originVertex == list[i]->originVertex)
        {
            iterator = list[i];

            // if first edge contains the destination
            if(iterator->destinationVertex == destinationVertex)
            {
                // remove the index from vector if only one edge
                if(iterator->next == nullptr)
                {
                    delete iterator;
                    list.erase(list.begin() + i);
                }
                else
                {
                    list[i] = list[i]->next;
                    delete iterator;
                }
            }
            else
            {
                // finds the edge before target edge
                while(iterator->next->destinationVertex != destinationVertex)
                {
                    iterator = iterator->next;

                    if(iterator == nullptr)
                    {
                        return;
                    }
                }
                
                temp = iterator->next;

                iterator->next = iterator->next->next;

                delete temp;
                return;
            }
        }
    }
}

int  AdjacencyList::findIndex(string vertex) const
{
    for(int s = 0; s < list.size(); ++s)
    {
        if(vertex == list[s]->originVertex)
        {
            return s;
        }
    }

    return -1;
}

void AdjacencyList::dfs(string originVertex)
{
    cout << "[AdjacencyList::DFS]" << endl;

    vector<bool> visited(list.size(), false);
    set<pair<int, int>> printedEdges;

    int totalDistance;
    int startIndex;

    totalDistance = 0;
    startIndex    = 0;

    startIndex = findIndex(originVertex);

    dfsRecusion(startIndex, startIndex, visited,printedEdges, totalDistance);

    totalDist = totalDistance;

    cout << "Total distance traveled: " << totalDistance << endl;
}

void AdjacencyList::dfsRecusion(int vertexIndex, int lastIndex, vector<bool>& visited,set<pair<int, int>>& printedEdges, int& totalDistance)
{
    edge* iterator;

    visited[vertexIndex] = true; 
    iterator = list[vertexIndex];
    
    // goes through each edge connected to the vertex
    while(iterator != nullptr)
    {
        int destIndex = findIndex(iterator->destinationVertex);

        // used to mark if a specific edge has been printed both ways
        pair<int, int> edgeKey = {vertexIndex, destIndex};
        pair<int, int> reverseEdgeKey = {destIndex, vertexIndex};

        // if edge was not visited before
        if(!visited[destIndex])
        {
            cout << "Discovery Edge: " << iterator->originVertex << " -> " << iterator->destinationVertex << endl;
            totalDistance += iterator->distance;
            printedEdges.insert(edgeKey);
            printedEdges.insert(reverseEdgeKey);

            path.push_back(AdjacencyNode(iterator->destinationVertex,iterator->distance));

            dfsRecusion(destIndex,vertexIndex ,visited,printedEdges, totalDistance);
        }
        else if(destIndex != lastIndex && printedEdges.count(edgeKey) == 0)
        {
            cout << "Back Edge: " << iterator->originVertex << " -> " << iterator->destinationVertex << endl;
            printedEdges.insert(edgeKey);
            printedEdges.insert(reverseEdgeKey);
        }

        iterator = iterator->next;
    }
}

vector<AdjacencyNode> AdjacencyList::returnPath() const
{
    return path;
}

int AdjacencyList::returnDist() const
{
    return totalDist;
}