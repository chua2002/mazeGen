#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <random>
#include <time.h>

using namespace std;


class Node
{
public:
    int cx;
    int cy;
    bool connected = false;
    

    vector<Node*> connections;
    vector<Node*> neighbors;

    Node(int x, int y);

};

Node::Node(int x,int y)
{
    cx = x;
    cy = y;
}