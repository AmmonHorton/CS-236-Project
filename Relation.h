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
public:
    Header header;
    set<Tuple<string>> tuples;
    Relation(string name, Header header) {
        this->name = name;
        this->header = header;
    }
    Relation() {
        this->name = "";
        Header newHeader;
        this->header = newHeader;
    }
    
    /*Relation(Relation& relation) {
        header = relation.header;
        name = relation.get_name();
        for (set<Tuple<string>>::iterator it = relation.tuples.begin() ; it != relation.tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            tuples.insert(tuple);
        }
    }*/
    
    string get_name() {return name;}
    
    Tuple<string> insert(Tuple<string> tuple) {
        if(tuple.size() < header.size()) throw string("Not Enough Arguments for Scheme: " + name);
        if(tuple.size() > header.size()) throw string("Too Many Arguments for Scheme: " + name);
        //must change for sets
        tuples.insert(tuple);
        return tuple;
    }
    bool is_empty() {return tuples.size() == 0;}\
    size_t size() {return tuples.size();}
    
    Relation project(Header newHeader) {
        Relation newRelation(name, newHeader);
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple;
            for(size_t  i = 0; i < newHeader.size(); i++) {
                size_t j = 0;
                while(header[j++] != newHeader[i]) {if(j >= header.size()) throw string("Attribute does not exist or exists more than once: " + newHeader[i]);}
                j--;
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
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            //must change for sets
            if(it->select_row(index1, value)) newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    
    Relation select_no_similar_const(size_t index1, string value) {
        Relation newRelation(name, header);
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            //must change for sets
            if(!(it->select_row(index1, value))) newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    
    Relation select_rows_with_same_columns(size_t index1, size_t index2) {
        Relation newRelation(name, header);
        
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            //must change for sets
            if(it->select_column(index1) == it->select_column(index2)) newRelation.tuples.insert(tuple);
        }
        return newRelation;
    }
    
    Relation Difference(Relation relation) {
        Relation newRelation(name, header);
        bool tmpBool;
        //intersection of columns with same attribute names
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            tmpBool = false;
            for (set<Tuple<string>>::iterator iterator = relation.tuples.begin() ; iterator != relation.tuples.end(); ++iterator) {
                if(*it == *iterator) tmpBool = true;
            }
            if(!(tmpBool))newRelation.insert(tuple);
        }
        return newRelation;
    }
    
    Relation join(Relation relation) {
        Relation tmpRelation(name, header);
        Relation argRelation(relation.name, relation.header);
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            tmpRelation.tuples.insert(tuple);
        }
        for (set<Tuple<string>>::iterator it = relation.tuples.begin() ; it != relation.tuples.end(); ++it) {
            Tuple<string> tuple = *it;
            argRelation.tuples.insert(tuple);
        }
        
        
        
        vector<Relation> tmpRelations;
        vector<Relation> argRelations;
        
        set<string> sameAttributes;
        
        
        
        //from here to
        //intersection of columns with same attribute names
        for(size_t i = 0; i < header.size(); i++) {
            for(size_t j = 0; j < relation.header.size(); j++) {
                if(header[i] == relation.header[j]) {
                    sameAttributes.insert(header[i]);
                }
            }
        }
        
        for(size_t i = 0; i < header.size(); i++) {
            for(size_t j = 0; j < relation.header.size(); j++) {
                if(header[i] == relation.header[j]) {
                    for (set<Tuple<string>>::iterator it = argRelation.tuples.begin() ; it != argRelation.tuples.end(); ++it) {
                        Tuple<string> tuple = *it;
                        //must change for sets
                        tmpRelations.push_back(tmpRelation.select_const(i, tuple[j]));
                    }
                    for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
                        Tuple<string> tuple = *it;
                        //must change for sets
                        argRelations.push_back(argRelation.select_const(j, tuple[i]));
                    }
                }
            }
        }
        
        if(tmpRelations.size() != 0) {
            tmpRelation = tmpRelations[0];
            for(size_t t = 1; t < tmpRelations.size(); t++) {
                tmpRelation = tmpRelation.relation_union(tmpRelations[t]);
            }
        }
        if(argRelations.size() != 0) {
            argRelation = argRelations[0];
            for(size_t t = 1; t < argRelations.size(); t++) {
                argRelation = argRelation.relation_union(argRelations[t]);
            }
        }
        
        //here needs to be rethought
        //We are trying to get the intersection of only the columns that matter
        
        
        
        
        
        
        
        
        
        

        vector<size_t> variablePositions;
        vector<string> variableArguments;
        for(size_t k = 0; k < tmpRelation.header.size(); k++) {
            variableArguments.push_back(tmpRelation.header[k]);
            variablePositions.push_back(k);
        }
        for(size_t k = 0; k < argRelation.header.size(); k++) {
            variableArguments.push_back(argRelation.header[k]);
            variablePositions.push_back(k + tmpRelation.header.size());
        }
        
        set<string> variableArgumentsSet;
        for(size_t k = 0; k < variablePositions.size();k++) {
            variableArgumentsSet.insert(variableArguments[k]);
        }
        
        vector<Tuple<size_t>> modePositions;
        //go through set and asisgn each row of the 2d vector as a tuple of the variable's multiple positions throughout the query
        
        for (set<string>::iterator it = variableArgumentsSet.begin(); it != variableArgumentsSet.end(); ++it) {
            Tuple<size_t> tuple1;
            for(size_t k = 0; k < variableArguments.size();k++) {
                if(*it == variableArguments[k]) tuple1.push_back(variablePositions[k]);
            }
            modePositions.push_back(tuple1);
        }
        
        
        Relation tmpRelation1;
        tmpRelation1 = tmpRelation.cartesian_product(argRelation);
        
        for (size_t k = 0; k < modePositions.size(); k++) {
            for(size_t t = 1; t < (modePositions[k].size()); t++) {
                tmpRelation1 = tmpRelation1.select_rows_with_same_columns(modePositions[k][0], modePositions[k][t]);
            }
        }
        
        
        
        
        
        
        
        
        
        
        
        
        

        Relation newRelation(name, tmpRelation1.header);
        
        if(sameAttributes.size() != 0) {
            for(size_t i = 0; (i < tmpRelation1.header.size() - 1); i++) {
                for(size_t t = (i + 1); t < tmpRelation1.header.size(); t++) {
                    if(tmpRelation1.header[i] == tmpRelation1.header[t] && (sameAttributes.find(tmpRelation1.header[i]) != sameAttributes.end())) {
                        for (set<Tuple<string>>::iterator it = tmpRelation1.tuples.begin() ; it != tmpRelation1.tuples.end(); ++it) {
                            Tuple<string> tuple = *it;
                            if(it->select_column(i) == it->select_column(t)) newRelation.insert(tuple);
                        }
                    }
                }
            }
        }
        else {
            for (set<Tuple<string>>::iterator it = tmpRelation1.tuples.begin() ; it != tmpRelation1.tuples.end(); ++it) {
                Tuple<string> tuple = *it;
                newRelation.insert(tuple);
            }
        }
        
        
        Header newHeader = argRelation.header;
        for(size_t i = 0; i < newHeader.size(); i++) {
            for (set<string>::iterator it = sameAttributes.begin() ; it != sameAttributes.end(); ++it) {
                if(newHeader[i] == *it)newHeader.erase(i);
            }
        }
        Header projectHeader;
        
        for(size_t i = 0; i < tmpRelation.header.size();i++) {
            projectHeader.push_back(tmpRelation.header[i]);
        }
        for(size_t i = 0; i < newHeader.size();i++) {
            projectHeader.push_back(newHeader[i]);
        }
        newRelation = newRelation.project(projectHeader);
        
        
        return newRelation;
    }
    
    Relation cartesian_product(Relation relation) {
        if(relation.is_empty()) return *this;
        if(is_empty()) return relation;
        Header newHeader = header;
        
        for(size_t j = 0; j < relation.header.size(); j++) {
            newHeader.push_back(relation.header[j]);
        }
        Relation newRelation(name, newHeader);
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            for (set<Tuple<string>>::iterator iterator = relation.tuples.begin() ; iterator != relation.tuples.end(); ++iterator) {
                newRelation.insert(*it + *iterator);
            }
        }
        return newRelation;
    }
    
    
    Relation relation_union(Relation relation) {
        Relation newRelation = *this;
        for (set<Tuple<string>>::iterator iterator = relation.tuples.begin() ; iterator != relation.tuples.end(); ++iterator) {
            newRelation.insert(*iterator);
        }
        return newRelation;
    }
    
    Relation rename(string oldAttributeName, string newAttributeName)  {
        Relation newRelation(name, header);
            for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
                newRelation.tuples.insert(*it);
            }
        
        
        for(size_t  i = 0; i < newRelation.header.size(); i++) {
            if(newRelation.header[i] == oldAttributeName) {
                newRelation.header[i] = newAttributeName;
                break;
            }
        }
        
        return newRelation;
    }
    
    string str() {
        size_t i = 0;
        ostringstream os;
        for (set<Tuple<string>>::iterator it = tuples.begin() ; it != tuples.end(); ++it) {
            os << "  ";
            for(size_t j = 0; j < header.size(); j++) {
                Tuple<string> tuple = *it;
                os << header[j] << "=" << tuple[j] << ((j == header.size() - 1)? "" : ", ");
            }
            os << endl;
            i++;
        }
        return os.str();
    }
};

#endif /* Relation_h */
