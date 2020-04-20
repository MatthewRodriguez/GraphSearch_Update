//
//  GraphSearch.cpp
//  GraphSearch
//
//  Created by Matt Rodriguez on 3/26/20.
//  Copyright © 2020 Matthew Rodriguez. All rights reserved.
//

#include "GraphSearch.h"

GraphSearch::GraphSearch(){
    nodes.clear();
}

void GraphSearch::addNode(int val){
    Node *n = new Node(val);
    if (nodes.empty())
        nodes.push_back(n);
    else{
        for (int i = 0; i < nodes.size(); i++)
            if (nodes[i]->info == val)
                return;
        nodes.push_back(n);
    }
}

unordered_set<Node*> GraphSearch::getAllNodes(){
    unordered_set<Node*> s;
    for (int i = 0; i < nodes.size(); i++)
        s.insert(nodes[i]);
    return s;
}

void GraphSearch::clear(){
    nodes.clear();
}

void GraphSearch::addUndirectedEdge(Node *first, Node *second){
     if ((first->list.empty() && second->list.empty()) && (first->info != second->info)){
          first->list.push_back(second);
          second->list.push_back(first);
      }
      else if(first->info == second->info){
          return;
      }
      else{
          for (int i = 0; i < first->list.size(); i++)
              if (first->list[i]->info == second->info)
                  return;
          for (int i = 0; i < second->list.size(); i++)
              if (second->list[i]->info == first->info)
                return;
          first->list.push_back(second);
          second->list.push_back(first);
      }
}

void GraphSearch::undirectedEdge(Node *first, Node *second){
    first->list.push_back(second);
}

void GraphSearch::removeUndiretedEdge(Node *first, Node *second){
    if (first->list.empty() && second->list.empty())
        return;
    for (int i = 0; i < first->list.size(); i++){
        if(first->list[i]->info == second->info)
            first->list.erase(first->list.begin()+i);
    }
    for (int i = 0; i < second->list.size(); i++){
        if(second->list[i]->info == first->info)
            second->list.erase(second->list.begin()+i);
    }
}

void GraphSearch::printNodes(){
    cout << " { ";
    for (int i = 0; i < nodes.size(); i++)
        cout << nodes[i]->info << " ";
    cout << " }" << endl;
}

void GraphSearch::printU(){
    for (int i = 0; i < nodes.size(); i++){
        cout << nodes[i]->info << ": ";
        for (int j = 0; j < nodes[i]->list.size(); j++)
            cout << nodes[i]->list[j]->info << " ";
        cout << endl;
    }
    cout << endl;
}

void GraphSearch::createRandom(int n){
    int random, randA, randB, times = n*5;
    for (int i = 0; i < n; i++){
        random = (rand() % times) + 1;
        addNode(random);
    }
    for (int i = 0; i < nodes.size(); i++){
        if(nodes.size() > 1){
            randA = rand() % nodes.size();
            randB = rand() % nodes.size();
            addUndirectedEdge(nodes[randA], nodes[randB]);
        }
    }
}

void GraphSearch::createLinkedList(int size){
    for (int i = 0; i < size; i++){
        addNode(i);
        if (nodes.size() > 1)
            undirectedEdge(nodes[nodes.size()-2], nodes[nodes.size()-1]);
    }
}

vector<Node*> GraphSearch::DF(){
    return DFSRec(nodes[0], nodes[4]);
}

vector<Node*> GraphSearch::DFSRec(Node *start, Node *end){
    vector<Node*> container, path;
    bool found = false;
    start->visit = true;
    path.push_back(start);
    for (int i = 0; i < start->list.size(); i++){
        if (!start->list[i]->visit){
            start->list[i]->visit = true;
            if (start->list[i]->info == end->info){
                path.push_back(start->list[i]);
                resetVisit();
                return path;
            }
            container.push_back(start->list[i]);
            DFSHelper(container, path, end, found);
            if (found){
               resetVisit();
               return path;
            }
        }
    }
    for (int i = 0; i < nodes.size(); i++)
        nodes[i]->visit = false;
    return path;
}

void GraphSearch::DFSHelper(vector<Node*> &vec, vector<Node*> &p, Node *end, bool &found){
 Node *n;
 if (vec.empty())
     return;
 n = vec.back();
 vec.pop_back();
 p.push_back(n);
 for (int i = 0; i < n->list.size(); i++){
     if (!n->list[i]->visit){
         n->list[i]->visit = true;
         if (n->list[i]->info == end->info){
            found = true;
            p.push_back(n->list[i]);
            return;
         }
         vec.push_back(n->list[i]);
     }
 }
 DFSHelper(vec, p, end, found);
}

//This function is only for testing.
vector<Node*> GraphSearch::D(){
    return DFSIter(nodes[0], nodes[4]);
}

vector<Node*> GraphSearch::DFSIter(Node *start, Node *end){
    vector<Node*> stack, path;
    Node *n;
    start->visit = true;
    path.push_back(start);
    for (int i = 0; i < start->list.size(); i++){
        if (!start->list[i]->visit){
            start->list[i]->visit = true;
            if (start->list[i]->info == end->info){
                path.push_back(start->list[i]);
                return path;
            }
            stack.push_back(start->list[i]);
            while (!stack.empty()){
                n = stack.back();
                stack.pop_back();
                path.push_back(n);
                for (int j = 0; j < n->list.size(); j++){
                    if (!n->list[j]->visit){
                        n->list[j]->visit = true;
                        if (n->list[j]->info == end->info){
                            path.push_back(n->list[j]);
                            resetVisit();
                            return path;
                        }
                        stack.push_back(n->list[j]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < nodes.size(); i++)
        nodes[i]->visit = false;
    return path;
}

vector<Node*> GraphSearch::BFTIter(){
    vector<Node*> queue, path;
    Node *n;
    for (int i = 0; i < nodes.size(); i++){
        if (!nodes[i]->visit){
            nodes[i]->visit = true;
            queue.push_back(nodes[i]);
            /*
              This while loop iterate through the queue
              unitl is empty and consider the first element on it
              to add to the path if it noyt there already.
            */
            while (!queue.empty()){
                n = queue.front();
                queue.erase(queue.begin());
                path.push_back(n);
                for (int j = 0; j < n->list.size(); j++){
                    if (!n->list[j]->visit){
                        n->list[j]->visit = true;
                        queue.push_back(n->list[j]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < nodes.size(); i++)
        nodes[i]->visit = false;
    return path;
}

vector<Node*> GraphSearch::BFTRec(){
    vector<Node*> v, path;
    for (int i = 0; i < nodes.size(); i++){
        if (!nodes[i]->visit){
            nodes[i]->visit = true;
            v.push_back(nodes[i]);
            //BFTHelper is where the recursion happens.
            BFTHelper(v, path);
        }
        
    }
    for (int i = 0; i < nodes.size(); i++)
        nodes[i]->visit = false;
    return path;
}

void GraphSearch::BFTHelper(vector<Node*> &vec, vector<Node*> &p){
    Node *n;
    if (vec.empty())
        return;
    n = vec.front();
    vec.erase(vec.begin());
    p.push_back(n);
    for (int i = 0; i < n->list.size(); i++){
        if (!n->list[i]->visit){
            n->list[i]->visit = true;
            vec.push_back(n->list[i]);
        }
    }
    BFTHelper(vec, p);
}

vector<Node*> GraphSearch::BFTRecLinkedList(){
    return BFTRec();
}

vector<Node*> GraphSearch::BFTIterLinkedList(){
    return BFTIter();
}
