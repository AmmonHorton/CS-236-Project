//
//  Elements.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 3/8/21.
//

#ifndef Elements_h
#define Elements_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include "ElementParentClasses.h"

class IDList : public ProductionElementLists {
public:
    IDList() {
        elementPtr = NULL;
        listElementPtr = new ProductionElementBase("ID");
        followSetItemPtr = new ProductionElementBase("RIGHT_PAREN");
    }
    ~IDList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("COMMA");
        if(*productionElementsDeque.front() == *elementPtr) {
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            //Check for all ID's
            
            elementPtr = new ProductionElementBase("ID");
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() const {
        ostringstream os;
        /*deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            os << "," << **it++;
        }*/
        for(size_t i = 0; i < elementListDeque.size(); i++) {
            os << "," << *elementListDeque[i];
        }
        return os.str();
    }
};


class StringList : public ProductionElementLists {
public:
    StringList() {
        elementPtr = NULL;
        listElementPtr = new ProductionElementBase("STRING");
        followSetItemPtr = new ProductionElementBase("RIGHT_PAREN");
    }
    ~StringList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("COMMA");
        if(*productionElementsDeque.front() == *elementPtr) {
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            //Check for all Strings
            
            elementPtr = new ProductionElementBase("STRING");
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() const {
        ostringstream os;
        /*deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            os << "," << **it++;
        }*/
        for(size_t i = 0; i < elementListDeque.size(); i++) {
            os << "," << *elementListDeque[i];
        }
        return os.str();
    }
    deque<string*> GetDomain() {
        deque<string*> domainDeque;
        deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            ostringstream os;
            os << **it++;
            string* strPtr = new string(os.str());
            domainDeque.push_front(strPtr);
        }
        return domainDeque;
    }
    
};


class Operator : public ProductionElement{
public:
    ProductionElementBase* addToken;
    ProductionElementBase* multiplyToken;
    IDList idList;
    Operator(string st = "OPERATOR") : ProductionElement(st){
        elementPtr = NULL;
        addToken = new ProductionElementBase("ADD");
        multiplyToken = new ProductionElementBase("MULTIPLY");
    }
    ~Operator() {
        if(addToken) delete addToken;
        if(multiplyToken) delete multiplyToken;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        if(*productionElementsDeque.front() == *addToken) {
            delete multiplyToken;
            multiplyToken = NULL;
            
            elementPtr = new ProductionElementBase("ADD");
            if(CheckForToken(productionElementsDeque, addToken)) return true;
        }
        else if(*productionElementsDeque.front() == *multiplyToken) {
            delete addToken;
            addToken = NULL;
            
            elementPtr = new ProductionElementBase("MULTIPLY");
            if(CheckForToken(productionElementsDeque, multiplyToken)) return true;
        }
        return false;
    }
    string str() const {
        if(multiplyToken) return multiplyToken->str();
        else return addToken->str();
    }
};



class Parameter : public ProductionElement {
public:
    ProductionElementBase* expressionToken;
    ProductionElementBase* strTokenPtr;
    ProductionElementBase* idTokenPtr;
    Parameter(string st = "PARAMETER") : ProductionElement(st){
        elementPtr = NULL;
        expressionToken = NULL;
        strTokenPtr = new ProductionElementBase("STRING");
        idTokenPtr = new ProductionElementBase("ID");
    }
    ~Parameter() {
        if(expressionToken) delete expressionToken;
        if(strTokenPtr) delete strTokenPtr;
        if(idTokenPtr) delete idTokenPtr;
    }
    
    class Expression : public ProductionElement {
    public:
        ProductionElementBase* parameterToken;
        ProductionElementBase* operatorToken;
        ProductionElementBase* scndParameterToken;
        Expression(string st = "EXPRESSION") : ProductionElement(st){
            elementPtr = NULL;
            parameterToken = new Parameter();
            operatorToken = new Operator();
            scndParameterToken = new Parameter();
        }
        ~Expression() {
            delete parameterToken;
            delete operatorToken;
            delete scndParameterToken;
        }
        bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
            ProductionElementBase* nullPtr = NULL;
            
            elementPtr = new ProductionElementBase("LEFT_PAREN");
            if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
            
            if(!(parameterToken->isValid(productionElementsDeque))) return false;
            if(!(operatorToken->isValid(productionElementsDeque))) return false;
            if(!(scndParameterToken->isValid(productionElementsDeque))) return false;
            
            elementPtr = new ProductionElementBase("RIGHT_PAREN");
            if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
            
            return true;
        }
        string str() const {
            return "(" + parameterToken->str() + operatorToken->str() + scndParameterToken->str() + ")";
        }
    };
    
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* leftParenTokenPtr = new ProductionElementBase("LEFT_PAREN");
        
