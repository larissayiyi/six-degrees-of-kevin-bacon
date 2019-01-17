/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        ActorNode.h
 * Description:     Header file for ActorNode object, which defines the
 *                  variables that form an actor's identity, as well as other
 *                  methods to determine connections with others
 * Assigment Number: 
 * Sources of help: 
 ***/
 
#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <vector>
#include <string>

#include "ActorEdge.h"


using namespace std;

class ActorNode {
private:

public:
    //the previous node in traversal
    ActorNode * prev;

    //The current actor's name
    string name;

    //Whether the node has been visited in traversal
    bool visited;

    //Adjacency list; all adjacent actors
    vector<ActorEdge*> adj;
    
    //Distance of node, if graph is weighted
    int dist;
    
    /* Funtion name: ActorNode
     * Description: constructor for ActorNode
     * Parameters: string name - name of the actor
     * Return value: none
     */
    ActorNode(string name);

    /* Funtion name: edgeFinder
     * Description: get the edge that is connecting the two nodes
     * Parameters: ActorNode * actor2 - the other actor this is connecting to
     * Return value: ActorEdge that is connecting the two actors
     */
    ActorEdge * edgeFinder(ActorNode * actor2);
};

#endif
