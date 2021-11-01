#include <common/net/message/storage/readonly/SetReadOnlyRespMsg.h>
#include <common/net/message/control/GenericResponseMsg.h>
#include <program/Program.h>
#include <components/ReadonlyManager.h>

#include "SetReadOnlyMsgEx.h"

bool SetReadOnlyMsgEx::processIncoming(ResponseContext& ctx)
{
   FhgfsOpsErr result;

   App* app = Program::getApp();

   if(app->isShuttingDown())
   {
      ctx.sendResponse(GenericResponseMsg(
               GenericRespMsgCode_TRYAGAIN, "Mgmtd shutting down."));
      return true;
   }

   bool setOrNot = getSetOrNot();
   const std::string& entryID = getEntryID();

   LOG_DEBUG(
         "Set dir readonly incoming",
         Log_DEBUG,
         "Set "  + entryID + (setOrNot? " readonly": " readwrite"));

   ReadonlyManager* readonlyManager = app->getReadonlyManager();
   bool setRes = readonlyManager->updateReadonlyEntries(entryID, setOrNot);
   if (setRes)
      result = FhgfsOpsErr_SUCCESS;
   else
      result = FhgfsOpsErr_INTERNAL;

   ctx.sendResponse(SetReadOnlyRespMsg(result));

   return true;
}
