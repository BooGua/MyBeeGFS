#ifndef MODESETREADONLY_H_
#define MODESETREADONLY_H_

#include <common/Common.h>
#include "Mode.h"

class ModeSetReadOnly : public Mode
{
   public:
      ModeSetReadOnly(bool value = true) : setOrNot(value){}

      virtual int execute();
      static void printHelp();

   private:
      bool setOrNot; // 标明是否为只读。
      bool setReadOnly(Node& mgmtNode, const std::string& entryID);
};

class ModeSetReadWrite : public ModeSetReadOnly
{
   public:
      ModeSetReadWrite() : ModeSetReadOnly(false){}

      static void printHelp();
};

#endif /*MODESETREADONLY_H_*/
