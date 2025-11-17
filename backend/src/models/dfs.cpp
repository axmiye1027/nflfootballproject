#include "dfs.h"

AdjacencyList::AdjacencyList()
{
    InitializeList();
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

void AdjacencyList::AddEdge(edge& vertexEdge)
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

void AdjacencyList::RemoveEdge(string originVertex, string destinationVertex)
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

int  AdjacencyList::FindIndex(string vertex) const
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

void AdjacencyList::DFS(string originVertex) const
{
    cout << "[AdjacencyList::DFS]" << endl;

    vector<bool> visited(list.size(), false);
    set<pair<int, int>> printedEdges;

    int totalDistance;
    int startIndex;

    totalDistance = 0;
    startIndex    = 0;

    startIndex = FindIndex(originVertex);

    DFSRecusion(startIndex, startIndex, visited,printedEdges, totalDistance);

    cout << "Total distance traveled: " << totalDistance << endl;
}

void AdjacencyList::DFSRecusion(int vertexIndex, int lastIndex, vector<bool>& visited,set<pair<int, int>>& printedEdges, int& totalDistance) const
{
    edge* iterator;

    visited[vertexIndex] = true; 
    iterator = list[vertexIndex];
    
    // goes through each edge connected to the vertex
    while(iterator != nullptr)
    {
        int destIndex = FindIndex(iterator->destinationVertex);

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

            DFSRecusion(destIndex,vertexIndex ,visited,printedEdges, totalDistance);
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

void AdjacencyList::InitializeList()
{
    cout << "[AdjacencyList::InitializeList()]" << endl;

    edge seattleToChicago       ("Seattle", "Chicago",        2097);
    edge seattleToDenver        ("Seattle", "Denver",         1331);
    edge seattleToSanFrancisco  ("Seattle", "San Francisco",  807);

    edge sanFranciscoToSeattle   ("San Francisco", "Seattle",      807);
    edge sanFranciscoToDenver    ("San Francisco", "Denver",       1267);
    edge sanFranciscoToLosAngeles("San Francisco", "Los Angeles",  381);

    edge losAngelesToSanFrancisco("Los Angeles", "San Francisco",  381);
    edge losAngelesToDenver      ("Los Angeles", "Denver",         1015);
    edge losAngelesToKansasCity  ("Los Angeles", "Kansas City",    1663);
    edge losAngelesToDallas      ("Los Angeles", "Dallas",         1435);

    edge denverToSeattle     ("Denver", "Seattle",       1331);
    edge denverToSanFrancisco("Denver", "San Francisco", 1267);
    edge denverToLosAngeles  ("Denver", "Los Angeles",   1015);
    edge denverToKansasCity  ("Denver", "Kansas City",   599);
    edge denverToChicago     ("Denver", "Chicago",       1003);

    edge kansasCityToLosAngeles("Kansas City", "Los Angeles", 1663);
    edge kansasCityToDenver    ("Kansas City", "Denver",      599);
    edge kansasCityToChicago   ("Kansas City", "Chicago",     533);
    edge kansasCityToNewYork   ("Kansas City", "New York",    1260);
    edge kansasCityToAtlanta   ("Kansas City", "Atlanta",     864);
    edge kansasCityToDallas    ("Kansas City", "Dallas",      496);

    edge dallasToLosAngeles("Dallas", "Los Angeles", 1435);
    edge dallasToKansasCity("Dallas", "Kansas City", 496);
    edge dallasToAtlanta   ("Dallas", "Atlanta",     781);
    edge dallasToHouston   ("Dallas", "Houston",     239);

    edge houstonToDallas ("Houston", "Dallas", 239);
    edge houstonToAtlanta("Houston", "Atlanta", 810);
    edge houstonToMiami  ("Houston", "Miami",   1187);

    edge atlantaToKansasCity("Atlanta", "Kansas City", 864);
    edge atlantaToDallas    ("Atlanta", "Dallas",      781);
    edge atlantaToHouston   ("Atlanta", "Houston",     810);
    edge atlantaToMiami     ("Atlanta", "Miami",       661);
    edge atlantaToNewYork   ("Atlanta", "New York",    888);
 
    edge miamiToHouston("Miami", "Houston", 1187);
    edge miamiToAtlanta("Miami", "Atlanta", 661);

    edge newYorkToBoston    ("New York", "Boston",      214);
    edge newYorkToChicago   ("New York", "Chicago",     787);
    edge newYorkToKansasCity("New York", "Kansas City", 1260);
    edge newYorkToAtlanta   ("New York", "Atlanta",     888);

    edge bostonToNewYork("Boston", "New York", 214);
    edge bostonToChicago("Boston", "Chicago",  983);

    edge chicagoToSeattle   ("Chicago", "Seattle",     2097);
    edge chicagoToDenver    ("Chicago", "Denver",      1003);
    edge chicagoToKansasCity("Chicago", "Kansas City", 533);
    edge chicagoToNewYork   ("Chicago", "New York",    787);
    edge chicagoToBoston    ("Chicago", "Boston",      983);

    AddEdge(seattleToChicago);
    AddEdge(seattleToDenver);
    AddEdge(seattleToSanFrancisco);

    AddEdge(sanFranciscoToSeattle);
    AddEdge(sanFranciscoToDenver);
    AddEdge(sanFranciscoToLosAngeles);

    AddEdge(losAngelesToSanFrancisco);
    AddEdge(losAngelesToDenver);
    AddEdge(losAngelesToKansasCity);
    AddEdge(losAngelesToDallas);

    AddEdge(denverToSeattle);
    AddEdge(denverToSanFrancisco);
    AddEdge(denverToLosAngeles);
    AddEdge(denverToKansasCity);
    AddEdge(denverToChicago);

    AddEdge(kansasCityToLosAngeles);
    AddEdge(kansasCityToDenver);
    AddEdge(kansasCityToChicago);
    AddEdge(kansasCityToNewYork);
    AddEdge(kansasCityToAtlanta);
    AddEdge(kansasCityToDallas);

    AddEdge(dallasToLosAngeles);
    AddEdge(dallasToKansasCity);
    AddEdge(dallasToAtlanta);
    AddEdge(dallasToHouston);

    AddEdge(houstonToDallas);
    AddEdge(houstonToAtlanta);
    AddEdge(houstonToMiami);

    AddEdge(atlantaToKansasCity);
    AddEdge(atlantaToDallas);
    AddEdge(atlantaToHouston);
    AddEdge(atlantaToMiami);
    AddEdge(atlantaToNewYork);

    AddEdge(miamiToHouston);
    AddEdge(miamiToAtlanta);

    AddEdge(newYorkToBoston);
    AddEdge(newYorkToChicago);
    AddEdge(newYorkToKansasCity);
    AddEdge(newYorkToAtlanta);

    AddEdge(bostonToNewYork);
    AddEdge(bostonToChicago);

    AddEdge(chicagoToSeattle);
    AddEdge(chicagoToKansasCity);
    AddEdge(chicagoToNewYork);
    AddEdge(chicagoToBoston);
    AddEdge(chicagoToDenver);
}