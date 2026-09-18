#ifndef GS_PARAM_STORAGE
#define GS_PARAM_STORAGE

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

#include <cstddef>
#include <stdexcept>
#include <vector>

/***
WARNING TO NEXT POOR SAP (probalby me):

I thought that we wanted to use the `unordered_map` if it's available.

We DO NOT want to use the `unordered_map` it inexplicably causes the parser to show
absolutely weird behaviour that makes no sense. I lost an entire day on this.
Sanity checkers don't find the problem!
Debuggers show only that somehow a recently promoted DictList(acting as a list)
reverts to being an `undefined`, and that reversion only happens when you call
the `DictList::size()` function,
and ONLY in the context of being inside a flex/bison parser.

The same code has no problem in isolation.

YOU HAVE BEEN WARNED!
*/
//#if __cplusplus >= 201103L
//    #include <unordered_map>
//    #define DICTLIST_BASIC_MAP_TEMPLATE std::unordered_map
//#else
    #include <map>
    #define DICTLIST_BASIC_MAP_TEMPLATE std::map
//#endif

#include <string>
#include <stack>
#include <sstream>

#include <cassert>

#include <iostream>

namespace gsparams {

namespace __detail {
  //Private namespace
  template<typename K, typename V>
  class DictListStorage_MIXIN {
    /*
    You may think that I should have just implemented an isertion-ordered-map
     and used that in the DictList, but this version may have some efficiency gains.
    Because sometimes we switch between being a list and being a dictionary.
    This lets us get the items at full speed of a vector instead of needing to do several dictionary lookups.
    For our usecase this will be faster. We do the traversals often.

    Usually we will want the items in order and all at once, more often that we're going to do dictionary lookup.
    NOTE: Is that true?
    */
    public:
      typedef std::size_t size_type;
      typedef DICTLIST_BASIC_MAP_TEMPLATE<K,size_type> map_storage_type; //DO NOT change to `std::unordered_map` , see warning above.
      typedef std::vector<K> map_key_storage_type;
      typedef std::vector<V> list_storage_type;
    protected:
      map_storage_type map_storage; //Keys to locations in the list.
      map_key_storage_type map_key_storage; //Keys in insertion order.
      list_storage_type list_storage; //only primitive values lack list storage.
    public:
      //TODO: Move all the lookup and storage code to here so that we have the option to play with it.
      /*
      inline void insert(const K& key, const V& value){
        typename DICTLIST_BASIC_MAP_TEMPLATE<K,size_type>::iterator iter = map_storage.find(key);
        if(iter == map_storage.end()){ //key alredy existed in map

        }else{//inserting a new value into the map

        }
      }
      */

      inline void clear(){
          //don't user .clear()
          this->list_storage = list_storage_type();
          this->map_key_storage = map_key_storage_type();
          this->map_storage = map_storage_type();
      }

      inline size_type size() const{
        return this->list_storage.size();
      }
  };
} //END of namespace __detail

#define dictlist_default_primitive_t double
#define dictlist_primitive_t double
//#define dictlist_key_t std::string
typedef std::string dictlist_key_t;

typedef enum { undecided=0, primitive=1, dict=2, list=3} DictListType;

template<typename T> class DictList_BASE;
template<typename T> inline std::ostream& operator<<(std::ostream& os, const DictList_BASE<T>& obj);

template<typename T = dictlist_primitive_t>
class DictList_BASE : public __detail::DictListStorage_MIXIN<dictlist_key_t, DictList_BASE<T> >{
    public:
    //private:
    //public:

        typedef __detail::DictListStorage_MIXIN<dictlist_key_t, DictList_BASE<T> > base_type;
        typedef typename base_type::size_type size_type; //our own size type, syntactic sugar.
        class iterator; //Nested iterator class, forward declared.
        typedef typename base_type::map_storage_type dictlist_map_t;

        //member variables.
        DictListType my_type;
        T my_value;
        //dictlist_map_t map_storage;
        //std::vector<dictlist_key_t> map_key_storage; //when we want to output, we need to be able to iterate over keys too.
        //td::vector<DictList_BASE<T> > list_storage; //only primitive values lack list storage.

