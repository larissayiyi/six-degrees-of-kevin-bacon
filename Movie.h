/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        Movie.h
 * Description:     Header file for movie class, which contains internal
 *                  variables such as name, as well as prototype methods to use
 *                  in actual movie class
 * Assigment Number: 4 
 * Sources of help: 
 ***/

#ifndef MOVIE_H
#define MOVIE_H

#include <vector>
#include <unordered_set>
#include <string>

#include "ActorNode.h"
#include "ActorEdge.h"


using namespace std;

class Movie {
private:

public:
    //Movie name stored in string
    string name;

    //year that movie was released
    int year;
    
    // store all the edges created by this movie
    vector<ActorEdge *> edges;
   
    // cast member for the movie
    vector<ActorNode *> cast;
    
    /* Funtion name: Movie
     * Description: constructor for the Movie object
     * Parameters:
     *   string name - name of the movie
     *   int year - year of the movie
     * Return value: none
     */
    Movie(string name, int year): name(name), year(year){};

    /* Funtion name: conect
     * Description: connect all cast members from the movie 
     * Parameters: bool use_weighted - if the edges needs to be weighted
     * Return value: none
     */
    void connect(bool use_weighted);    
    
    /* Funtion name: deleteEdges
     * Description: helper for the destructor to delete the edges made by this
     * Movie object
     * Parameters: none
     * Return value: none
     */
    void deleteEdges();
};


#endif
