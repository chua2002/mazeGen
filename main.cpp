
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <iterator>
#include <new>
#include <system_error>
#include <vector>
#include <random>
#include <time.h>
#include "node.cpp"

using namespace std;


void printMaze(vector<vector<int>> *maze)
{

    // usleep(50000);
    // system("clear");
    
    for (int j=0; j < maze->at(0).size()+2; j++){ cout << "█";}
    cout << endl;


    for (int i=0; i < maze->size(); i++)
    {
        cout << "█";
        for (int j=0; j < maze->at(0).size(); j++)
        {
            if (maze->at(i).at(j) > 0) cout << "█";
            else cout << " ";
        }
        cout << "█";
        cout << endl;
    }

    for (int j=0; j < maze->at(0).size()+2; j++){ cout << "█";}
    cout << endl;
}

vector<vector<Node*>> createGraph(int sizex, int sizey)
{
    vector<vector<Node*>> vec;
    vector<Node*> tempvec;


    for (int i = 0; i < sizex; i++)
    {

        for (int j = 0; j < sizey; j++)
        {
            tempvec.push_back(new Node(i, j));
        }
        vec.push_back(tempvec);

        tempvec.clear();

    }

    for (auto i: vec) // set neighbor nodes
    {

        for (auto j : i)
        {
            if (sizex > j->cx+1) j->neighbors.push_back(vec.at(j->cx+1).at(j->cy));

            if (j->cx-1 >= 0) j->neighbors.push_back(vec.at(j->cx-1).at(j->cy));

            if (sizey > j->cy+1) j->neighbors.push_back(vec.at(j->cx).at(j->cy+1));

            if (j->cy-1 >= 0) j->neighbors.push_back(vec.at(j->cx).at(j->cy-1));
        }

    }



    return vec;
}

void addNeighbors( Node*current, vector<Node*> *Neighbors) // add all unconnected neighbors to neighbor list
{
    for (auto n: current->neighbors)
    {
        if (!n->connected && find(Neighbors->begin(), Neighbors->end(), n) == Neighbors->end()) // dont allow duplicates
        {
            Neighbors->push_back(n);
        }
    }

}

void setMazeConnection(Node* a, Node* b, vector<vector<int>> *maze)
{
    // translate coords into maze size
    int ax = a->cx *2;
    int ay = a->cy *2;
    int bx = b->cx *2;
    int by = b->cy *2;

    maze->at(ax).at(ay) = 0; // set a
    maze->at(bx).at(by) = 0; // set b
    maze->at((ax+bx)/2).at((ay+by)/2) = 0; // set a

}


void primMaze(vector<vector<Node*>> *graph, int startx, int starty, vector<vector<int>> *maze)
{


    vector<Node*> nearNeighbors;
    vector<Node*> path;
    Node* current = graph->at(startx).at(starty);
    path.push_back(current);
    current->connected = true;

    
    addNeighbors(current, &nearNeighbors);

    while(!nearNeighbors.empty())
    {
        int currentIndex = rand() % nearNeighbors.size();
        current = nearNeighbors.at(currentIndex); // pick random unvisited node nearby
        
        vector<Node*> connectedNeighbors;
        for (auto n: current->neighbors) // get connected neighbors
        {
            if (n->connected) connectedNeighbors.push_back(n);
        }
        Node* target = connectedNeighbors.at(rand() % connectedNeighbors.size()); // pick one at random

        //connect the two
        target->connections.push_back(current);
        current->connections.push_back(target);

        setMazeConnection(target, current, maze);
        // printMaze(maze);

        // set current as connected
        current->connected = true;

        // add current's neighbors to near neighbor set
        addNeighbors(current, &nearNeighbors);

        // remove current from near neighbors
        nearNeighbors.erase(nearNeighbors.begin()+currentIndex);
    }
    


}



int main(int argc, char* argv[])
{

    int graphx;
    int graphy;

    if (argc == 1)
    {
        graphx = 10;
        graphy = 10;
    }
    
    else if (argc == 2) {
        try
        {
            graphx = stoi(argv[1]);
            graphy = stoi(argv[1]);
        }
        catch(exception &err)
        {
            printf("Argument ( %s ) is not a valid argument\n", argv[1]);
            return 1;
        }

    }

    else if (argc == 3)
    {
        try
        {
            graphx = stoi(argv[1]);
        }
        catch(exception &err)
        {
            printf("Argument ( %s ) is not a valid argument\n", argv[1]);
            return 1;
        }

        try
        {
            graphy = stoi(argv[2]);
        }
        catch(exception &err)
        {
            printf("Argument ( %s ) is not a valid argument\n", argv[2]);
            return 1;
        }
    }

    else {
        printf("Invalid number of arguments\n");
        return 1;
    }

    srand(time(NULL));

    vector<vector<Node*>> graph = createGraph(graphx, graphy);


    vector<vector<int>> maze(graphx*2-1, vector<int> (graphy*2-1, 1)); // int array for maze display, 1 = wall, 0 = path


    primMaze(&graph, 0, 0, &maze);
    printMaze(&maze);


    return 0;
}
