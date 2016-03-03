//
// Created by user on 12/21/15.
//

#include "CommunityUser.h"
#include "../Requests/Behaviours/Community/BehaviourContainerCOMM.h"
#include "../DBconnection.h"


CommunityUser::CommunityUser(const StreamSocket &socket): User(socket, new BehaviourContainerCOMM()) {
cout<<"CommiunityUser Created"<<endl;
}

void CommunityUser::bindWithServer() {
  // Before calling shared_from_this(),
  // there should be at least one std::shared_ptr p that owns *this
  tcpConnection->setContext(shared_from_this());
}
/*
bool CommunityUser::load_by_uid() {
    odb::database& db = DBconnection::getDb();
    odb::transaction t(db.begin());


    try{


        //unique_ptr<User> jane(db.load<User>(uid_));
        db.load (User::getUid(), *this);

        //db.reload(this);
        t.commit();
        cout<<"user loaded sucessfully "<<" uuid : "<<User::getUuid()<<endl;
    }catch(const odb::object_not_persistent& e){
        cout<<"user does not exist"<<endl;
        t.rollback();
        return false;
    }catch (odb::exception & e){
        cerr<<e.what()<<endl;
        t.rollback();
        return false;
    }
    return true;

}
*/