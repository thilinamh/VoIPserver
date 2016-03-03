//
// Created by tm on 31/12/15.
//

#include "DBconnection.h"
#include "UserStructure/User.hxx"
#include "UserStructure/User-odb.hxx"

#include <odb/mysql/database.hxx>


using namespace std;

odb::database* DBconnection::create_db() {
    using namespace std;
    using namespace odb::core;
    try {



        odb::database* db_con(new odb::mysql::database(
                "testuser",// database login name
                "password", // database password
                "testdb" // database name
        ));

        // The following code only creates the schema if it
        // hasn't already been created. To detect the existence of the schema
        // this version tries to query the database for a person object. If
        // the corresponding table does not exist, then an exceptions will be
        // thrown in which case we proceed to creating the schema.

        {
            transaction t (db_con->begin ());

            try
            {
                db_con->query<User> (false);
                cout<<"schema available"<<endl;
                //schema_catalog::drop_schema (*db);
            }
            catch (const odb::exception& e)
            {
                cout<<"creating schema"<<endl;
                schema_catalog::create_schema (*db_con);
            }

            t.commit ();
        }

        return db_con;
    }catch(odb::mysql::database_exception& e){
        cerr<<e.what()<<endl;

        throw e;
    }
    //return unique_ptr<odb::database>(nullptr);
}
unique_ptr<odb::database>  DBconnection::db(DBconnection::create_db()); //initialized
odb::database& DBconnection::getDb() {

    if(db.get()== nullptr){
        db.reset(DBconnection::create_db());
    }
    return *db;
}


