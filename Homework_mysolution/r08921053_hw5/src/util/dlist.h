/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <iostream>
using namespace std;

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (_node->_data); }
      T& operator * () { return (_node->_data); }
      iterator& operator ++ () { _node=_node->_next; return *(this); }
      iterator operator ++ (int) { iterator tmp = *this; _node=_node->_next; return tmp; }
      iterator& operator -- () { _node=_node->_prev; return *(this); }
      iterator operator -- (int) { iterator tmp = *this; _node=_node->_prev; return tmp; }

      iterator& operator = (const iterator& i) { _node=i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node!=i._node); }
      bool operator == (const iterator& i) const { return (_node==i._node); }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head); }
   iterator end() const {  return iterator(_head->_prev); }
   bool empty() const { return (_head->_prev==_head); }
   size_t size() const { 
       size_t i=0; 
	   DListNode<T>* tmp = _head;
       DListNode<T>* ending = _head->_prev;
       while(tmp != ending){
       	   ++i;
       	   tmp=(tmp->_next);
	      }
	   return i; 
	   }

   void push_back(const T& x) { 
       bool size0 = false;
	   if((_head->_prev) == _head) size0 = true;
	   
	   if(size0 == false){
			DListNode<T>*  dummy = _head->_prev;
			DListNode<T>*  last  = dummy->_prev;
	   	    DListNode<T>* fresh  = new DListNode<T>(x, last, dummy);
			last->_next  = fresh;
			dummy->_prev = fresh; 
	   }
	   else{ //In this case, size = 0
	       DListNode<T>*  dummy = _head;
	       DListNode<T>*  fresh = new DListNode<T>(x, _head, _head);
	       dummy->_prev = fresh;
	       dummy->_next = fresh;
	       _head        = fresh;	
	   }  
   }

   void pop_front() { 
       bool size0 = false, size1 = false;
       DListNode<T>* dummy = _head->_prev;
       if(dummy == _head)               size0 = true;
       else if( dummy->_prev == _head ) size1 = true;
       
	   if(size0 == false && size1 == false){
       	   DListNode<T>* num2  = _head->_next;
       	   DListNode<T>* dummy = _head->_prev;
       	   num2->_prev  = dummy;
       	   dummy->_next = num2;
       	   
       	   delete _head;
	   _head = num2; 
	   }
	   else if(size0 == false && size1 == true){
	   	   DListNode<T>* dummy = _head->_prev;
	   	   dummy->_next = dummy;
	   	   dummy->_prev = dummy;
	   	   
	   	   delete _head;
	   	   _head       = dummy;
	   }
	   else if(size0 == true && size1 == false){
	   	   //size = 0, we can not delete anything
	    }
	   else{
	   	  //It is impossible that size0 = true = size1
	   }
   }
   void pop_back() { 
       bool size0 = false, size1 = false;
       DListNode<T>* dummy = _head->_prev;
	   if(dummy == _head)             size0 = true;
	   else if(dummy->_prev == _head) size1 = true;
	   
	   if(size0 == false && size1 == false){
	   	    DListNode<T>* dummy = _head->_prev;
	   	    DListNode<T>* last  = dummy->_prev;
	   	    DListNode<T>* seclt =  last->_prev;
	   	    seclt->_next = dummy;
	   	    dummy->_prev = seclt; 
	   	    
			delete last;
			last = 0;
	   }
	   else if(size0 == false && size1 == true){
	   	    DListNode<T>* dummy = _head->_prev;
	   	    dummy->_next = dummy;
	   	    dummy->_prev = dummy;
	   	    
	   	    delete _head;
	   	    _head       = dummy;
	   }
	   else if(size0 == true && size1 == false){
	   	    //size = 0, we can not delet anything
	   }
	   else{
	   	    //It is impossible that size0 = true = size1
	   }
   
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
   	    DListNode<T>* zero = 0;
   	    DListNode<T>* dummy = _head->_prev;
   	    size_t size0 = false, size1 = false;
   	    if(dummy == _head)             size0 = true;
   	    else if(dummy->_prev == _head) size1 = true;
   	    
   	    if(size0 == true){ 
		    return false;
		    }
   	    else if(size1 == true){
   	    	if(pos == _head){
   	    		dummy->_next = dummy;
   	    		dummy->_prev = dummy;
   	    		
   	    		delete pos._node;
   	    		pos._node = 0;
   	    		_head        = dummy;
   	    	    return true;
			   }
			return false;
		   }
		else{ //In this case, size >= 2 
			if(pos._node == _head){
				pop_front();
				return true;
			}
			else if(pos._node == dummy||pos._node == zero){
				return false;
			}
			else{
				DListNode<T>* right = (pos._node)->_next;
				DListNode<T>* left  = (pos._node)->_prev;
				right->_prev = left;
				left->_next  = right;
				
				delete pos._node;
				pos._node = 0;
				return true;
			}		
		}	    
	}
   bool erase(const T& x) { 
       iterator given = find(x);
       if(given == end()) return false;
       else               return erase(given);
    }

   iterator find(const T& x) { 
       iterator  tmp    = begin();
       iterator  ending = end();
       while(tmp!=ending){
       	   if( (tmp._node)->_data == x ) return tmp;
       	   ++tmp; 
	       }
       return ending; 
	   }

   void clear() { 
       DListNode<T>* dummy = _head->_prev;
       //DListNode<T>* tmp   = dummy;
       iterator i = begin();
       iterator ending = end();
       while(i!=ending){
       	    DListNode<T>* tmp = i._node;
       	    ++i;
       	    delete tmp;
       	    tmp = 0;
	   }
       _head = dummy;
       _head->_next = dummy;
       _head->_prev = dummy;
       
   }  // delete all nodes except for the dummy node  

   void sort() {
   	   size_t length = size();
   	   for(size_t i=0; i<length; ++i){
   	   	    iterator bigst = begin(); 
   	        iterator go    = bigst  ;
		    for(size_t j=0; j<(length-i); ++j){
		    	if(bigst._node->_data < go._node->_data) bigst = go;
		    	++go;
			}
			//_node[length-i-1] <-> _node(small) switch
			--go;
			T tmp = go._node->_data;
			go._node->_data = bigst._node->_data;
			bigst._node->_data = tmp;
		  }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
