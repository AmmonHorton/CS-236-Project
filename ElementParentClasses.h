//
//  ElementParentClasses.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 2/15/21.
//

#ifndef ElementParentClasses_h
#define ElementParentClasses_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <set>

class ProductionElementBase {
private:
    string selfType;
public:
    string data;
    ProductionElementBase(string st = "ProductionElementBase", string dt = "") {
        selfType = st;
        data = dt;
    }
    virtual ~ProductionElementBase() {}
    bool operator==(const ProductionElementBase& tc) {return this->selfType == tc.selfType;}
    
    virtual string str() const {
        return (data);
    }
    
    virtual string st() {
        return (selfType);
    }
    
    
    friend ostream & operator << (ostream &os, ProductionElementBase &pEB) {
        os << pEB.str();
        return os;
    }
    virtual bool isValid(deque<ProductionElementBase*> &tokenDeque) {return true;}
    virtual deque<string*> GetDomain() {
        deque<string*> dq;
        return dq;
    }
};

class ProductionElement : public ProductionElementBase{
protected:
    ProductionElementBase* elementPtr;
public:
    ProductionElement(string st = "ProductionElement") : ProductionElementBase(st) {}
    ~ProductionElement() {
        if(elementPtr) {
            delete elementPtr;
            elementPtr = NULL;
        }
    }
    bool CheckForToken(deque<ProductionElementBase*> &tokenDeque, ProductionElementBase* &fsaPtr) {
        if(*tokenDeque.front() == *elementPtr) {
            if(fsaPtr != NULL) {
                delete fsaPtr;
                fsaPtr = tokenDeque.front();
                tokenDeque.pop_front();
                if(elementPtr) delete elementPtr;
                elementPtr = NULL;
                return true;
            }
            else {
                delete tokenDeque.front();
                tokenDeque.pop_front();
                delete elementPtr;
                if(elementPtr) elementPtr = NULL;
                return true;
            }
        }
        else {
            if(fsaPtr != NULL) {
                delete fsaPtr;
                fsaPtr = NULL;
            }
            delete elementPtr;
            if(elementPtr) elementPtr = NULL;
            return false;
        }
    }
    string str() const {
        return "";
    }
};

class ProductionElementLists {
protected:
    ProductionElementBase* elementPtr;
    ProductionElementBase* listElementPtr;
    ProductionElementBase* followSetItemPtr;
public:
    deque<ProductionElementBase*> elementListDeque;
    ~ProductionElementLists() {
        if(elementPtr) {
            //delete elementPtr;
            elementPtr = NULL;
        }
        //delete listElementPtr;
        //delete followSetItemPtr;
        for(size_t i = elementListDeque.size(); i > 0; i--) {
            //delete elementListDeque.back();
            elementListDeque.pop_back();
        }
    }
    
    bool CheckForToken(deque<ProductionElementBase*> &tokenDeque) {
        if(*tokenDeque.front() == *elementPtr) {
            if(*listElementPtr == *elementPtr) {
                elementListDeque.push_back(tokenDeque.front());
                tokenDeque.pop_front();
                if(elementPtr) delete elementPtr;
                elementPtr = NULL;
                return true;
            }
            else {
                delete tokenDeque.front();
                tokenDeque.pop_front();
                if(elementPtr) delete elementPtr;
                elementPtr = NULL;
                return true;
            }
        }
        else {
            if(elementPtr) delete elementPtr;
            elementPtr = NULL;
            return false;
        }
    }
    
    size_t size() const {
        return elementListDeque.size();
    }
    string str() const {
        ostringstream os;
        /*deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            os << "," << **it++;
        }*/
        for(size_t i = 0; i < elementListDeque.size(); i++) {
            os << *elementListDeque[i] << endl;
        }
        return os.str();
    }
    
    ProductionElementBase* operator[](size_t index) const {
        if (index >= size()) {
                cout << "Array index out of bound, exiting";
                return NULL;
            }
        return elementListDeque[index];
    }
    
};

#endif /* ElementParentClasses_h */
