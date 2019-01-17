/***
 * Name:            Aaron Liu, Larissa Johnson   
 * Date:            Nov 28 2016
 * Filename:        ActorGraph.cpp
 * Description:     ActorGraph represents ActorNode relations connected by
 *                  ActorEdges. The graph is used to find the shortest 
 *                  connection between two actors in within the ActorGraph 
 * Assigment Number: 4
 * Sources of help: 
 ***/
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <climits>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}


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
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges)
{
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it 
            // in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // pairing the name of the movie with the year it's made
        //pair <string, int> pairMovie(movie_title, movie_year);

        //Pair used to hold information about the actor

        ActorNode * workingActor;
        unordered_map<string, ActorNode *> :: iterator findActor = 
            actorList.find(actor_name);
        

        //Create new actor node if no node for actor exists yet
        if((findActor == actorList.end()) || actorList.empty())
        {
            //Create new actor object
            ActorNode * newActor = new ActorNode(actor_name);

            //Create pair with actorname as key to insert to hashmap
            pair <string, ActorNode *> actorInfo(actor_name, newActor);

            //Insert to hashmap
            actorList.insert(actorInfo);

            workingActor = newActor;
        }

        else
        {
            workingActor = findActor->second;
        }
        
        string originalTitle = movie_title;

        //Find movie in list, if it exists
        unordered_map<string, Movie *> :: iterator findMovie = 
            movieList.find(movie_title.append(record[2]));

        //If it doesn't create 
        if((findMovie == movieList.end()) || movieList.empty())
        {
            //create move if it doesn't exist yet
            Movie * newMovie = new Movie(originalTitle, movie_year);
            
            //Insert the actor into the movie
            newMovie->cast.push_back(workingActor);
            

            //Insert new movie into list, with year appended for key
            pair<string, Movie *> movieInfo(movie_title, newMovie);
            
            movieList.insert(movieInfo);
        }
        else
        {
            //Add actor to new cast if it exists already
            findMovie->second->cast.push_back(workingActor);       
        }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

/* Funtion name: build
 * Description: calls connect for each movie in the movieList to make all
 * edges between actors
 * Parameters:
 *   bool use_weighted - whether edges should be weighted or not
 * Return value: True if graph is built sucessfully false otherwise
 */
bool ActorGraph::build(bool use_weighted)
{
    Movie * work;

    //If there are no actors, don't execute further 
    if (movieList.empty())
    {
        cerr << "Movie list is empty. Make sure actors/movie info are loaded"
        << "correctly." << endl;
        return false;
    }
    
    //Loop through all movies
    unordered_map<string, Movie *>::iterator mov;
    
    //In each movie, connect all nodes to each other
    for (mov = movieList.begin(); mov != movieList.end(); mov++)
    {
        mov->second->connect(use_weighted);
    }
    return true;
}

/* Funtion name: clearFields
 * Description: reset prev, dist, and visted fields in nodes for the next
 * call
 * Parameters: none
 * Return value: none
 */
void ActorGraph::clearFields()
{
    //Iterator to parse entire unordered_map
    unordered_map<string, ActorNode *>::iterator it;
    
    //Parse unordered_map and reset the fields of every ActorNode
    for(it = actorList.begin() ; it != actorList.end(); it++)
    {
        it->second->prev = 0;
        
        it->second->dist = INT_MAX;

        it->second->visited = false; 
    }
}

/* Funtion name: ~ActorGraph
 * Description: Destructor for ActorGraph, deletes pointers in 
 * unordered_maps
 * Parameters: none
 * Return value: none
 */
ActorGraph::~ActorGraph()
{  
    //Iterator to parse entire list of actors
    unordered_map<string, ActorNode *>::iterator itact;

    //Move through entire list of actors and delete each node
    for(itact = actorList.begin() ; itact != actorList.end(); itact++)
    {
        delete itact->second;
    }

    //clear the vector to remove pointers
    actorList.clear();

    //Iterator to parse list of movies
    unordered_map<string, Movie *>::iterator itmov;

    //Move through entire list of movies and delete each one
    for(itmov = movieList.begin() ; itmov != movieList.end(); itmov++)
    {
        //before deleting each one delete all edges held within it
        itmov->second->deleteEdges();
        delete itmov->second;
    }
    movieList.clear();
  
}