        inline void clear_helper(){
            this->base_type::clear();
            //this->my_value = 0.0;
            //this->my_type = undecided;
        }

        inline void copy_helper(const DictList_BASE<T>& other){
          if(this == &other){
            return;//no need to copy.
          }

            this->my_type = other.my_type;
            this->my_value = other.my_value;

            this->list_storage = typename base_type::list_storage_type(other.list_storage);
            this->map_key_storage = typename base_type::map_key_storage_type(other.map_key_storage);
            this->map_storage = typename base_type::map_storage_type(other.map_storage);

        }

        inline void undecided_to_dict_else_error(){
            if(this->my_type == gsparams::dict){return;} // already a dictionary.

            if(this->my_type == undecided){
                this->clear_helper();//should be unnecessary
                this->my_type = gsparams::dict;
            }else{
                throw std::runtime_error("Already upgraded to a different container or leaf type");
            }

            assert(this->my_type == gsparams::dict);
        }

        inline void undecided_to_list_else_error(){
            //std::cerr << "Promoting an undecided DictList to a list." << std::endl;
            if(this->my_type == gsparams::list){return;} //already a list

            if(this->my_type == gsparams::undecided){
                this->clear_helper();//should be unnecessary
                this->my_type = gsparams::list;
            }else{
                throw std::runtime_error("Already upgraded to a different container or leaf type");
            }

            assert(this->my_type == gsparams::list);
        }

    //protected:
        inline void traverse_internal(std::vector< T >* target) const {
            //public function has already cleared and setup the beginnigs of the target vector.
            if(undecided == this->my_type){
                return;
            }

            if(primitive == this->my_type){
                target->push_back(this->v());
                return;
            }

            size_type n = this->list_storage.size();
            for(size_type i = 0; i < n; i++){
                this->list_storage.at(i).traverse_internal(target);
                //shoudl be able to overwrite some operator with an iterator and
            }
        }

        inline size_type populate_internal(const std::vector< T >& target,const size_type starting_at) {
            //public function has already cleared and setup the beginnigs of the target vector.
            if(undecided == this->my_type){
                return 0;
            }

            if(primitive == this->my_type){
                this->my_value = target[starting_at];
                return 1;
            }

            //some type that stores things

            size_type num_consumed = 0;
            size_type n = this->list_storage.size();
            for(size_type i = 0; i < n; i++){
                num_consumed += this->list_storage.at(i).populate_internal(target,starting_at+num_consumed);
            }
            return num_consumed;
        }

    public:
        inline DictList_BASE() : base_type(), my_type(gsparams::undecided), my_value(0.0) {
            this->my_type = gsparams::undecided;
            this->my_value = 0.0;//TODO: can we make this nan or some other defensive value?
        }
        inline DictList_BASE(const T& in) : base_type(), my_type(gsparams::primitive), my_value(in)  {
            this->my_type = gsparams::primitive;
            this->my_value = in;
        }
        //copy constructor

        inline DictList_BASE(const DictList_BASE& other) : base_type(), my_type(other.my_type), my_value(other.my_value)  {

            this->copy_helper(other);
            this->my_type = other.my_type;
            this->my_value = other.my_value;

        }


        inline ~DictList_BASE(){
        }

        inline DictList_BASE& operator=(const DictList_BASE &other){
            if(this != &other){
              this->copy_helper(other);
              this->my_type = other.my_type;
              this->my_value = other.my_value;
            }
            return *this;
        }

        inline DictList_BASE& operator=(const T& value){
            if(!(undecided == this->my_type || primitive == this->my_type)){
                throw std::runtime_error("Attempt to assign primitive value to nonprimitive, non undecided DictList.");
            }

            assert(this->size() == -2 || this->size() == 1);//undecided or primitive

            //Don't need this because we already know that the thing is empty.
            //this->clear_helper();//You could assign something to equal a value stored in itself and if so, it could cause problems?

            this->my_type = primitive;
            this->my_value = value;

            return *this;
        }



