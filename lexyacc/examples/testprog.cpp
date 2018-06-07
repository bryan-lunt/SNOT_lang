//#define DEBUG 1

/*
MIT License

Copyright (c) 2017 Bryan Lunt <bjlunt2@illinois.edu> <bryan.j.lunt@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "param_storage.h"
#include <iostream>

using namespace gsparams;

int main(){

/*
    ParamMap *mymap = new ParamMap();
    ParamList *mylist = new ParamList();
    mymap->emplace("Foo", mylist);
    */

    /*
    DictList a_prim(1.5);

    DictList another_prim = a_prim;

    //std::cerr << "another prim : " << another_prim.my_type << " : " << another_prim.my_value << std::endl;

    DictList i_plan_to_use_as_a_list;

    std::cout << "going to push_back" << std::endl;
    i_plan_to_use_as_a_list.push_back(a_prim);

    std::cout << "size " << i_plan_to_use_as_a_list.size() << std::endl;

    i_plan_to_use_as_a_list.push_back(1.4);//Why does this trigger the copy constructor of a_prim?
                                        //Maybe because when the underlying vector is copied
    std::cout << "foobar" << std::endl << std::flush;
    std::cout << "size " << i_plan_to_use_as_a_list.size() << std::endl;
    i_plan_to_use_as_a_list.push_back(4.5);
    std::cout << "size " << i_plan_to_use_as_a_list.size() << std::endl;
    i_plan_to_use_as_a_list.push_back(3.5);
    std::cout << "size " << i_plan_to_use_as_a_list.size() << std::endl;

    std::cout << "size " << i_plan_to_use_as_a_list.size() << std::endl;

    DictList as_dict;

    as_dict.set("test",i_plan_to_use_as_a_list);
    as_dict.set("foobar",4.5656565);

    std::vector< double > gya;

    as_dict.traverse(&gya);

    std::cout << "traversal" << std::endl;
    for(int i = 0;i<gya.size();i++){
        std::cout << gya[i] << std::endl;
    }

    DictList as_dict_another = as_dict;

    as_dict_another.traverse(&gya);
    std::cout << "traversal" << std::endl;
    for(int i = 0;i<gya.size();i++){
        std::cout << gya[i] << std::endl;
    }



    std::cout << "Ensuring that deep copies are made." << std::endl;

    std::cout << as_dict.list_storage - as_dict_another.list_storage << std::endl;

    */

    DictList top_dict;

    DictList tfs;

    DictList a_tf;
    DictList b_tf;

    a_tf.push_back(1.23);
    a_tf.push_back(1.24);
    a_tf.push_back(1.25);
    tfs["A"] = a_tf;

    b_tf.push_back(2.1);
    b_tf.push_back(2.2);
    b_tf.push_back(2.3);


    tfs["B"] = b_tf;

    top_dict["tfs"] = tfs;


    std::cout << "Access test " << top_dict["tfs"]["A"][0] << std::endl;
    top_dict["tfs"]["A"][0] = 1.2345;
    std::cout << "Access test after alteration " << top_dict["tfs"]["A"][0] << std::endl;
    top_dict["tfs"]["A"][0] = 1.23;

    double foobar = 5.0*top_dict["tfs"]["A"][0];
    dictlist_primitive_t foobaz = 3.3/top_dict["tfs"]["A"][0];

    std::cout << "traversal" << std::endl;
    std::vector< double > gya;
    gya.clear();
    top_dict.traverse(&gya);
    for(int i = 0;i<gya.size();i++){
        std::cout << gya[i] << std::endl;
    }

    DictList::iterator myiter = top_dict.begin();
    std::cout << "Iterator in standard for loop" << std::endl;
    myiter = top_dict.begin();
    for(int i = 0;i<gya.size();i++){
        std::cout << "from iter: " << (*myiter).v() << std::endl;
        ++myiter;
    }

    std::cout << "Using iterators" << std::endl;
    myiter = top_dict.begin();
    std::cout << "Starting loop" << std::endl;
    for(DictList::iterator itr2 = top_dict.begin();itr2 != top_dict.end();++itr2){
        std::cout << "from iter: " << (*itr2).v() << std::endl;
    }

    std::cout << "Iterators assignable? (Should change the 4th one to 5.6789.)" << std::endl;

    myiter = top_dict.begin();
    for(int i = 0;i<gya.size();i++){
        if( i == 3){
            (*myiter) = 5.6789;
        }

        ++myiter;
    }

    for(DictList::iterator itr2 = top_dict.begin();itr2 != top_dict.end();++itr2){
        std::cout << "from iter: " << (*itr2).v() << std::endl;
    }


    std::cout << "Assign from a vector: 6 5 4 3 2 1" << std::endl;
    myiter = top_dict.begin();
    (*(myiter++)) = 6.0;
    (*(myiter++)) = 5.0;
    (*(myiter++)) = 4.0;
    (*(myiter++)) = 3.0;
    (*(myiter++)) = 2.0;
    (*(myiter++)) = 1.0;

    std::cout << "Now contains? " << std::endl;
    std::cout << "Using iterators" << std::endl;
    for(DictList::iterator itr2 = top_dict.begin();itr2 != top_dict.end();++itr2){
        std::cout << "from iter: " << (*itr2).v() << std::endl;
    }



    std::cout << "PATHS " << std::endl;
    for(myiter = top_dict.begin();myiter!=top_dict.end();++myiter){
	       std::cout << myiter.get_path() << std::endl;
    }

}
