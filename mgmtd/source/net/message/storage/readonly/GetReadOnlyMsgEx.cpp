#include <common/net/message/storage/readonly/GetReadOnlyRespMsg.h>
#include <common/net/message/control/GenericResponseMsg.h>
#include <program/Program.h>
#include <components/ReadonlyManager.h>

#include "GetReadOnlyMsgEx.h"

bool GetReadOnlyMsgEx::processIncoming(ResponseContext& ctx)
{
   std::string entryIDs = "";

   App* app = Program::getApp();

   // 检查自己是不是关闭。
   if (app->isShuttingDown())
   {
      // art TODO sendResponse 是给谁发送呢？
      ctx.sendResponse(GenericResponseMsg(GenericRespMsgCode_TRYAGAIN, "Mgmtd shutting down."));
      return true;
   }

   // 获取 getEntryID。
   const std::string& entryID = getEntryID();
   bool getAll = entryID.length() == 0;

   LOG_DEBUG(
         "Get dir readonly incoming", 
         Log_DEBUG,
         "Get " + (getAll? "all:" entryID) + " readonly entry");

   ReadonlyManager* readonlyManager = app->getReadonlyManager();
   if (getAll)
   {
      entryIDs = readonlyManager->getReadonlyEntries();
   } else if (readonlyManager->isEntryReadOnly(entryID))
   {
      entryIDs = entryID;
   }

   // 发给谁，怎么发呢？
   ctx.sendResponse(GetReadOnlyRespMsg(entryIDs));

   return true;
}

