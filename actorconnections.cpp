/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        actorconnections.cpp
 * Description:     File which contains method of finding the earliest date in
 *                  which two actors can be linked through movies that they 
 *                  have starred in.
 * Assigment Number: 4
 * Sources of help: 
 ***/

#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"
#include "UnionFind.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <ctime>
#include <chrono>

#define converter 1000000

using namespace std;

class sortYear 
{
  public:
        /*
         * Function Name: operator()
         * Description: comparator to compare the two years in the given movie
         *              objects for use in priority queue
         * Parameters:
         *      movie1: first movie to check year of
         *      movie2: second movie to check year of
         * Return Value: True if the first movie has a year that is later than
         *               the second
         */
        bool operator()(Movie *& movie1, Movie *& movie2) const
        {
            return movie1->year > movie2->year;
        }
};

/*
 * Function Name: main
 * Description: Main function of actorconnections, determines the year that
 *              there exists a connection between two given actors accross the
 *              graph of all actors in a given set.
 * Parameters:
 *      argc: The number of arguments inputted
 *      argv: Array of actual arguments inputted at program start
 * Return Value: 1 if sucessful, -1 if invalid input
 */
int main (int argc, char ** argv)
{
    if (argc != 5) 
    {
        cerr << "Incorrect number of arguments" << endl;
        return -1;
    }
    const char * input_file = argv[1];
    string paths = argv[2];
    string outfile = argv[3];
    string searchType = argv[4];
    
    //Objects and iterators for BFS
    ActorGraph * aConnect;
    unordered_map <string, ActorNode *>::iterator starting;
    unordered_map <string, ActorNode *>::iterator ending;

    //Objects and iterators for Union Find 
    UnionFind * aUnion;
    unordered_map <string, vector<ActorNode *>>::iterator uStarting;
    unordered_map <string, vector<ActorNode *>>::iterator uEnding;
    
    //Nodes representing the beginning and end of paths
    ActorNode * sActor;
    ActorNode * eActor;

    //vector containing movies sorted by earliest to latest year
    vector<Movie *> movieListSorted;
    
    //Used to measure runtime of each respective algorithm
    chrono::time_point<chrono::high_resolution_clock> start_time;
    chrono::time_point<chrono::high_resolution_clock> end_time;
    long long runtimes;
    int iterations = 0;
    
    //If bfs is specified in arguments
    if (searchType == "bfs")
    {
        //Create new actorgraph to conduct bfs on
        aConnect = new ActorGraph();

        //Load data of all actors and movies from file
        aConnect->loadFromFile(input_file, 0);

        //Create priority queue using comparator that sorts movies by earliest
        //year to latest year
        priority_queue < Movie *, vector <Movie *>, sortYear> movieByYear;

        //Parse through the entire movielist, adding each movie to priority
        //queue
        unordered_map<string, Movie *>::iterator it;
        for (it = aConnect->movieList.begin(); it != aConnect->movieList.end(); 
            it++)
        {
            movieByYear.push(it->second);
        }
        
        //Move sorted elements to vector for easier iteration
        while(!movieByYear.empty())
        {
            movieListSorted.push_back(movieByYear.top());
            movieByYear.pop();
        }
        
        //Output that BFS is being run
        cout << "Running BFS" << endl;
    }
    
    //If ufind is specified in arguments
    else if (searchType == "ufind")
    {
        //Create new UnionFind object
        aUnion = new UnionFind();

        //Load data of all actors and movies from files
        aUnion->loadFromFile(input_file);
        
        //Create priority queue using comparator that sorts movies by earliest
        //year to latest year
        priority_queue < Movie *, vector <Movie *>, sortYear> movieByYear;
        
        //Parse through the entire movielist, adding each movie to priority
        //queue
        unordered_map<string, Movie *>::iterator it; 
        for(it = aUnion->movieList.begin(); it != aUnion->movieList.end(); 
            it++)
        {
            movieByYear.push(it->second);
        }
        
        //Move sorted elements to vector for easier iteration
        while(!movieByYear.empty())
        {
            movieListSorted.push_back(movieByYear.top());
            movieByYear.pop();
        }
        
        //Output that Union Find is being run
        cout << "Running Union Find" << endl;
    }
    
    //If any other output is specified, output error and return
    else 
    {
        cerr << "Wrong search type, please specify 'bfs' or 'ufind'." << endl;
        return -1;
    }
    
    //Input and output streams for files
    ifstream pairs;
    ofstream pathFile;
    
    //Open file containing pairs and output file
    pairs.open(paths);
    pathFile.open(outfile);
    
    //Boolean to skip over file header
    bool have_header = false;
    
    //Write header of output files
    pathFile << "Actor1" << "\t" << "Actor2" << "\t" << "Year" << endl;
    
    //Continue looping until all pairs have been read
    while(pairs)
    {
        //string for the entire line read in   
        string line;
        
        //If no line to read in file, exit
        if(!getline(pairs, line))
        {
            break;
        }
        
        //Ignore header on first run
        if(!have_header)
        {
            have_header = true;
            continue;
        }
        
        //Used to extract individual actors from read line
        istringstream individual(line);
        vector<string> findPath;
        
        //Loop until all actors on line are read
        while(individual)
        {
            //string to hold individual actor's name
            string next;
            
            //if no other names are found, exit
            if(!getline(individual, next, '\t'))
            {
                break;
            }

            //add actor name to vector
            findPath.push_back(next);
        }
        
        //If more than 2 actors are read, ignore line
        if(findPath.size() != 2)
        { 
            cerr << "Please use columns of size 2" << endl;
            continue;
        }
        
        //Set strings to the actor names for easy access
        string start(findPath[0]);
        string end(findPath[1]);
        
        //If bfs was specified, perform a bfs search for pair
        if(searchType == "bfs")
        {
            //Start timer to measure runtime of search
            start_time = chrono::high_resolution_clock::now();

            //Find the starting and ending iterators to the actorNodes being
            //searched for
            starting = aConnect->actorList.find(start);
            ending = aConnect->actorList.find(end);
            
            //If beginning or ending node isn't found, skip the line
            if ((starting == aConnect->actorList.end()) || 
            ending == aConnect->actorList.end()) 
            {
                cerr << "Actors not in original file" << endl;
                continue;
            }
            
            //If two actors are actually the same person, also exit
            if (starting == ending) 
            {
                cerr << "Same actor" << endl;
                continue;
            }

            //Get actual nodes from iterators
            sActor = starting->second;
            eActor = ending->second;
            
            //Boolean to check if a valid path has been found
            bool pathFound = false;
            
            //Loop through movies, from earliest release to latest
            for(int i = 0; i < movieListSorted.size(); i++)
            { 
                //Vector of nodes traversed to reset later
                vector<ActorNode *> traversedNodes;

                //Grab the oldest movie that hasn't been connected to the graph
                Movie * oldest = movieListSorted[i];
                
                pathFound = false;
                
                //Since BFS is used, use unweighted edges and connect all
                //actors in movie
                oldest->connect(false);
                
                //If the ending actor hasn't been found yet, continue, since
                //bfs won't find it anyway
                if(eActor->adj.empty())
                {
                    continue;
                }
       
                //Queue to perform BFS
                queue<ActorNode *> explorer;

                //since sActor is starting point, has distance of 0
                sActor->dist = 0;
                
                //Put actor in queue
                explorer.push(sActor);
                
                //loop until all available nodes have been explored
                while(!explorer.empty())
                {
                    //Node to explore neighbors of
                    ActorNode * working = explorer.front();
                    explorer.pop();

                    //Loop through adjacency list to find nodes to traverse to
                    for(int i = 0; i < working->adj.size(); i++)
                    {
                        //get adjacent node
                        ActorNode * neighbor = working->adj[i]->
                            traverse(working);
                        
                        if(neighbor == sActor)
                        {
                            continue;
                        }
                        
                        //If node hasn't been visited, set current node as the
                        //previous node in search
                        if(!neighbor->prev)
                        {
                            neighbor->prev = working;

                            //add new node to queue to explore later
                            explorer.push(neighbor);

                            //Track traversed nodes to reset later
                            traversedNodes.push_back(neighbor);
                        }
                    }
                }
                
                //Start finding path through ending actor
                ActorNode * curr = eActor;
                
                //While there is still a node to traverse to
                while(curr)
                {
                    //If no parent, done traversing
                    if(!curr->prev)
                    {
                        break;
                    }
                    
                    //traverse to previous node
                    curr = curr->prev;
                    
                    //If previous node is starting node, found path
                    if(curr == sActor)
                    {
                        //Print year that two actors are connected
                        pathFile << sActor->name << "\t" << eActor->name 
                            << "\t" << oldest->year << endl;
                        pathFound = true;
                        break;
                    }
                }
                
                //Reset all prevs for searching next round
                unordered_map<string, ActorNode*>::iterator clearPrevs;
                for(int j = 0; j < traversedNodes.size(); j++)
                {
                    traversedNodes[j]->prev = 0; 
                }
                
                //Exit for loop if path is found, no need to continue
                if(pathFound)
                {
                    break;
                }

            }
            
            //If no path is found over entire search, output 9999 as connecting
            //year
            if(!pathFound)
            {
                pathFile << sActor->name << "\t" << eActor->name << "\t" 
                    << "9999" << endl;
            }
            

            unordered_map<string, ActorNode*>::iterator resetting;

            //If not at eof, reset adjacency matrixes, don't do on last because
            //destructor is called anyway
            if(pairs.peek() != -1)
            {
                //clear all adjacency matrixes after graph is finished
                for(resetting = aConnect->actorList.begin(); resetting != 
                    aConnect->actorList.end(); resetting++)
                {
                    resetting->second->adj.clear();
                }
            }
            
            //finish timing current round
            end_time = chrono::high_resolution_clock::now();
            
            //Add to total runtime
            runtimes += (long long)chrono::duration_cast<chrono::nanoseconds>
                (end_time - start_time).count();
            iterations++;
        }
        
        //If ufind is specified search for pair using Union Find
        if(searchType == "ufind")
        {
            //Start timer to measure runtime of search
            start_time = chrono::high_resolution_clock::now();
            
            //Find iterators to starting actor and ending actor
            uStarting = aUnion->forest.find(start);
            uEnding = aUnion->forest.find(end);
            
            //If actors don't exist in original input file, skip this pair
            if ((uStarting == aUnion->forest.end()) || 
            uEnding == aUnion->forest.end()) 
            {
                cerr << "Actors not in original file" << endl;
                continue;
            }
            
            //If pair of actors actually same actor, skip pair
            if (uStarting == uEnding) 
            {
                cerr << "Same actor" << endl;
                continue;
            }
            
            //Get actual actorNodes for current pair
            sActor = uStarting->second[0];
            eActor = uEnding->second[0];
            
            //Used to access current oldest movie in list
            Movie * working;

            //Indicates if a path has been found or not
            bool pathFound = false;

            //Used to track nodes traversed, to reset later
            vector<ActorNode *> traversedNodes;
            
            //Loop through movies, from earliest release to latest
            for(int i = 0; i < movieListSorted.size(); i++)
            {
                //no path has been found at the start
                pathFound = false;

                //set working to earliest movie that hasn't been used
                working = movieListSorted[i];
                
                //If current movie only has 1 actor, nothing needs to be done
                if (working->cast.size() <= 1)
                {
                    continue;
                }
                
                //Start by tracking first actor listed in cast
                traversedNodes.push_back(working->cast[0]);
                
                //Union all disjoint sets of actors in the movie
                for (int j = 1; j < working->cast.size(); j++)
                {
                    aUnion->unionActor(working->cast[0], working->cast[j]);

                    //Track actors being traversed over
                    traversedNodes.push_back(working->cast[j]);
                }
                
                //Call find; if returned sentinels are the same, actors are in
                //same set and path has been found
                if (aUnion->find(sActor) == aUnion->find(eActor))
                {
                    //print out year actors were connected and exit
                    pathFile << sActor->name << "\t" << eActor->name 
                            << "\t" << working->year << endl;
                    pathFound = true;
                    break;
                }
            }
            
            //If no path is found, output 9999 as year actors were connected
            if (!pathFound) 
            {
                pathFile << sActor->name << "\t" << eActor->name << "\t" 
                    << "9999" << endl;
            }
            
            //Reset all prev fields in nodes for next pair
            for(int k = 0; k < traversedNodes.size(); k++)
            {
                traversedNodes[k]->prev = 0;
            }
            
            //Iterate through forest of nodes and reduce all sets to
            //single-node sets for next pair
            unordered_map<string, vector<ActorNode *>>::iterator clearing;
            for(clearing = aUnion->forest.begin(); clearing != 
                aUnion->forest.end(); clearing++)
            {
                clearing->second.resize(1);
            }
            
            //End runtime of current operation
            end_time = chrono::high_resolution_clock::now();
            
            //Add to total runtime of program
            runtimes += (long long)chrono::duration_cast<chrono::nanoseconds>
                (end_time - start_time).count();
            iterations++;
            
        }
    } 
    
    //Convert runtime from nanoseconds to milliseconds
    runtimes = (runtimes / converter);
    
    //Output total runtime
    cout << "The duration in milliseconds is : " << runtimes << endl;
    
    //Calculate average runtime by dividing total runtime by number of
    //iterations in operation
    long long avgRuntime = (runtimes / (long long)iterations);
    
    //output average runtime
    cout << "Average runtime is : " << avgRuntime << endl;
    
    //Call destructor for either ActorGraph or UnionFind to free memory
    if(searchType == "bfs")
    {
        delete aConnect;
    }
    else if(searchType == "ufind")
    {
        delete aUnion;
    }

    return 1;
}
