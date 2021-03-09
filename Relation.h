//
//  Relation.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 3/8/21.
//

#ifndef Relation_h
#define Relation_h

#include <vector>
#include <string>
#include <set>
#include <sstream>
#include "Elements.h"
#include "Parser.h"
#include "Header.h"
#include "Tuple.h"

class Relation {
private:
    string name;
    Header header;
public:
    set<Tuple<string>> tuples;
    Relation(string name, Header& header) {
        this->name = name;
        this->header = header;
    }
    
    string get_name() {return name;}
    
    Tuple<string> insert(Tuple<string> tuple) {
        if(tuple.size() < header.size()) throw string("Not Enough Arguments for Scheme: " + name);
        if(tuple.size() > header.size()) throw string("Too Many Arguments for Scheme: " + name);
        //must change for sets
        tuples.insert(tuple);
        return tuple;
    }
    Relation project(Header newHeader) {
        Relation newRelation(name, newHeader);
        /*for(size_t  k = 0; k < tuples.size(); k++) {
            Tuple<string> tuple;
            for(size_t  i = 0; i < header.size(); i++) {
                size_t j = i;
                while(header[j++] != newHeader[i]) {if(j >= header.size()) throw string("Attribute does not exist or exists more than once: " + newHeader[i]);}
                j--;
                if(j != i) {
                    string tmpStr = header[i];
                    header[i] = header[j];
                    header[j] = tmpStr;
                }
                //must change for sets
                tuple.push_back(tuples[k].select_column(j));
            }
            //must change for sets
            newRelation.tuples.push_back(tuple);
        }*/
        
        
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple;
            for(size_t  i = 0; i < header.size(); i++) {
                size_t j = 0;
                while(header[j++] != newHeader[i]) {if(j >= header.size()) throw string("Attribute does not exist or exists more than once: " + newHeader[i]);}
                j--;
                /*if(j != i) {
                    string tmpStr = header[i];
                    header[i] = header[j];
                    header[j] = tmpStr;
                }*/
                string tmpStr = it->select_column(j);
                tuple.push_back(tmpStr);
            }
            //must change for sets
            newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    
    Relation select_const(size_t index1, string value) {
        Relation newRelation(name, header);
        /*for(size_t  k = 0; k < tuples.size(); k++) {
            Tuple<string> tuple;
            //must change for sets
            if(tuples[k].select_row(index1, value)) newRelation.tuples.push_back(tuples[k]);
        }*/
        
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            //must change for sets
            if(it->select_row(index1, value)) newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    Relation select_same_columns(size_t index1, size_t index2) {
        Relation newRelation(name, header);
        /*for(size_t  k = 0; k < tuples.size(); k++) {
            Tuple<string> tuple;
            //must change for sets
            if(tuples[k].select_column(index1) == tuples[k].select_column(index2)) newRelation.tuples.push_back(tuples[k]);
        }*/
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            //must change for sets
            if(it->select_column(index1) == it->select_column(index2)) newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    void rename(string oldAttributeName, string newAttributeName)  {
        for(size_t  i = 0; i < header.size(); i++) {
            if(header[i] == oldAttributeName) header[i] = newAttributeName;
        }
    }
};

#endif /* Relation_h */