        inline T v() const {
            if(this->my_type != primitive){throw std::runtime_error("Asked value of non primitive");}
            return this->my_value;
        }

        /*
        * functions and operators for list-like behaviour.
        */
        inline void push_back(const DictList_BASE& in) {
            this->undecided_to_list_else_error();
            assert(this->my_type == gsparams::list);
            assert(0 <= this->size());



            //TODO: scan ourselves to see that we don't already contain this value? Or make a copy?

            switch(this->my_type){
                case gsparams::primitive:
                    throw std::runtime_error("Cannot append to a primitive");
                    break;
                case gsparams::undecided:
                    throw std::runtime_error("still undecided even after asserts");
                    break;
                case gsparams::dict:
                    throw std::runtime_error("Cannot append to a dict as though it were a list");//because no key.
                    break;
                case gsparams::list:
                    //DictList_BASE tmp_element(in);//should use copy assignment?
                    //this->list_storage.push_back(tmp_element);
                    this->list_storage.push_back(in);
                    break;
                default:
                    throw std::runtime_error("somethind strange.");
            }
        }

        inline void push_back(const T& in){
            #ifdef GS_PARAM_STORAGE_DEBUG
            std::cerr << "append from value" << std::endl;
            #endif
            //relies on the other version for sanity checking.
            DictList_BASE indictlist(in);
            this->push_back(indictlist);

            assert(gsparams::list == this->my_type);
        }

        inline DictList_BASE& at(const size_type location) {
            switch(this->my_type){

                case gsparams::list:
                case gsparams::dict:
                    //in both of these cases, we access as a list.
                    return this->list_storage.at(location);//vector::at() is in C++98, keep
                    break;
                case gsparams::primitive:
                    throw std::runtime_error("Can't subscript a primitive value.");
                    break;
                case gsparams::undecided:
                default:
                    throw std::runtime_error("Cant get values from undecided dictlist.");
            }
            throw std::runtime_error("Cant get values from undecided dictlist.");
            //compiler warning
        }


        inline DictList_BASE& operator[](const size_type location) {
            return this->at(location);
        }
        /*
        *END of list-like behaviours
        */


        /*
        *functions and operators for map/dictionary -like behaviour
        */
        inline void set(dictlist_key_t key, const DictList_BASE& in){
            this->undecided_to_dict_else_error();
            assert(gsparams::dict == this->my_type);
            if(gsparams::dict != this->my_type){throw std::runtime_error("Not a dictionary.");}

            //What if the value already exists?
            typename dictlist_map_t::iterator key_to_int = this->map_storage.find(key);

            //TODO: move this to the storage mixin.
            if(this->map_storage.end() == key_to_int){
                //key does not exist
                this->list_storage.push_back(in);
                this->map_key_storage.push_back(key);
                this->map_storage[key] = (int)(this->list_storage.size()-1);
            }else{
                //The key already exists, so we need to replace it.
                this->list_storage[key_to_int->second] = in;
            }
        }

        inline void set(dictlist_key_t key, T in){
            DictList_BASE indictlist(in);
            this->set(key,indictlist);
        }

        inline DictList_BASE& at(dictlist_key_t key) {
            if(gsparams::dict != this->my_type){
                throw std::runtime_error("Can't us this as a dictionary.");
            }
            typename dictlist_map_t::iterator key_to_int = this->map_storage.find(key);
            if(this->map_storage.end() == key_to_int){
                throw std::out_of_range("Asked for a key that does not exist.");
            }
            return this->list_storage.at(key_to_int->second);//vector::at() in C++98
        }

        inline DictList_BASE& operator[](dictlist_key_t key) {
            this->undecided_to_dict_else_error();
            typename dictlist_map_t::iterator key_to_int = this->map_storage.find(key);
            if(this->map_storage.end() == key_to_int){
                //
                DictList_BASE tmp;
                this->set(key,tmp);//should insert into the key list if necessary

                //Try again using the same search code.
                key_to_int = this->map_storage.find(key);
                if(this->map_storage.end() == key_to_int){
                    throw std::out_of_range("this function should have created a new one...");
                }
            }

            return this->list_storage.at(key_to_int->second);
        }

