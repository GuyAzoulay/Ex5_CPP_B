#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <vector>

using namespace std;
namespace ariel
{
    struct node
    {
        string name;
        vector<node *> children;
        node *parent;
        int level;
        int number_of_children; // number of children of the current node

        ~node()
        {
            for (uint i = 0; i < children.size(); i++)
            {
                delete children[i];
            }
        }
    };
    // creating new data type for the OrgChart
    enum IteratorType
    {
        Level_Order,
        Reverse_Level_Order,
        Pre_Order,
        No_Type
    };

    class Iterator
    {
        node *root;
        vector<node *> node_list;
        uint index;

    public:
        Iterator()
        {
            root = NULL;
            index = 0;
        }
        Iterator(node *root, IteratorType type = No_Type);

        Iterator &operator++()
        {
            if (index < node_list.size() - 1)
            {
                index++;
            }
            else
            {
                *this = Iterator(nullptr);
            }
            return *this;
        }
        std::string operator*()
        {

            return node_list[index]->name;
        }
        string *operator->()
        {
            return &node_list[index]->name;
        }

        bool operator!=(const Iterator &other)
        {
            return this->root != other.root;
        }
    };

    class OrgChart
    {
    public:
        node *root;
        OrgChart();
        // deep copy constructor
        OrgChart(const OrgChart &other);
        // A transfer constructor perform superficial copying with smart pointers.
        OrgChart(OrgChart &&other) noexcept; // use 'noexcept' to avoid leaking memory


        static void delete_org_chart(node *root);
        static void print_recursive(node *root);
        OrgChart add_root(const string &name);
        OrgChart add_sub(const string &parent, const string &child);
        friend ostream &operator<<(ostream &os, const OrgChart &org);
        // itarate over the tree and print the tree with level order traversal
        Iterator begin_level_order()const;
        Iterator end_level_order()const;

        Iterator begin_reverse_order()const;
        Iterator reverse_order()const;

        Iterator begin_preorder()const;
        Iterator end_preorder()const;

        Iterator begin()const;
        Iterator end()const;

        node *find_node(const string &name, node *root);

       // signature opretor =
        OrgChart &operator=(const OrgChart &org);
        // signature opretor =
        OrgChart &operator=(OrgChart &&other) noexcept;
    };

}