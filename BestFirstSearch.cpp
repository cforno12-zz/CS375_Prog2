#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//Item class used to hold weight and profit of an item
class Item {
public:
    float weight;
    float profit;
};

//Node class used to hold information of the decision tree
class Node {
public:
    int level; //level at which the node is in
    float profit; // aggragated profit of node in tree
    float weight; // aggragated weight of node in tree
    int bound; //bound of node calcuated w/ profit_bound function
    vector<Item*> history; // node path

    //override of > operator for sorting priority queue
    bool operator>(const Node &rhs) const{
        return bound < rhs.bound;
    }
};

bool item_sort_compare (Item* i,Item* j);
void print_vector(vector<Item*> v);
int profit_bound(Node node, int capacity, int num_items, vector<Item*> items);

int main(int argc, char** argv){

    //check to make sure the program is being used correctly
    if (argc < 2 || argc > 3){
        fprintf(stderr, "Usage: ./BestFirstSearch <input.txt> <output.txt>\n");
        exit(1);
    }

    ifstream input;
    ofstream output;

    input.open(argv[1]);
    output.open(argv[2]);

    //make sure that the files open properly
    if(!input || !output){
        fprintf(stderr, "One of the files does not exist.\n");
        exit(1);
    }

    //file parsing variables
    string line;
    const char delim[2] = ",";
    char* token;

    //knapsack variables
    int num_items = 0;
    int capacity = 0;
    bool first = true; //boolean to determine wheather it is the first line of the file or not
    vector<Item*> items; // holds all of the items

    //reading input file
    while(getline(input, line)){
        char* new_line = (char*) line.c_str();
        if (first){
            token = strtok(new_line, delim);
            num_items = atoi(token);
            token = strtok(NULL, delim);
            capacity = atoi(token);
            first = false;
        } else {

            Item* t1 = new Item();
            //weight of item
            token = strtok(new_line, delim);
            t1->weight = atoi(token);
            //profit of item
            token = strtok(NULL, delim);
            t1->profit = atoi(token);
            //put item in vector
            items.push_back(t1);
        }
    }
    input.close(); //finished loading from input file

    //sorting items based off of profit/weight ratio
    sort(items.begin(), items.end(), item_sort_compare);

    //creating a priority queue to traverse through the node
    priority_queue<Node, vector<Node>, greater<Node> > pq;
    //dummy nodes
    Node u = Node();
    Node v = Node();
    int non_promising_nodes = 0; //# of non-promising/leaf nodes found
    int nodes_visited = 0; // total nodes visited in the tree
    int maxprofit= 0;
    Node best_node = Node(); // best node so far

    // Initialize frontier node
    v.profit = 0;
    v.weight = 0;
    v.level = 0;

    v.bound = profit_bound(v, capacity, num_items, items);

    pq.push(v);

    // Iterate through all nodes in queue and add children to v if it is promising
    while(!pq.empty()) {
        v = pq.top();
        pq.pop();

        if(v.bound > maxprofit){
            // u child of v
            u.level = v.level + 1;

            // "yes" child
            u.weight = v.weight + items[u.level - 1]->weight;
            u.profit = v.profit + items[u.level - 1]->profit;

            u.history = v.history;
            u.history.push_back(items[u.level-1]);

            // update max_profit
            if((u.weight <= capacity) && (u.profit > maxprofit)) {
                maxprofit = u.profit;
                best_node = u;
            }
            u.bound = profit_bound(u, capacity, num_items, items);
            if(u.bound > maxprofit){
                pq.push(u);
            } else {
                non_promising_nodes++;
            }
            // "no" child
            u.weight = v.weight; // not included
            u.profit = v.profit;
            u.history = v.history;
            u.bound = profit_bound(u, capacity, num_items, items);
            if(u.bound > maxprofit){
                pq.push(u);
            } else {
                non_promising_nodes++;
            }
            nodes_visited++;
        } else {
            non_promising_nodes++;
        }
    }

    nodes_visited+=non_promising_nodes;

    //writing results to the output file
    if(output.is_open()) {
        output << num_items << "," << maxprofit << "," << best_node.history.size() << endl;
        output << nodes_visited << "," << non_promising_nodes << endl;
        for(int i = 0; i < (int)best_node.history.size(); i++) {
            output << best_node.history[i]->weight << "," << best_node.history[i]->profit << std::endl;
        }
    } else {
        cout << "Open for output file failed," << endl;
    }

    output.close();
    exit(0);
}

//used to sort the items by profit/weight ratio
bool item_sort_compare (Item* i,Item* j) {
    return ((float)i->profit)/((float)i->weight) > ((float)j->profit)/((float)j->weight);
}

/* profit_bound takes in the node that it is calculating the bound for, the number of
 * items, the capacity of the knapsack, and the list of item objects.
 * profit_bound calculates the bound of the current node in the tree by doing
 * the fractional knapsack problem. It returns its bound.
 */
int profit_bound(Node node, int capacity, int num_items, vector<Item*> items){
    // if weight is greater knapsack capacity return 0
    if (node.weight >= capacity)
        return 0;

    // initialize profit_bound by current profit
    int profit_bound = node.profit;
    int j = node.level;
    int tot_weight = node.weight;

    // checking index and capacity conditions
    while ((j < num_items) && (tot_weight + items.at(j)->weight <= capacity))
        {
            tot_weight += items.at(j)->weight;
            profit_bound += items.at(j)->profit;
            j++;
        }

    if (j < num_items)
        profit_bound += (capacity - tot_weight) * items.at(j)->profit/items.at(j)->weight;

    return profit_bound;
}
