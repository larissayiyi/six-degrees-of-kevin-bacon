/***
 * Name:            Aaron Liu, Larissa Johnson   
 * Date:            Nov 28 2016
 * Filename:        ActorGraph.h
 * Description:     ActorGraph represents ActorNode relations connected by
 *                  ActorEdges. The graph is used to find the shortest 
 *                  connection between two actors in within the ActorGraph
 * Assigment Number: 4
 * Sources of help: 
 ***/
 

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <unordered_map>
#include <iostream>
#include <string>

#include "ActorNode.h"
#include "ActorEdge.h"
#include "Movie.h"

// Maybe include some data structures here

using namespace std;

class ActorGraph {
protected:

public:
    unordered_map <string, ActorNode *> actorList; 
    unordered_map <string, Movie *> movieList;
    
    /* Funtion name: ActorGraph
     * Description: constructor for ActorGraph
     * Parameters: none
     * Return value: none
     */
    ActorGraph(void);
    
    /* Funtion name: build
     * Description: calls connect for each movie in the movieList to make all
     * edges between actors
     * Parameters:
     *   bool use_weighted - whether edges should be weighted or not
     * Return value: True if graph is built sucessfully false otherwise
     */
    bool build(bool use_weighted);
  
    /* Funtion name: loadFromFile
     * Description: Load the graph from a tab-delimited file of actor->movie
     * relationships.
     * Parameters:
     *   const char * in_filename - input filename
     *   bool use_weighted_edges - if true, compute edge weights as
     *     1 + (2015 - movie_year), otherwise all edge weights will be 1
     * Return value: return true if file was loaded sucessfully, 
     * false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
   
    /* Funtion name: clearFields
     * Description: reset prev, dist, and visted fields in nodes for the next
     * call
     * Parameters: none
     * Return value: none
     */
    void clearFields();
    
    /* Funtion name: ~ActorGraph
     * Description: Destructor for ActorGraph, deletes pointers in 
     * unordered_maps
     * Parameters: none
     * Return value: none
     */
    ~ActorGraph();
};


#endif // ACTORGRAPH_H
