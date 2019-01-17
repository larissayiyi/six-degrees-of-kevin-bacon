/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        ActorEdge.cpp
 * Description:     ActorEdge represents connections between two actors by a 
 *                  movie. Same actors can be connected by multiple ActorEdge 
 *                  objects. 
 * Assigment Number: 4
 * Sources of help:  
 ***/

#include <vector>

#include "ActorNode.h"
#include "ActorEdge.h"

/* Funtion name: ActorEdge
 * Description: constructor for edges representing movies connecting actors
 * Parameters: 
 *   string name - movie names
 *   int year - year of the movie  
 * Return value: none
 */
ActorEdge::ActorEdge(string name, int year) : name(name), year(year), 
    weight(1), actor1(0), actor2(0) {}

/*
 * Function Name: traverse
 * Description: traverses across edge, returns actor on other side of edge
 * Parameters: 
 *      input: current node that needs to traverse edge
 * Return Value: Node on other side of the edge
 */
ActorNode * ActorEdge::traverse(ActorNode * input)
{
    //If starting node is the same as actor1, return node on other side of 
    //edge
    if(input == actor1)
    {
        return actor2;
    }
    //Else do opposite
    else
    {
        return actor1;
    }
}
