#include <iostream>
#include <cassert>


//#include "param_storage.h"
#include "snot.h"

using namespace std;
using namespace gsparams;

//extern int yydebug;

int main(){
//    yydebug = 0;


    std::cout << "Test 1: empty array" << std::endl << std::flush;
    string empty_array_str=R"([])";
    DictList empty_array_dl = parse_dictlist(empty_array_str);
    assert(empty_array_dl.my_type == gsparams::list);
    std::cout << "Test 1: Passed? " << empty_array_dl << std::endl<< std::flush;

    std::cout << "Test 2: one element array" << std::endl;
    string one_element_array_str=R"([ 1.0 ])";
    DictList one_element_array_dl = parse_dictlist(one_element_array_str);
    //one_element_array_dl.my_type = gsparams::list;

    std::cout << "Test 2: Passed " << one_element_array_dl << std::endl;

    std::cout << "Test 3: two element array" << std::endl;
    string two_element_array_str=R"([1.0,4.5])";
    DictList two_element_array_dl = parse_dictlist(two_element_array_str);
    std::cout << "Test 3: Passed " << two_element_array_dl << std::endl;



    string testin="{\"tfs\":{\"A\":[1.1,1.2,1.3],\"B\":[2.1,2.2,2.3],\"C\":[3.1,3.2,3.3,3.4,3.5,3.6]}}";

    DictList my_dictlist = parse_dictlist(testin);


        std::cout << "About to traverse the parsed object" << std::endl;
        std::vector<double> thevals(0);

        my_dictlist.traverse(&thevals);
        for(int i = 0;i<thevals.size();i++){
            cout << thevals[i] << " ";
        }
        thevals.clear();
        std::cout << std::endl;


        std::cout << "Oiginally parsed string is:" << std::endl << "```" << std::endl << my_dictlist.str() << std::endl << "```" <<std::endl;


    return 0;
}