        if(*productionElementsDeque.front() == *strTokenPtr) {
            delete idTokenPtr;
            if(expressionToken) delete expressionToken;
            
            idTokenPtr = NULL;
            expressionToken = NULL;
            
            elementPtr = new ProductionElementBase("STRING");
            if(CheckForToken(productionElementsDeque, strTokenPtr)) return true;
        }
        else if(*productionElementsDeque.front() == *idTokenPtr) {
            delete strTokenPtr;
            if(expressionToken) delete expressionToken;
            
            strTokenPtr = NULL;
            expressionToken = NULL;
            
            elementPtr = new ProductionElementBase("ID");
            if(CheckForToken(productionElementsDeque, idTokenPtr)) return true;
        }
        else if(*productionElementsDeque.front() == *leftParenTokenPtr) {
            delete idTokenPtr;
            delete strTokenPtr;
            
            idTokenPtr = NULL;
            strTokenPtr = NULL;
            
            expressionToken = new Expression();
            if(expressionToken->isValid(productionElementsDeque)) return true;
        }
        return false;
    }
    string str() const {
        if(idTokenPtr) return idTokenPtr->str();
        else if(strTokenPtr) return strTokenPtr->str();
        else return expressionToken->str();
    }
};

class ParameterList : public ProductionElementLists{
public:
    ParameterList() {
        elementPtr = NULL;
        listElementPtr = new Parameter();
        followSetItemPtr = new ProductionElementBase("RIGHT_PAREN");
    }
    ~ParameterList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        elementPtr = new ProductionElementBase("COMMA");
        if(*productionElementsDeque.front() == *elementPtr) {
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Parameter();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() const {
        ostringstream os;
        /*deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            os << "," << **it++;
        }*/
        for(size_t i = 0; i < elementListDeque.size(); i++) {
            os << "," << *elementListDeque[i];
        }
        return os.str();
    }
};

class HeadPredicate : public ProductionElement{
public:
    ProductionElementBase* idToken;
    ProductionElementBase* scndIdToken;
    IDList idList;
    HeadPredicate(string st = "HEADPREDICATE") : ProductionElement(st){
        elementPtr = NULL;
        idToken = new ProductionElementBase("ID");
        scndIdToken = new ProductionElementBase("ID");
    }
    ~HeadPredicate() {
        delete idToken;
        delete scndIdToken;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, idToken))) return false;
        
        elementPtr = new ProductionElementBase("LEFT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, scndIdToken))) return false;
        
        if(!(idList.isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("RIGHT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return idToken->str() + "(" + scndIdToken->str() + idList.str() + ")" ;
    }
};



class Predicate : public ProductionElement {
public:
    ProductionElementBase* idToken;
    ProductionElementBase* parameterToken;
    ParameterList parameterList;
    Predicate(string st = "PREDICATE") : ProductionElement(st){
        elementPtr = NULL;
        idToken = new ProductionElementBase("ID");
        parameterToken = new Parameter;
    }
    ~Predicate() {
        delete idToken;
        delete parameterToken;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, idToken))) return false;
        
        elementPtr = new ProductionElementBase("LEFT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        if(!(parameterToken->isValid(productionElementsDeque))) return false;
        if(!(parameterList.isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("RIGHT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return idToken->str() + "(" + parameterToken->str() + parameterList.str() + ")" ;
    }
};

class PredicateList : public ProductionElementLists{
public:
    PredicateList() {
        elementPtr = NULL;
        listElementPtr = new Predicate();
        followSetItemPtr = new ProductionElementBase("PERIOD");
    }
    ~PredicateList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("COMMA");
        if(*productionElementsDeque.front() == *elementPtr) {
            if(!(CheckForToken(productionElementsDeque))) return false;
            
            
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Predicate();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() const {
        ostringstream os;
        /*deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            os << "," << **it++;
        }*/
        for(size_t i = 0; i < elementListDeque.size(); i++) {
            os << "," << *elementListDeque[i];
        }
        return os.str();
    }
};

class Scheme : public ProductionElement {
public:
    ProductionElementBase* idToken;
    ProductionElementBase* scndIdToken;
    IDList idList;
    Scheme(string st = "SCHEME") : ProductionElement(st){
        elementPtr = NULL;
        idToken = new ProductionElementBase("ID");
        scndIdToken = new ProductionElementBase("ID");
    }
    ~Scheme() {
        //delete idToken;
        //delete scndIdToken;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, idToken))) return false;
        
        elementPtr = new ProductionElementBase("LEFT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, scndIdToken))) return false;
        
        if(!(idList.isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("RIGHT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return "  " + idToken->str() + "(" + scndIdToken->str() + idList.str() + ")";
    }
};

class Fact : public ProductionElement {
public:
    ProductionElementBase* idToken;
    ProductionElementBase* stringToken;
    StringList stringList;
    Fact(string st = "FACT") : ProductionElement(st){
        elementPtr = NULL;
        idToken = new ProductionElementBase("ID");
        stringToken = new ProductionElementBase("STRING");
    }
    ~Fact() {
        delete idToken;
        delete stringToken;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        elementPtr = new ProductionElementBase("ID");
        if(!(CheckForToken(productionElementsDeque, idToken))) return false;
        
        elementPtr = new ProductionElementBase("LEFT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        elementPtr = new ProductionElementBase("STRING");
        if(!(CheckForToken(productionElementsDeque, stringToken))) return false;
        
        if(!(stringList.isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("RIGHT_PAREN");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        elementPtr = new ProductionElementBase("PERIOD");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return "  " + idToken->str() + "(" + stringToken->str() + stringList.str() + ").";
    }
    deque<string*> GetDomain() {
        deque<string*> domainDeque = stringList.GetDomain();
        string* str = new string(stringToken->str());
        domainDeque.push_front(str);
        return domainDeque;
    }
};



class Rule : public ProductionElement{
public:
    ProductionElementBase* headPredicate;
    ProductionElementBase* predicate;
    PredicateList predicateList;
    Rule(string st = "RULE") : ProductionElement(st){
        elementPtr = NULL;
        headPredicate = new HeadPredicate;
        predicate = new Predicate;
    }
    ~Rule() {
        delete headPredicate;
        delete predicate;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        
        if(!(headPredicate->isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("COLON_DASH");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        if(!(predicate->isValid(productionElementsDeque))) return false;
        if(!(predicateList.isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("PERIOD");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return "  " + headPredicate->str() + " :- " + predicate->str() + predicateList.str() + ".";
    }
    PredicateList get_predicates() {
        return predicateList;
    }
};

class Query : public ProductionElement{
public:
    ProductionElementBase* predicate;
    Query(string st = "QUERY") : ProductionElement(st){
        elementPtr = NULL;
        predicate = new Predicate;
    }
    ~Query() {
        //delete predicate;
    }
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        ProductionElementBase* nullPtr = NULL;
        
        if(!(predicate->isValid(productionElementsDeque))) return false;
        
        elementPtr = new ProductionElementBase("Q_MARK");
        if(!(CheckForToken(productionElementsDeque, nullPtr))) return false;
        
        return true;
    }
    string str() const {
        return "  " + predicate->str() + "?";
    }
};

class SchemeList : public ProductionElementLists{
public:
    SchemeList() {
        elementPtr = NULL;
        listElementPtr = new Scheme();
        followSetItemPtr = new ProductionElementBase("FACTS");
    }
    ~SchemeList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("ID");
        if(*productionElementsDeque.front() == *elementPtr) {
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Scheme();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};

class FactList : public ProductionElementLists{
public:
    FactList() {
        elementPtr = NULL;
        listElementPtr = new Fact();
        followSetItemPtr = new ProductionElementBase("RULES");
    }
    ~FactList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("ID");
        if(*productionElementsDeque.front() == *elementPtr) {
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Fact();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    set<string> GetDomain() {
        ostringstream os;
        set<string> domainSet;
        deque<ProductionElementBase*>::iterator it = elementListDeque.begin();
        while(it != elementListDeque.end()) {
            ProductionElementBase* ptr = *it;
            deque<string*> domainDeque = ptr->GetDomain();
            deque<string*>::iterator iter = domainDeque.begin();
            while(iter != domainDeque.end()) {
                domainSet.insert(**iter);
                iter++;
            }
            it++;
            for(size_t i = domainDeque.size(); i > 0; i--) {
                delete domainDeque.front();
                domainDeque.pop_front();
            }
        }
        
        return domainSet;
    }
};

class RuleList : public ProductionElementLists {
public:
    RuleList() {
        elementPtr = NULL;
        listElementPtr = new Rule();
        followSetItemPtr = new ProductionElementBase("QUERIES");
    }
    ~RuleList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("ID");
        if(*productionElementsDeque.front() == *elementPtr) {
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Rule();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};

class QueryList : public ProductionElementLists {
public:
    QueryList() {
        elementPtr = NULL;
        listElementPtr = new Query();
        followSetItemPtr = new ProductionElementBase("EOF");
    }
    ~QueryList() {}
    bool isValid(deque<ProductionElementBase*> &productionElementsDeque) {
        
        elementPtr = new ProductionElementBase("ID");
        if(*productionElementsDeque.front() == *elementPtr) {
            //Check for all Schemes
            if(listElementPtr->isValid(productionElementsDeque)) {
                elementListDeque.push_back(listElementPtr);
                listElementPtr = new Query();
            }
            else return false;
            if(!(isValid(productionElementsDeque))) return false;
            else return true;
        }
        else if(!(*productionElementsDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};

#endif /* Elements_h */
