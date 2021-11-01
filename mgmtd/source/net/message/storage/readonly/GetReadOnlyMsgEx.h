#ifndef GETREADONLYMSGEX_H_
#define GETREADONLYMSGEX_H_

#include <common/net/message/storage/readonly/GetReadOnlyMsg.h>

class GetReadOnlyMsgEx : public GetReadOnlyMsg
{
   public:
      virtual bool processIncoming(ResponseContext& ctx);
};

#endif /*GETREADONLYMSGEX_H_*/
