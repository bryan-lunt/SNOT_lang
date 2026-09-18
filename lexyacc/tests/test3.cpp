#include <iostream>
#include <cassert>


//#include "param_storage.h"
#include "snot.h"

using namespace std;
using namespace gsparams;

//extern int yydebug;

int main(){
//    yydebug = 0;

DictList *retptr = new DictList();
  assert(gsparams::undecided == retptr->my_type);
  assert(-2 == retptr->size());
retptr->undecided_to_list_else_error();
  std::cerr << "Type of newly initialized empty DictList is " << retptr->my_type << std::endl<< std::flush;
  assert(gsparams::list == retptr->my_type);
  int temp_size = retptr->size();
  assert(gsparams::list == retptr->my_type); //did the type change just by asking the size?


  std::cerr << "Size of newly initialized empty DictList is " << retptr->size() << std::endl;
  assert(0 == retptr->size());

  assert(gsparams::list == retptr->my_type);

  std::cout << "Made it to the end of the program with no problems." << std::endl << std::flush;

    delete retptr;

    return 0;
}
