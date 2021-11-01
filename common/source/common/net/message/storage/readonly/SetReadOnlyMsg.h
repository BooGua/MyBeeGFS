#ifndef SETREADONLYMSG_H_
#define SETREADONLYMSG_H_

#include <common/net/message/SimpleIntStringMsg.h>

class SetReadOnlyMsg : public SimpleIntStringMsg
{
   public:

      SetReadOnlyMsg(const std::string entryID, bool value) :
         SimpleIntStringMsg(
               NETMSGTYPE_SetReadOnly,
               (int) value,
               std::move(entryID)){}

      SetReadOnlyMsg() : SimpleIntStringMsg(NETMSGTYPE_SetReadOnly){}

   private:

   public:

      bool getSetOrNot() const
      {
         return (bool)getIntValue();
      }

      const std::string& getEntryID() const
      {
         return getStrValue();
      }
};

#endif /*SETREADONLYMSG_H_*/
