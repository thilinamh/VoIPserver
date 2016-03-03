cmake_minimum_required(VERSION 3.3)
project(PocoServer)

add_definitions(-DCOMM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        Utils.cpp Utils.h
        ServerConnection.cpp ServerConnection.h
        ServerConnectionFactory.cpp ServerConnectionFactory.h
        UserStructure/User-odb.cxx UserStructure/User-odb.hxx
        UserStructure/User.cpp UserStructure/User.hxx
        ClientRequest.cpp ClientRequest.h
        Requests/Behaviours/RegistrationBehavior.cpp
        Requests/Behaviours/RegistrationBehavior.h
        Requests/RequestState/State.h
        Requests/RequestState/RegistrationState.cpp Requests/RequestState/RegistrationState.h
        Requests/RequestState/VerificationState.cpp Requests/RequestState/VerificationState.h
        Requests/RequestState/InitalState.cpp Requests/RequestState/InitalState.h
        Requests/Behaviours/Enterprise/RegistrationBehaviourENT.cpp Requests/Behaviours/Enterprise/RegistrationBehaviourENT.h Requests/Behaviours/Community/RegistrationBehaviourCOMM.cpp Requests/Behaviours/Community/RegistrationBehaviourCOMM.h UserStructure/EnterpriseUser.cpp UserStructure/EnterpriseUser.h UserStructure/CommunityUser.cpp UserStructure/CommunityUser.h Requests/Behaviours/Enterprise/ConferenceBehaviourENT.cpp Requests/Behaviours/Enterprise/ConferenceBehaviourENT.h Requests/Behaviours/Community/VerificationBehaviourCOMM.cpp Requests/Behaviours/Community/VerificationBehaviourCOMM.h DBconnection.cpp DBconnection.h Requests/RequestState/State.cpp Helper/Singleton.cpp Helper/Singleton.h Requests/Behaviours/BehaviourContainer.cpp Requests/Behaviours/BehaviourContainer.h Requests/Behaviours/Enterprise/BehaviourContainerENT.cpp Requests/Behaviours/Enterprise/BehaviourContainerENT.h Requests/Behaviours/Community/BehaviourContainerCOMM.cpp Requests/Behaviours/Community/BehaviourContainerCOMM.h Requests/RequestState/LoginState.cpp Requests/RequestState/LoginState.h
        Helper/OTP/OTPGenerationRequest.h
        Helper/OTP/OTPGenerationRequest.cpp
        Helper/OTP/OTPBase.h Helper/OTP/OTPBase.cpp
        Helper/OTP/OTPBase.h Helper/OTP/OTPAuthRequest.h
        Helper/OTP/OTPBase.h Helper/OTP/OTPAuthRequest.cpp
        Helper/Crypto.cpp Helper/Crypto.h Helper/StringHelper.cpp Helper/StringHelper.h Requests/Behaviours/VerificationBehaviour.cpp Requests/Behaviours/VerificationBehaviour.h  Requests/Behaviours/Enterprise/VerificationBehaviourENT.cpp Requests/Behaviours/Enterprise/VerificationBehaviourENT.h UserStructure/LoggedUsersMap.cpp UserStructure/LoggedUsersMap.h Requests/RequestState/CallSetuptState.cpp Requests/RequestState/CallSetuptState.h Requests/Behaviours/CallInItializerBehaviour.cpp Requests/Behaviours/CallInItializerBehaviour.h Requests/Behaviours/CallingBehaviour.cpp Requests/Behaviours/CallingBehaviour.h Helper/CallAggregator.cpp Helper/CallAggregator.h SMSServerConnectionFactory.cpp SMSServerConnectionFactory.h SMSServerConnection.cpp SMSServerConnection.h Helper/ObserverPatternBase/Subject.cpp Helper/ObserverPatternBase/Subject.h Helper/ObserverPatternBase/Observer.cpp Helper/ObserverPatternBase/Observer.h Requests/Behaviours/Community/CallInitializerBehaviourCOMM.cpp Requests/Behaviours/Community/CallInitializerBehaviourCOMM.h Requests/RequestState/RingingState.cpp Requests/RequestState/RingingState.h Requests/RequestState/InCallState.cpp Requests/RequestState/InCallState.h Requests/RequestState/CallingState.cpp Requests/RequestState/CallingState.h Requests/RequestState/ConferenceSetupState.cpp Requests/RequestState/ConferenceSetupState.h
        CommandServerConnection.h CommandServerConnection.cpp)
add_executable(PocoServer ${SOURCE_FILES})

#INCLUDE_DIRECTORIES(/usr/local/include/odb)
#LINK_DIRECTORIES(/usr/local/lib)
#TARGET_LINK_LIBRARIES(TestPoco PocoFoundation PocoNet)

#set(POCO_ROOT "/usr/local/include/")
#set(Poco_INCLUDE_DIR "/usr/local/include/Poco")
#set(Poco_LIBRARY_DIR "/usr/local/lib")

#SET(BASEPATH "${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("${BASEPATH}")


#include_directories(${POCO_ROOT})
#include_directories(${Poco_INCLUDE_DIRS})
#link_directories(${Poco_LIBRARY_DIRS})
find_library(POCO_FOUNDATION PocoFoundation)
find_library(CRYPTO crypto)
find_library(SSL ssl)
find_library(ODB_Mysql odb-mysql)
find_library(ODB odb)
find_library(Thread pthread)

#${CRYPTO} ${SSL}
target_link_libraries(PocoServer ${POCO_FOUNDATION}  PocoNet ${CRYPTO} ${SSL} ${ODB} ${ODB_Mysql} ${Thread})