        /*
        //NOTE: I don't know if this is needed, it definitely breaks in new C++ but does it work in old C++?
        inline DictList_BASE<T>& operator[](const char* key){
            return this->operator[](dictlist_key_t(key));
        }
        */

        /*
        *END functions and operators for map/dictionary -like behaviour
        */

        inline int size() const {
            //std::cerr << "Asked the size of " << (this) << " which is of type " << this->my_type << " ." << std::endl << std::flush;

            switch(this->my_type){
                case gsparams::undecided:
                    return -2;
                    break;
                case gsparams::primitive:
                    return 1;
                    break;
                case gsparams::list:
                case gsparams::dict:
                    return (int)this->base_type::size();
                default:
                    break;
            }

            //Should and must never reach here.
            //TODO: Consider adding an assert, an exception may make it too much slower.
            assert(false);
            return -3;
        }

        /*
        * Fill the vector target with the leaf values from this tree.
        *
        * Target must store the correct type. It's up to the user to ensure that.
        */
        inline void traverse(std::vector< T >* target) const {
            if(undecided == this->my_type){
                throw std::runtime_error("Can't traverse undecided");
            }

            target->clear();

            this->traverse_internal(target);
        }

        /*
        * Fill the leaves of this tree with values from the vector source.
        */
        inline void populate(const std::vector< T >& source) {
            size_type num_consumed = populate_internal(source,0);
            //TODO: do I want to check that the number consumed is right?
        }

        inline bool populate_or_revert(const std::vector< T >& source) {
            std::vector< T > tmp(0);
            this->traverse(&tmp);

            try{
                this->populate(source);
                return true;
            }catch(std::range_error e){
                this->populate(tmp);
                return false;
            }

            return false;
        }

        /**
        *   Create a new DictList_BASE hierarchy using values from another, but ordering according to this.
        *
        *   The strong precondition is that they both have the same tree hierarchy, but not necessarily the same order.
        *   Leaves must retain the type of the other. This object is only acting as the template.
        */
        template<typename U>
        inline DictList_BASE<U> use_as_prototype(DictList_BASE<U> &other){
            DictList_BASE<U> ret_list;

            //Check sanity & compatibility
            //TODO: Prevent an uninitialized object from acting as a template?
            if(this->my_type != other.my_type) { throw std::runtime_error("Could not use as prototype (mismatched types) "); }
            if(this->size() != other.size()) { throw std::runtime_error("Could not use as prototype (mismatched length) "); }
            /*
            * NOTE: PERFORMANCE: It would be tempting to make this do all sanity checking and tree matching each time. In real use you can't afford that time penalty.
            */


            //passed compatibility checking

            if(undecided == this->my_type || primitive == this->my_type){
                ret_list = DictList_BASE<U>(other);
                return ret_list;
            }

            if(list == this->my_type){
                //TODO: We can't actually enforce the order of a list if we don't know what the leaf values are
                //TODO: If the list contains dictionaries and things, should we enforce matching?
                for(size_type i = 0;i<this->list_storage.size();i++){
                    ret_list.push_back(this->at(i).use_as_prototype(other.at(i)));
                }

                return ret_list;
            }

            if(dict == this->my_type){
                for(size_type i = 0;i<this->map_key_storage.size();i++){
                    std::string the_key = this->map_key_storage.at(i);
                    DictList_BASE<U> blah = this->at(the_key).use_as_prototype(other.at(the_key));
                    ret_list.set(the_key, blah);
                }

                return ret_list;
            }

            throw std::logic_error("Should not make it to the bottom of DictList_BASE::use_as_prototype.");
            return ret_list;
        }

    inline iterator begin(){
        iterator ret_iter(this,-1);
        ++ret_iter;
        return ret_iter;
    }

