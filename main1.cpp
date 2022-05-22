#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <queue>
using namespace std;
//using namespace ariel;
#include <algorithm>
#include "sources/OrgChart.hpp"


 

int main(){


    ariel::OrgChart org;
    while(true){
        cout<< "Please enter a command from the following list: \n"
            << "Press 1 to add_root <name> <parent_name>\n"
            << "Press 2 to add_child <name> <parent_name>\n"
            << "Press 3 to print the OrgChart\n"
            << "Press 4 to print the OrgChart in reverse level order\n"
            << "Press 5 to print the OrgChart in level order\n"
            << "Press 6 to print the OrgChart in pre order\n"
            << "Press 7 to quit\n";
            
        int i;
        cin >> i;
        if(i == 1){
            string root_name;
            cout<< " Please enter the name of the root: \n";
            cin >> root_name;
            org.add_root(root_name);
        }
        else if(i == 2){
            string name;
            string parent_name;
            cout<< " Please enter the name of the Parent: \n";
            cin >> parent_name;
            cout<< " Please enter the name of the Child: \n";
            cin >> name;
            org.add_sub(parent_name, name);
        }
        else if(i == 3){
            cout<< "The OrgChart is: \n";
            cout<<org<<endl;
        }
        else if(i ==4){
            for(ariel::Iterator it = org.begin_reverse_order(); it != org.reverse_order(); ++it){
                cout<< *it << " ";
            }
            cout<<endl;
        }
        else if(i == 5){

            for(ariel::Iterator it = org.begin_level_order(); it != org.end_level_order(); ++it){
                cout<< *it << " ";
            }
            cout<<endl;
        }
        else if( i == 6){
            for(ariel::Iterator it = org.begin_preorder(); it != org.end_preorder(); ++it){
                cout<< *it << " ";
            }
            cout<<endl;
        }
        else if(i == 7){
            cout << "Goodbye!" << endl;
            break;
        }
    
        else{

            cout << "Invalid command\n";
        }
        }





    }



