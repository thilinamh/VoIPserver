//
// Created by tm on 31/12/15.
//

#ifndef POCOSERVER_DBCONNECTION_H
#define POCOSERVER_DBCONNECTION_H

#include <odb/mysql/exceptions.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <memory>
using namespace std;
class DBconnection {

public:


    static odb::database& getDb() ;


private:
    DBconnection()= default;
    DBconnection(DBconnection const&)= delete;
    DBconnection& operator=(DBconnection const&)= delete;
    static odb::database* create_db();
    static unique_ptr<odb::database>  db;
};

#endif //POCOSERVER_DBCONNECTION_H
