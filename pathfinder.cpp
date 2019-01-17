/***
 * Name:            Aaron Liu, Larissa Johnson  
 * Date:            Nov 28 2016
 * Filename:        pathfinder.cpp
 * Description:     Program which finds and prints the shortest path between
 *                  two actors, using movies that they or someone they have
 *                  previously starred with have starred in.
 * Assigment Number: 4 
 * Sources of help: 
 ***/

#include "ActorGraph.h"
#include "ActorNode.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <climits>

using namespace std;

class Djcomp
{
    //Comparator to use in priority queue
    public:
    
        /* Funtion name:operator()
         * Description: compare function for the priority queue
         * Parameters: 
         *   ActorNode *& actor1 - first actor in the comparison
         *   ActorNode *& actor2 - second actor in the comparison
         * Return value: true if the first dist is larger than second, false 
         * otherwise
         */
        bool operator()(ActorNode *& actor1, ActorNode *& actor2) const
        {
            return actor1->dist > actor2->dist;
        }
};

/*
 * Function name: printPaths
 * Description: Prints out the path between the first actor and the second
 *              actor to the filestream specified by out
 * Parameters:
 *      start: the ActorNode to start drawing the path from
 *      end: the ActorNode to end at
 *      out: file to print the path to
 * Return Value: none
 */
void printPaths(ActorNode * start, ActorNode * end, ofstream& out)
{
    //Stack to hold complete path between actors
    stack<ActorNode *> printed;
    
    //start from the last node, since traversing using indicated
    //"previous" nodes
    ActorNode * curr = end;
    
    //loop until starting actor is found, or null is reached
    while(curr)
    {
        //Add node to stack to print later
        printed.push(curr);

        //traverse using "previous" nodes
        curr = curr->prev;
        
        //If the current node is the starting actor, path has been found
        if(curr == start)
        {
            //add current actor to path and exit
            printed.push(curr);
            break;
        }
    }
    
    //Edge object to represent movie connecting 2 actors
    ActorEdge * connecting;

    //Path starts from top node on stack; last inserted one
    ActorNode * printing = printed.top();

    //print statement for actor name
    out << '(' << printing->name << ')';

    //remove actor from stack after it's been saved in printing
    printed.pop();

    //Continue while there are still nodes to print
    while(!printed.empty())
    {
        //Grab next node traversed; node at top of stack
        printing = printed.top();

        //find correct edge between 2 nodes on the path
        connecting = printing->edgeFinder(printing->prev);

        //write movie name and year to file
        out << "--[" << connecting->name << "#@" << connecting->year 
            << "]-->";

        //write actor name to file
        out << '(' << printing->name << ')';

        //pop off actor, so next can be used
        printed.pop(); 
    }
    
    //Add one last newline char at end
    out << endl;
}

/*
 * Function name: dijkstra
 * Description: performs dijkstra's algorithm to find paths starting from the
 *              specified node.
 * Parameters:
 *      sActor: the starting actor to begin finding paths from
 * Return Value: none
 */
void dijkstra(ActorNode * sActor)
{
    //Priority queue to perform Dijkstra's Algorithm
    priority_queue<ActorNode *, vector<ActorNode *>, Djcomp> traveler;

    //Set distance of initial node to 0
    sActor->dist = 0;

    //enqueue into priority queue
    traveler.push(sActor);
    
    //Loop until path is found
    while(!traveler.empty())
    {
        //Get node at front of priority queue and then remove it
        ActorNode * current = traveler.top();
        traveler.pop();
        
        //If node hasn't been visited, set it so that it is
        if(!current->visited)
        {
            current->visited = true;
            for(int i = 0; i < current->adj.size(); i++)
            {
                //Current adjacent node from matrix
                ActorNode * adjNode = current->adj[i]->traverse(current);

                //Variable for current path's distance
                int thisDist = current->dist + current->adj[i]->weight;
                
                //If the current path's distance is shorter:
                if(thisDist < adjNode->dist)
                {
                    //current node is now new parent
                    adjNode->prev = current;

                    //Distance changed to smaller distance
                    adjNode->dist = thisDist;
                    
                    //Add adjNode to queue for later
                    traveler.push(adjNode);
                }
            }
        }
    }
}

