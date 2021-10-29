// art TODO GETREADONLYMSG_H_ 这个宏定义的作用是什么？
#ifndef GETREADONLYMSG_H_
#define GETREADONLYMSG_H_

#include <common/net/message/SimpleStringMsg.h>

// SimpleStringMsg 是所有 Msg 需要继承的，
// 包含（1）基本信息；（2）操作方法。
class GetReadOnlyMsg : public SimpleStringMsg{
   public:
      GetReadOnlyMsg(const std::string& entryID) :
         // NETMSGTYPE_GetRradOnly go to 202110291358
         // SimpleStringMsg go 202110291428。art TODO 这里具体先不管了，以后再看。
         SimpleStringMsg(NETMSGTYPE_GetRradOnly, entryID.c_str())
      {}

      // For deserialization only! 什么时候会用到反序列化？
      GetReadOnlyMsg() : SimpleStringMsg(NETMSGTYPE_GetRradOnly) {}
};

select(FD_CLR);

#endif /*GETREADONLYMSG_H_*/
