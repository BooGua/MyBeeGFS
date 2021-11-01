#ifndef SETREADONLYMSGEX_H_
#define SETREADONLYMSGEX_H_

#include <common/net/message/storage/readonly/SetReadOnlyMsg.h>

class SetReadOnlyMsgEx : public SetReadOnlyMsg
{
   public:
      virtual bool processIncoming(ResponseContext& ctx);
};

#endif /*SETREADONLYMSGEX_H_*/
