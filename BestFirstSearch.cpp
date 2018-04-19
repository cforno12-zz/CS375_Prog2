#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class Item {
public:
    int weight;
    int profit;
};

class Node {
public:
    int level;
    int profit;
    int weight;
    int bound;
    vector<Item*> history;
};


bool item_sort_compare (Item* i,Item* j);

bool Compare(Node u, Node v) {
    return u.bound < v.bound;
}

void print_vector(vector<Item*> v);

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

    if(!input || !output){
        fprintf(stderr, "One of the files does not exist.\n");
        exit(1);
    }

    string line;
    const char delim[2] = ",";
    char* token;

    int num_items = 0;
    int capacity = 0;
    bool first = true;
    vector<Item*> items;
    int total_nodes = 0;

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
            //this is the number of items OR weight of item
            token = strtok(new_line, delim);
            t1->weight = atoi(token);
            //this the total weight of items OR profit of item
            token = strtok(NULL, delim);
            t1->profit = atoi(token);

            items.push_back(t1);
        }
    }
    input.close();
    //finished loading from input file

    //sorting items based off of profit/weight ratio
    sort(items.begin(), items.end(), item_sort_compare);

    //Will be used to calculate max profit
    int max_profit = 0;

    //creating a priority queue to traverse through the node
    std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> Q(Compare);
    Node current = Node();
    Node next = Node(); //next is the child of current

    //initialize current node
    current.profit = 0;
    current.weight = 0;
    current.level = 0;
    Q.push(current);

    while(!Q.empty){
        current = Q.top();
        Q.pop();



    }


    output.close();
    exit(0);
}


void print_vector(vector<Item*> v){
    for (std::vector<Item*>::const_iterator i = v.begin(); i != v.end(); ++i)
        std::cout << "Weight: " << (*i)->weight << " Profit: "<< (*i)->profit << endl;
}

bool item_sort_compare (Item* i,Item* j) {
    return ((float)i->profit)/((float)i->weight) > ((float)j->profit)/((float)j->weight);
}
