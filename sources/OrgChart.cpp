#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <iterator>
#include "OrgChart.hpp"
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
int global = 0;
namespace ariel
{
    OrgChart::OrgChart(){
        this->root = NULL;
    }
    
    // copy constructor
    OrgChart::OrgChart(const OrgChart &other){
        this->root = NULL;
        this->root = other.root;
        this->root->number_of_children = other.root->number_of_children;
        this->root->level = other.root->level;
        this->root->name = other.root->name;
        this->root->parent = other.root->parent;
        this->root->children = other.root->children;
        for(uint i = 0; i < other.root->children.size(); i++){
            this->root->children[i] = other.root->children[i];
        }
    }
    // implement move constructor
    OrgChart::OrgChart(OrgChart &&other) noexcept: root(other.root){
        other.root = NULL;
    }


    // using the BFS method in aim to run on the whole tree in level order
    void BFS(node *root, vector<node *> &node_list)
    {

        node_list.clear();
        queue<node *> my_queue;
        my_queue.push(root);

        int currentLevel = 0;
        while (!my_queue.empty())
        {
            node *p = my_queue.front();
            node_list.push_back(p);
            for (node *n : p->children)
            {
                n->level = currentLevel + 1;
                my_queue.push(n);
            }
            my_queue.pop();
        }
    }

    // using the BFS method in aim to run on the whole tree in reverse level order
    void BFS_Reversed(node *root, vector<node *> &node_list)
    {

        node_list.clear();
        queue<node *> my_queue;
        my_queue.push(root);
        int currentLevel = 0;
        while (!my_queue.empty())
        {
            node *p = my_queue.front();
            node_list.push_back(p);
            for (uint i = p->children.size(); i > 0; i--)
            {
                p->children[i - 1]->level = currentLevel + 1;
                my_queue.push(p->children[i - 1]);
            }
            my_queue.pop();
        }
        reverse(node_list.begin(), node_list.end());
    }

    // PreOrder running on the tree
    void preOrder(node *root, vector<node *> &node_list)
    {

        node_list.push_back(root);
        for (node *child : root->children)
        {
            preOrder(child, node_list);
        }
    }

    Iterator::Iterator(node *root, IteratorType type)
    {
        this->root = root;
        index = 0;
        if (root != NULL)
        {
            switch (type)
            {
            case Level_Order:
                BFS(root, this->node_list);
                break;
            case Reverse_Level_Order:
                BFS_Reversed(root, this->node_list);
                break;
            case Pre_Order:
                preOrder(root, this->node_list);
                break;
            default:;
            }

            if (this->node_list.empty())
            {
                this->root = NULL;
            }
        }

        
    }

    void OrgChart::delete_org_chart(node *root)
    {
        if (root != NULL)
        {
            for (node *child : root->children)
            {
                delete_org_chart(child);
            }
            delete root;
        }
        else
        {
            return;
        }    
    }

    OrgChart OrgChart::add_root(const string &name)
    {
        /*
        this function add a new root to the org chart, if there is root already, it will replace it
        */
        if (this->root == NULL)
        {
            cout << "root added" << endl;
            this->root = new node;
            this->root->children = vector<node *>();
            /*
            TODO:
            CHECK WHAT TO DO EITH THE PREVIOUS ORGCHART
            */
            this->root->name = name;
            this->root->parent = NULL;
            this->root->level = 0;
            this->root->number_of_children = 0;
            return *this;
        }
        
        
            node *old_root = this->root;
            node *new_root = new node;
            new_root->children = old_root->children;
            new_root->name = name;
            new_root->parent = NULL;
            new_root->level = 0;
            for (node *child : this->root->children)
            {
                cout << "Child's name: " << child->name << endl;
                child->parent = new_root;
            }
            this->root = new_root;
            old_root = NULL;
            return *this;
        
    }
    OrgChart OrgChart::add_sub(const string &parent, const string &child)
    {
        if (this->root == nullptr)
        {
            throw std::runtime_error("No root in the org chart");
        }
        // this function add a new child to the org chart, if there is no parent, it will throw an exception
        node *parent_node = this->find_node(parent, this->root);
        if (parent_node == NULL)
        {
            throw std::invalid_argument("parent does not exist-first");
        }
        node *child_node = new node;
        child_node->name = child;
        parent_node->children.push_back(child_node);
        child_node->parent = parent_node;
        child_node->level = parent_node->level + 1;
        parent_node->number_of_children++;
        return *this;
    }
    // print our org chart
    std::ostream &operator<<(ostream &os, const OrgChart &org)
    {

        node *current_node = org.root;
        os << "        ----------" << current_node->name << "---------- " << endl;
        OrgChart::print_recursive(current_node);
        return os;
    }

    node *OrgChart::find_node(const string &name, node *root)
    {
        /*
        this function find a node in the org chart, if there is no node, it will return NULL
        */

        node *current_node = root;

        if (current_node->name == name)
        {
            return current_node;
        }
        
            for (node *n : current_node->children)
            {
                current_node = find_node(name, n);
                if (current_node != NULL)
                {
                    return current_node;
                }
            }
        
        return NULL;
    }

    void OrgChart::print_recursive(node *root)
    {
        // this function print the org chart with recursion according to his children
        global = root->level;
        for (node *n : root->children)
        {

            cout << "     " << n->name << " <----- is child of--->  " << n->parent->name << endl;
            OrgChart::print_recursive(n);
        }
    }
    Iterator OrgChart::begin() const
    {
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(this->root, Level_Order);
    }

    Iterator OrgChart::end()const
    {
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(NULL);
    }

    Iterator OrgChart::begin_level_order()const
    {
        // this function return the begin of the level order iterator
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(this->root, Level_Order);
    }

    Iterator OrgChart::end_level_order()const
    {
        // this function return the end of the level order iterator
        if (this->root == NULL){
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(NULL);
    }

    Iterator OrgChart::begin_reverse_order() const
    {
        // this function return the begin of the reverse order iterator
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(this->root, Reverse_Level_Order);
    }

    Iterator OrgChart::reverse_order() const
    {
        // this function return the end of the reverse order iterator
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(NULL);
    }

    Iterator OrgChart::begin_preorder()const
    {
        // this function return the begin of the preorder iterator
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(this->root, Pre_Order);
    }

    Iterator OrgChart::end_preorder()const
    {
        // this function return the end of the preorder iterator
        if (this->root == NULL)
        {
            throw std::invalid_argument("chart is empty!");
        }
        return Iterator(NULL);
    }

    // implement opreator =
    OrgChart &OrgChart::operator=(const OrgChart &org)
    {
        if (this != &org)
        {
            delete_org_chart(this->root);
            this->root = new node;
            this->root->children = vector<node *>();
            this->root->name = org.root->name;
            this->root->parent = NULL;
            this->root->level = 0;
            this->root->number_of_children = 0;
            node *current_node = org.root;
            node *new_node = this->root;
            for (node *child : current_node->children)
            {
                new_node->children.push_back(new node);
                new_node->children.back()->name = child->name;
                new_node->children.back()->parent = new_node;
                new_node->children.back()->level = child->level;
                new_node->children.back()->number_of_children = child->number_of_children;
                new_node = new_node->children.back();
            }
        }
        return *this;
    }
   

}
