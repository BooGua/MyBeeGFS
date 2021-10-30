#ifndef GETREADONLYRESPMSG_H_
#define GETrEADONLYRESPMSG_H_

#include <common/net/message/SimpleStringMsg.h>

class GetReadOnlyRespMsg : public SimpleStringMsg
{
   public:
      GetReadOnlyRespMsg(const std::string& entryIDs):
         SimpleStringMsg(NETMSGTYPE_GetReadOnlyResp, entryIDs.c_str()){}
      
      GetReadOnlyRespMsg() : SimpleStringMsg(NETMSGTYPE_GetReadOnlyResp){}

      // getters & setters
      std::string getEntryIDs()
      {
         return std::string(getValue());
      }
};

#endif /*GETREADONLYRESPMSG_H_*/
