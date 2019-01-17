/***
 * Name:            Aaron Liu, Larissa Johnson 
 * Date:            Nov 28 2016
 * Filename:        ActorNode.cpp
 * Description:     Individual node class for a single actor, which manages the
 *                  behavior of a nodes, such as finding edges in its adjacency
 *                  list.
 * Assigment Number: 
 * Sources of help: 
 ***/

#include <vector>
#include <climits>

#include "ActorNode.h"

using namespace std;

/* Funtion name: ActorNode
 * Description: constructor for ActorNode
 * Parameters: string name - name of the actor
 * Return value: none
 */
ActorNode::ActorNode(string name) : name(name), visited(0), prev(0), 
    dist(INT_MAX) {}


/* Funtion name: edgeFinder
 * Description: get the edge that is connecting the two nodes
 * Parameters: ActorNode * actor2 - the other actor this is connecting to
 * Return value: ActorEdge that is connecting the two actors
 */
ActorEdge * ActorNode::edgeFinder(ActorNode * actor2)
{
    ActorEdge * connect = 0;
    // use double loop to make connections to all cast members
    for (int i = 0; i < adj.size(); i++) 
    {
        for (int j = 0; j < actor2->adj.size(); j++)
        {
            if (adj[i] == actor2->adj[j]) 
            {   
                // weight the connections
                if(!connect || (connect->weight > adj[i]->weight))
                {
                    connect = adj[i];
                }
            }
        }
    }
    return connect;
}
