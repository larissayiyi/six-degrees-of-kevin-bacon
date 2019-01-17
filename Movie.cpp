/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        Movie.cpp
 * Description:     Movie class containing methods to organize the cast members
 *                  inside and manage edges between the cast
 * Assigment Number: 4
 * Sources of help:  
 ***/
 
#include <vector>
#include <iostream>

#include "Movie.h"
#include "ActorNode.h"
#include "ActorEdge.h"

using namespace std;

/* Funtion name: conect     
 * Description: connect all cast members from the movie 
 * Parameters: bool use_weighted - if the edges needs to be weighted
 * Return value: none
 */
void Movie::connect(bool use_weighted)
{ 
    for(int i = 0; i < cast.size() - 1; i++)
    {
        for(int j = i + 1; j < cast.size(); j++)
        {   
            //Create new edge object
            ActorEdge * newEdge = new ActorEdge(name, year); 

            //Add actors to edge
            newEdge->actor1 = cast[i];
            newEdge->actor2 = cast[j];
            
            //set weight if weighted edges are enabled
            if(use_weighted)
            {
                newEdge->weight = 1 + (2015 - year);   
            }

            //Add edge to adjacency list for both nodes
            cast[i]->adj.push_back(newEdge);
            cast[j]->adj.push_back(newEdge);
            edges.push_back(newEdge);
        }
    }
}

/* Funtion name: deleteEdges
 * Description: helper for the destructor to delete the edges made by this
 * Movie object
 * Parameters: none
 * Return value: none
 */
void Movie::deleteEdges()
{
    for (int i = 0; i < edges.size(); i++) 
    {
        delete edges[i];
    }
}