/* Funtion name: main
 * Description: main driver for the pathfinder function
 * Parameters: 
 *   int argc - number of input arguments
 *   char ** argv - array of arguments
 * Return value: 1 for success -1 for error
 */
int main(int argc, char ** argv)
{
    //Check if the right number of arguments was inputted
    if (argc != 5) 
    {
        cerr << "Incorrect number of arguments" << endl;
        return -1;
    }

    //name of the input file
    const char * input_file = argv[1];

    //name of file containing paths to search for
    string paths = argv[3];
    
    //name of file to print to
    string outfile = argv[4];

    //Specifies to use weighted or unweighted edges
    bool use_weighted;  

    //Iterators for holding the actor at start of path and end of path
    unordered_map <string, ActorNode *>::iterator starting;
    unordered_map <string, ActorNode *>::iterator ending;

    //ActorNodes which will contain actor at start and end of path
    ActorNode * sActor;
    ActorNode * eActor;
    
    //Check if program should use weighted or unweighted edges
    if(*argv[2] == 'u')
    {
        use_weighted = false;
    }

    else if(*argv[2] == 'w')
    {
        use_weighted = true;
    }

    else
    {
        cerr << "Enter a valid weight option" << endl;
        return -1;
    }
    
    //Initialize the graph data structure
    ActorGraph * aGraph = new ActorGraph();
    
    //Streams to read input files and write to output file
    ifstream pairs;
    ofstream pathFile;
    
    //Open input and output files
    pairs.open(paths);
    pathFile.open(outfile);
  
    //This is the header to the output file
    pathFile << "(actor)--[movie#@year]-->(actor)--..." << endl;
    
    //Call ActorGraph's load from file to initialize lists of actors and movies
    aGraph->loadFromFile(input_file, use_weighted);
    
    //build the graph, specifying whether it should be weighted or not
    aGraph->build(use_weighted);

    //If no pairs of actors to search, print an error
    if (!pairs) 
    {
        cerr << "Please enter pairs of actors for search" << endl;
        return -1;
    }
    
    //Used to ignore the header of the input file
    bool have_header = false;
    
    //Loop until all paths in file are found
    while(pairs)
    {
        //line to read from file
        string line;
        
        //if no lines in file, break out
        if(!getline(pairs, line))
        {
            break;
        }
        
        //Ignore header, then continue looping
        if(!have_header)
        {
            have_header = true;
            continue;
        }
        
        //used for extracting actor names from line
        istringstream individual(line);

        //Vector containing the individual names of the two actors
        vector<string> findPath;
        
        //Grab the names of each actor, using tabs to denote change of actor
        while(individual)
        {
            string next;
             
            if(!getline(individual, next, '\t'))
            {
                break;
            }

            //add new actor to vector
            findPath.push_back(next);
        }
        
        //If more than two vectors found, print error and break out
        if(findPath.size() != 2)
        { 
            cerr << "Please use columns of size 2" << endl;
            continue;
        }
        
        //store names of actors to find path between in new strings
        string start(findPath[0]);
        string end(findPath[1]);
        
        //search for the actorNodes representing each actor using the
        //unordered_maps in the graph
        starting = aGraph->actorList.find(start);
        ending = aGraph->actorList.find(end);
        
        //If actors queried are not in original graph, print error and move on
        if ((starting == aGraph->actorList.end()) || 
           ending == aGraph->actorList.end()) 
        {
            cerr << "Actors not in original file" << endl;
            continue;
        }

        //If path asked for is between the same actor, print error and move on
        if (starting == ending) 
        {
            cerr << "Same actor" << endl;
            continue;
        }

        //Grab actual actor node from iterators
        sActor = starting->second;
        eActor = ending->second;
        
        //Perform dijkstra's algorithm, starting from first node
        dijkstra(sActor);

        //print out all paths and direct to output file
        printPaths(sActor, eActor, pathFile);

        //clear all fields in nodes for next search
        aGraph->clearFields();
    }

    //close input and output files
    pathFile.close();
    pairs.close();

    //Call destructor to end program
    delete aGraph;
    return 1;
}


