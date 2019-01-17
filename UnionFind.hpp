/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        UnionFind.hpp
 * Description:     Contains methods for a disjoint set data structure, which
 *                  primarily functions as an up-tree.
 * Assigment Number: 4 
 * Sources of help: 
 ***/

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "ActorNode.h"
#include "Movie.h"


using namespace std;

class UnionFind 
{
    public:
        unordered_map<string, Movie *> movieList;
        unordered_map<string, vector<ActorNode *>> forest;
  
        UnionFind(void);
        bool loadFromFile(const char* in_filename);
        ActorNode * find(ActorNode * actor);
        bool unionActor(ActorNode * actor1, ActorNode * actor2);

        ~UnionFind();
    private:
	
};

/* Funtion name: UnionFind
 * Description: constructor for the unionfind object
 * Parameters: none
 * Return value: none
 */
UnionFind::UnionFind(void){}

bool UnionFind::loadFromFile(const char * in_filename)
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
        unordered_map<string, vector<ActorNode *>> :: iterator findActor = 
            forest.find(actor_name);
        

        //Create new actor node if no node for actor exists yet
        if((findActor == forest.end()) || forest.empty())
        {
            //Create new actor object
            ActorNode * newActor = new ActorNode(actor_name);

            vector<ActorNode *> forestEntry;

            forestEntry.push_back(newActor);

            //Create pair with actorname as key to insert to hashmap
            pair <string, vector<ActorNode *>> 
                actorInfo(actor_name, forestEntry);

            //Insert to hashmap
            forest.insert(actorInfo);

            workingActor = newActor;
        }

        else
        {
            workingActor = findActor->second[0];
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

/* Funtion name: unionActor
 * Description: union two actors from two disjointed sets
 * Parameters:
 *   ActorNode * actor1 - first actor to union
 *   ActorNode * actor2 - second actor to union
 * Return value: true if union is successful false otherwise
 */
bool UnionFind::unionActor(ActorNode * actor1, ActorNode * actor2)
{ 
    ActorNode * temp1 = find(actor1);
    ActorNode * temp2 = find(actor2);
    
    // if they have the same sentinel theres no need for union
    if(temp1 == temp2)
    {
        return true;
    }
	// iterators to find the two sets with the sentinels
    unordered_map<string, vector<ActorNode *>>::iterator set1 = 
        forest.find(temp1->name);
    unordered_map<string, vector<ActorNode *>>::iterator set2 = 
        forest.find(temp2->name);

    // if the sets are not in the file
    if(set1 == forest.end() || set2 == forest.end())
    {
        return false;
    }

    // the larger set becomes the sentinel
    if(set1->second.size() > set2->second.size())
    {
        set2->second[0]->prev = set1->second[0];

        // add nodes of set2 to the set1's vector
        for(int i = 0; i < set2->second.size(); i++)
        {
            set1->second.push_back(set2->second[i]);
        }
    }
    // when set2 is the bigger set do the opposite
    else
    { 
        set1->second[0]->prev = set2->second[0];

        for(int i = 0; i < set1->second.size(); i++)
        {
            set2->second.push_back(set1->second[i]);
        } 
    }
    return true;
}

/* Funtion name: find
 * Description: find the sentinel of the actor and optimize nodes along the 
 * paths
 * Parameters: ActorNode * actor - actor we would like to find the sentinel of
 * Return value: sentinel of the actor
 */
ActorNode * UnionFind::find(ActorNode * actor)
{
    // list of actors on the path for optimization purposes
	vector<ActorNode *> traversedNodes;
    
    // when actor is the sentinel
    if(!actor->prev)
    {
        return actor;
    }

    // keep going to the top of the set
    while(actor->prev)
    {
        // add node to traversedNodes to optimize the paths
        traversedNodes.push_back(actor);
        actor = actor->prev;
    }

    // for all the nodes on the path
    for(int i = 0; i < traversedNodes.size(); i++)
    {
        // make prev the sentinel of the set
        traversedNodes[i]->prev = actor;
    }
    
    return actor;
}

/* Funtion name: ~UnionFind
 * Description: destructor of the unionfind object
 * Parameters: none 
 * Return value: none
 */
UnionFind::~UnionFind()
{
    // iterator for the forest
	unordered_map<string, vector<ActorNode *>>::iterator ra;                    
    // iterator for the movieList
	unordered_map<string, Movie *>::iterator rm;                                
                                                                                
    for(ra = forest.begin(); ra != forest.end(); ra++)                          
    {   
        // delete the sentinels of each set, since we never deleted
        delete ra->second[0];                                                   
    }                                                                           
    forest.clear();                                                             
                                                                                
    for(rm = movieList.begin(); rm != movieList.end(); rm++)                    
    {   
        // delete all the movie pointers
        delete rm->second;                                                      
    }
}
#endif
