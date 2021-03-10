//
//  RelationalDatabase.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 3/8/21.
//

#ifndef RelationalDatabase_h
#define RelationalDatabase_h
#include "Relation.h"


class Database {
public:
    ostringstream outStr;
    vector<Relation> relations;
    Database(DatalogProgram dp) {
        size_t i = 0;
        do {
            string scheme;
            Header header;
            if(i == 0) scheme = dp.scheme.str();
            else scheme = dp.schemeList[i - 1]->str();
            scheme.pop_back();
            scheme.erase(0, 2);
            istringstream stream(scheme);
            getline(stream, scheme, '(');
            
            while(stream) {
                string substr;
                getline(stream, substr, ','); //get first string delimited by comma
                if(substr == "") break;
                if(substr[0] == '\'') throw string("Argument for Scheme: " + scheme + " is not a valid ID");
                header.push_back(substr);
            }
            Relation relation(scheme, header);
            
            relations.push_back(relation);
        } while(i++ < (dp.schemeList.size()));
        i = 0;
        
        for(size_t j = 0; j < dp.factList.size(); j++) {
            string fact;
            Tuple<string> tuple;
            fact = dp.factList[j]->str();
            fact.pop_back();
            fact.pop_back();
            fact.erase(0, 2);
            istringstream stream(fact);
            getline(stream, fact, '(');
            
            while(stream) {
                string substr;
                getline(stream, substr, ','); //get first string delimited by comma
                if(substr == "") break;
                if(substr.at(0) != '\'') throw string("Argument for Fact: " + fact + " is not a valid string");
                tuple.push_back(substr);
            }
            for(int j = 0; j < relations.size(); j++) {
                if(relations[j].get_name() == fact){
                    relations[j].insert(tuple);
                    break;
                }
                if(i == relations.size() - 1) throw string("Scheme Not Found for Fact: " + fact);
            }
        }
        
        do {
            string query;
            string entireQueryLine;
            Tuple<string> tuple;
            if(i == 0) query = dp.query.str();
            else query = dp.queryList[i - 1]->str();
            query.erase(0, 2);
            entireQueryLine = query;
            query.pop_back();
            query.pop_back();
            istringstream stream(query);
            getline(stream, query, '(');
            
            while(stream) {
                string substr;
                getline(stream, substr, ','); //get first string delimited by comma
                if(substr == "") break;
                tuple.push_back(substr);
            }
            
            for(int j = 0; j < relations.size(); j++) {
                if(relations[j].get_name() == query){
                    
                    //do stuff with queries and analyze and whatnot
                    //query is the scheme being used, and tuple is the list of arguments searched for in the given scheme
                    
                    vector<size_t> stringPositions;
                    vector<string> stringArguments;
                    vector<size_t> variablePositions;
                    vector<string> variableArguments;
                    for(size_t k = 0; k < tuple.size();k++) {
                        if(tuple[k][0] == '\'') {
                            stringArguments.push_back(tuple[k]);
                            stringPositions.push_back(k);
                        }
                        else{
                            variableArguments.push_back(tuple[k]);
                            variablePositions.push_back(k);
                        }
                    }
                    Relation relation = relations[j];
                    
                    for(size_t k = 0; k < stringPositions.size();k++) {
                        relation = relation.select_const(stringPositions[k], stringArguments[k]);
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
                    
                    for (size_t k = 0; k < modePositions.size(); k++) {
                        for(size_t t = 0; t < (modePositions[k].size() - 1); t++) {
                            relation = relation.select_same_columns(modePositions[k][0], modePositions[k][t + 1]);
                        }
                    }
                    
                    Header trimProject;
                    for(size_t k = 0; k < variablePositions.size(); k++) {
                        trimProject.push_back(relation.header[variablePositions[k]]);
                    }
                    
                    relation = relation.project(trimProject);
                    
                    Header finalProject;
                    for(size_t k = 0; k < variableArgumentsSet.size(); k++) {
                        finalProject.push_back("");
                    }
                    
                    for(size_t k = 0; k < variableArguments.size(); k++) {
                        if(variableArgumentsSet.find(variableArguments[k]) != variableArgumentsSet.end()) {
                            variableArgumentsSet.erase(variableArguments[k]);
                            finalProject[k] = (variableArguments[k]);
                        }
                    }
                    
                    for(size_t k = 0; k < variableArguments.size(); k++) {
                        relation.rename(relation.header[k], variableArguments[k]);
                    }
                    
                    
                    relation = relation.project(finalProject);
                    ostringstream os;
                    os << "Yes(" << relation.size() << ")";
                    outStr << entireQueryLine << " ";
                    outStr << ((relation.is_empty())? "No" : os.str()) << endl;
                    if(!relation.is_empty()) {
                        string str = relation.str();
                        if(str.find("  \n") != string::npos) {
                            str.pop_back();
                            str.pop_back();
                            str.pop_back();
                        }
                        outStr << str;
                    }
                    break;
                }
                if(j == relations.size() - 1) throw string("Scheme Not Found for Query: " + query);
            }
            
        } while(i++ < (dp.queryList.size()));
        i = 0;
    }
    
    string str() {
        return outStr.str();
    }
    
};

#endif /* RelationalDatabase_h */
