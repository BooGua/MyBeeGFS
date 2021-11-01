#ifndef SETREADONLYRESPMSG_H_
#define SETREADONLYRESPMSG_H_

#include <common/net/message/SimpleIntMsg.h>

class SetReadOnlyRespMsg : public SimpleIntMsg
{
   public:
      SetReadOnlyRespMsg(int result):
         SimpleIntMsg(NETMSGTYPE_SetReadOnlyResp, result){}

      SetReadOnlyRespMsg() : SimpleIntMsg(NETMSGTYPE_SetReadOnlyResp){}

   private:

   public:
};

#endif /*SETREADONLYRESPMSG_H_*/
