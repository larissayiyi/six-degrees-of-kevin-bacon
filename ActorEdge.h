/***
 * Name:            Aaron Liu, Larissa Johnson   
 * Date:            Nov 28 2016
 * Filename:        ActorEdge.h
 * Description:     ActorEdge represents connections between two actors by a 
 *                  movie. Same actors can be connected by multiple ActorEdge 
 *                  objects.
 * Assigment Number: 4
 * Sources of help: 
 ***/

#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include <vector>
#include <string>

#include "ActorNode.h"

using namespace std;

class ActorNode;

class ActorEdge {
private:

public:
    //Movie name stored in string
    string name;

    //year that movie was released
    int year;
    
    //Calculated weight of node, unused for checkpoint
    int weight;
    
    //Two actors on either side of the edge
    ActorNode * actor1;
    ActorNode * actor2;

    /* Funtion name: ActorEdge
     * Description: constructor for edges representing movies connecting 
     * actors
     * Parameters: 
     *   string name - movie names
     *   int year - year of the movie  
     * Return value: none
     */
    ActorEdge(string name, int year);
    
    /*
     * Function Name: traverse
     * Description: traverses across edge, returns actor on other side 
     * of edge
     * Parameters: 
     *      input: current node that needs to traverse edge
     * Return Value: Node on other side of the edge
     */
    ActorNode * traverse(ActorNode * input);
    
    
};


#endif
