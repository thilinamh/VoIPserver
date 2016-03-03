//
// Created by user on 12/21/15.
//

#ifndef POCOSERVER_COMMUNITYUSER_H
#define POCOSERVER_COMMUNITYUSER_H

#include "User.hxx"
#include "../Requests/Behaviours/RegistrationBehavior.h"


class CommunityUser: public User {
friend class odb::access;
public:
    CommunityUser(const StreamSocket &socket) ;

    virtual void bindWithServer() override;


    //virtual bool load_by_uid() override;

private:
CommunityUser()= default;
};

#pragma db object(CommunityUser) pointer(std::shared_ptr)
//#pragma db member(CommunityUser::verfBehaviour) transient

#endif //POCOSERVER_COMMUNITYUSER_H
