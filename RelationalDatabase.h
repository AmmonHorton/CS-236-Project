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
            Tuple<string> tuple;
            if(i == 0) query = dp.query.str();
            else query = dp.queryList[i - 1]->str();
            query.pop_back();
            query.erase(0, 2);
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
                    
                    break;
                }
                if(i == relations.size() - 1) throw string("Scheme Not Found for Query: " + query);
            }
            
        } while(i++ < (dp.queryList.size()));
        i = 0;

        Header header;
        header.push_back("B");
        header.push_back("A");
        
        Relation relation1 = relations[0].project(header);
        
        Relation relation2 = relations[0].select_const(0, "'b'");
        
        Relation relation3 = relations[0].select_same_columns(0, 1);
        
        cout << "hello";
    }
};

#endif /* RelationalDatabase_h */
