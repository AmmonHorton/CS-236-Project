//
//  Terminals.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 2/12/21.
//

#ifndef Terminals_h
#define Terminals_h
#include <string>
#include <sstream>

class TokenItemInterface {
private:
    string selfType;
public:
    string data;
    TokenItemInterface(string st = "TokenItemInterface", string dt = "") {
        selfType = st;
        data = dt;
    }
    bool operator==(const TokenItemInterface& tc) {return this->selfType == tc.selfType;}
    
    virtual string str() {
        return (data);
    }
        
    friend ostream & operator << (ostream &os, TokenItemInterface &tII) {
        os << tII.str();
        return os;
    }
    virtual bool isValid(deque<TokenItemInterface*> &tokenDeque) {return true;}
    virtual deque<string*> GetDomain() {
        deque<string*> dq;
        return dq;
    }
};

class TokenItem : public TokenItemInterface{
protected:
    TokenItemInterface* tItemPtr;
public:
    TokenItem(string st = "TokenItem") : TokenItemInterface(st) {}
    ~TokenItem() {
        if(tItemPtr) {
            delete tItemPtr;
            tItemPtr = NULL;
        }
    }
    bool CheckForToken(deque<TokenItemInterface*> &tokenDeque, TokenItemInterface* &fsaPtr) {
        if(*tokenDeque.front() == *tItemPtr) {
            if(fsaPtr != NULL) {
                delete fsaPtr;
                fsaPtr = tokenDeque.front();
                tokenDeque.pop_front();
                delete tItemPtr;
                tItemPtr = NULL;
                return true;
            }
            else {
                delete tokenDeque.front();
                tokenDeque.pop_front();
                delete tItemPtr;
                tItemPtr = NULL;
                return true;
            }
        }
        else {
            if(fsaPtr != NULL) {
                delete fsaPtr;
                fsaPtr = NULL;
            }
            delete tItemPtr;
            tItemPtr = NULL;
            return false;
        }
    }
    string str() {
        return "";
    }
};



class TokenItemLists {
protected:
    TokenItemInterface* tItemPtr;
    TokenItemInterface* listTokenfsaPtr;
    TokenItemInterface* followSetItemPtr;
public:
    deque<TokenItemInterface*> listTokenfsaDeque;
    ~TokenItemLists() {
        if(tItemPtr) {
            delete tItemPtr;
            tItemPtr = NULL;
        }
        delete listTokenfsaPtr;
        delete followSetItemPtr;
        for(size_t i = listTokenfsaDeque.size(); i > 0; i--) {
            delete listTokenfsaDeque.back();
            listTokenfsaDeque.pop_back();
        }
    }
    
    bool CheckForToken(deque<TokenItemInterface*> &tokenDeque) {
        if(*tokenDeque.front() == *tItemPtr) {
            if(*listTokenfsaPtr == *tItemPtr) {
                listTokenfsaDeque.push_back(tokenDeque.front());
                tokenDeque.pop_front();
                delete tItemPtr;
                tItemPtr = NULL;
                return true;
            }
            else {
                delete tokenDeque.front();
                tokenDeque.pop_front();
                delete tItemPtr;
                tItemPtr = NULL;
                return true;
            }
        }
        else {
            delete tItemPtr;
            tItemPtr = NULL;
            return false;
        }
    }
    
    size_t GetSizeOfList() {
        return listTokenfsaDeque.size();
    }
    string str() {
        ostringstream os;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            os << **it++ << endl;
        }
        return os.str();
    }
};

#endif /* Terminals_h */