    inline iterator end(){
        iterator ret_iter(this,this->size());
        ++ret_iter;
        return ret_iter;
    }

    /*
    Unfortunately, it looks like we can't have typecasting and have nice subscripting at the same time.

    We can, by virtue of altering the subscript operator, which somehow makes that take presedence over this cast.
    Thanks stack overflow! : https://stackoverflow.com/questions/15850840/ambiguous-overload-for-operator-if-conversion-operator-to-int-exist

    The overloaded subscript is next to the other subscript.
    */

    inline operator T() const {
        if(primitive != this->my_type){ throw std::runtime_error("Cannot cast non-primitive.");}
        return this->v();
    }

    friend std::ostream& operator<< <T>(std::ostream& os, const DictList_BASE<T>& obj);

    inline std::string str(){
        std::stringstream ss;
        ss.clear();
        ss << *this;
        std::string foobar(ss.str());
        return foobar;
    }

};//END OF DICTLIST

template<typename T>
class DictList_BASE<T>::iterator : public std::forward_iterator_tag {
    protected:
        /*This iterator will need some kind of stack for state storage.
        The things it iterates over each provide iterators, so maybe the natural thing is to have a stack of iterators?
        It might not be storage efficient, but it's programmer time efficient.

        Duh. No, we don't need an explicit stack. The iterator knows which DictList_BASE it iterates, and has a handle to a current sub-iterator.
        The handles to handles to handles of sub-iterators _are_ the stack.

        Blah. That's just reasoning to avoid learning more STL, it will create a lot of memory derefrences.
        Yes, this library won't be used in really slow parts of the code, but still...
        */
        DictList_BASE<T>* my_dictlist;
        std::stack< std::pair<DictList_BASE<T>*, int> > my_stack;
        //std::vector<std::vector<DictList_BASE<T>>::iterator> position_stack;

    public:
        inline iterator(DictList_BASE<T> *initial_dictlist, int position) : my_dictlist(initial_dictlist), my_stack(){
            my_stack.push(std::make_pair(initial_dictlist,position));
        }

    public:

        inline iterator(const DictList_BASE<T>::iterator &other) : my_dictlist(other.my_dictlist), my_stack(other.my_stack) {
            //pass
        }

        inline DictList_BASE<T>::iterator& operator=(const DictList_BASE<T>::iterator &other){
            my_dictlist = other.my_dictlist;
            my_stack = other.my_stack;
            return *this;
        }

        inline bool operator==(const DictList_BASE<T>::iterator &other) const{
            return (my_dictlist == other.my_dictlist && my_stack == other.my_stack);
        }

        inline bool operator!=(const DictList_BASE<T>::iterator &other) const{
            return !(my_dictlist == other.my_dictlist && my_stack == other.my_stack);
        }

        inline DictList_BASE<T>::iterator& operator++(){
            //TODO: The meat of the traversal algorithm
            if(my_stack.size() < 1){ //off the end.
                //std::cerr << "off end." << std::endl;
                return *this;
            }

            /* COROUTINE-like code.
            So, we really want a coroutine here, and we'd like it to be recursive.
            That's unrolled and done below. Pseudocode:

            traverse(X):
                for o in X:
                    if( o is container ):
                        traverse(o)
                    elif( o is primitive ):
                        yield o
                    elif( o is undef ):
                        continue #skip over these

            */

            bool loop_continues = true;
            while(my_stack.size() > 0 && loop_continues){
                loop_continues = false;
                my_stack.top().second++;
                if(my_stack.top().second >= my_stack.top().first->size()){
                    //primitives will never be on the stack, so this is ok.
                    //The stack is only those containers that we have descended into.
                    //So, I guess that implies that iterators for undecided and primitive types need to be initialized to .end();
                    my_stack.pop();
                    loop_continues = true;
                    continue;
                }

                //We're looking at the next subobject of the thing on top of the stack.
                //If it is another compound object, we need to descend into it( put it on top of the stack. )
                DictList_BASE<T> *current_pointed_element = &(my_stack.top().first->at(my_stack.top().second));
                DictListType check_type = current_pointed_element->my_type;

                //some compound type, gets pushed to the stack, equivalent to recursion
                if(undecided != check_type && primitive != check_type){
                    my_stack.push(std::make_pair(current_pointed_element,-1));
                    loop_continues = true;
                    continue;//need to descend into that.
                }

                if(undecided == check_type){
                    //Yes, iterators now iterate over undecided.
                    loop_continues = false;
                    break;
                }
                if(primitive == check_type){
                    loop_continues = false;
                    break;
                }

                throw std::runtime_error("Never make it here.");
            }
            /*Otherwise, we are pointing to a primitive type (or undecided, behaviour not defined)
            */

            return *this;
        }

