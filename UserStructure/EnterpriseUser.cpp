//
// Created by user on 12/21/15.
//

#include "EnterpriseUser.h"
#include "../Requests/Behaviours/Enterprise/BehaviourContainerENT.h"

EnterpriseUser::EnterpriseUser(const StreamSocket &socket):User(socket, new BehaviourContainerENT()) {

}