        inline DictList_BASE<T>::iterator operator++(int unused){
            DictList_BASE<T>::iterator tmp(*this);
            this->operator++();
            return tmp;
        }

        inline DictList_BASE<T>& operator*(){

            //Actually, the increment should handle this.
            DictList_BASE<T>& current_pointed_element = my_stack.top().first->at(my_stack.top().second);

            if(primitive == current_pointed_element.my_type){
                return current_pointed_element;//TODO: need to check that this actually returns a reference, rather than creating a new one with copy construction.
            }

            //DEBUG
            //std::cerr <<


            throw std::runtime_error("Invalid iterator state.");
        }

        /**
        * Return the JSONPath to the current position of this iterator.
        */
        inline std::string get_path(){
            std::string path_str = "";
            if(my_stack.size() <= 0){
                return std::string("$");
            }

            //We can't access inside the stack easily.
            //I'd be tempted to change the stack storage to a vector,
            //but looking inside the stack probably doesn't happen often.

            std::stack< std::pair<DictList_BASE<T>*, int> > tmp_stack(my_stack);

            while(tmp_stack.size() != 0){

                DictList_BASE<T> *top_container = tmp_stack.top().first;
                int top_index = tmp_stack.top().second;
                tmp_stack.pop();


                if(list == top_container->my_type){
			std::ostringstream int_to_str_ss;
			int_to_str_ss << top_index;
                    path_str = std::string("[") + int_to_str_ss.str() + std::string("]")  + path_str;//Deliberate choice to not use std::to_string.
		    //std::to_string can corrupt values in favor of prefering to write out the decimal point instead of using scientific notation.
                }else if(dict == top_container->my_type){
                    path_str = std::string("[\"") + top_container->map_key_storage.at(top_index) + std::string("\"]")  + path_str;

                }else{
                    throw std::runtime_error("Somehow tried to get a path from invalid iterator");
                }


            }

            path_str = "$" + path_str;

            return path_str;
        }

};//End of declaration of iterator

template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const DictList_BASE<T>& obj)
    {

        if(gsparams::undecided == obj.my_type){
            os << "null";
            return os;
        }

        // write obj to stream
        if(gsparams::primitive == obj.my_type){
            os << obj.v();
            return os;
        }

        if(gsparams::dict == obj.my_type){
            os << "{";
                int n_members = obj.size();
                if(n_members > 0){
                    os << '"' << obj.map_key_storage.at(0) << '"' << ":" << obj.list_storage.at(0);

                    for(int i = 1;i<n_members;i++){
                        os << "," << '"' << obj.map_key_storage.at(i) << '"' << ":" << obj.list_storage.at(i);
                    }

                }
            os << "}";
            return os;
        }else if(gsparams::list == obj.my_type){
            os << "[";
            int n_members = obj.size();
            if(n_members > 0){
                os << obj.list_storage.at(0);

                for(int i = 1;i<n_members;i++){
                    os << "," << obj.list_storage.at(i);
                }

            }

            os << "]";
        }else if(gsparams::primitive == obj.my_type){
            os << obj.v();
        }


        return os;
    }


typedef DictList_BASE<dictlist_default_primitive_t> DictList;

} //END of Namespace


#endif
