#include <app/App.h>
#include <common/net/message/storage/readonly/GetReadOnlyMsg.h>
#include <common/net/message/storage/readonly/GetReadOnlyRespMsg.h>
#include <common/toolkit/MathTk.h>
#include <common/toolkit/MetadataTk.h>
#include <common/toolkit/NodesTk.h>
#include <common/toolkit/UnitTk.h>
#include <program/Program.h>
#include "ModeGetReadOnly.h"

// 这个参数的作用是什么。
#define MODEGETREADONLY_ARG_UNMOUNTEDPATH "--unmounted"
#define MODEGETREADONLY_ARG_READFROMSTDIN "-"

int ModeGetReadOnly::execute()
{

   std::cout << "You run getreadonly! <==============" << std::endl;

   // art TODO 不懂。
   int retVal = APPCODE_NO_ERROR;

   // 获取 mgmtNode。
   App* app = Program::getApp();
   NodeStoreServers* mgmtNodes = app->getMgmtNodes();
   auto mgmtNode = mgmtNodes->referenceFirstNode();

   // art TODO StringMap 是用来放什么的？
   StringMap* cfg = app->getConfig()->getUnknownConfigArgs();
   StringMapIter iter;

   // check arguments
   bool useMountedPath = true; // art TODO dontknow
   iter = cfg->find(MODEGETREADONLY_ARG_UNMOUNTEDPATH);
   if(iter != cfg->end())
   {
      useMountedPath = false;
      cfg->erase(iter);
   }

   if(cfg->empty()) // 查看路径是否为空。
   {
      std::cerr << "No path specified." << std::endl;
      return APPCODE_RUNTIME_ERROR;
   }

   // 这一步也没理解。
   std::string pathStr = cfg->begin()->first;
   cfg->erase(cfg->begin());

   // 查看参数等是否合法。
   if(ModeHelper::checkInvalidArgs(cfg))
   {
      return APPCODE_INVALID_CONFIG;
   }

   // 为什么会有从 stdin 读取路径？
   bool readFromStdin = (pathStr == MODEGETREADONLY_ARG_READFROMSTDIN);
   if (readFromStdin)
   { // read first path from stdin
      pathStr.clear();
      getline(std::cin, pathStr);

      if (pathStr.empty())
      {
         return APPCODE_NO_ERROR; // no files given is not an error.
      }
   }

   do // art TODO dontknow.
   {
      std::string mountRoot;
      bool setRes;

      // find owner node
      
      Path path(pathStr);

      NodeHandle ownerNode;
      EntryInfo entryInfo;

      if(!ModeHelper::getEntryAndOwnerFromPath(
               path,
               useMountedPath,
               *app->getMetaNodes(),
               app->getMetaRoot(),
               *app->getMetaMirrorBuddyGroupMapper(),
               entryInfo,
               ownerNode))
      {
         retVal = APPCODE_RUNTIME_ERROR;
         std::cout << "Failed to access " << pathStr << std::endl;
         goto finish_this_entry;
      }

      setRes = getReadOnly(*mgmtNode, pathStr, entryInfo.getEntryID());
      if (!setRes) {
         retVal = APPCODE_RUNTIME_ERROR;
         goto finish_this_entry;
      }

finish_this_entry:

      if (!readFromStdin)
         break;
      std::cout << std::endl;

      // read next ralPath from stdin
      pathStr.clear();
      getline(std::cin, pathStr);

   } while (!pathStr.empty());

   return retVal;
}

void ModeGetReadOnly::printHelp()
{
   std::cout << " ======= help ======= " << std::endl;
   std::cout << " ======= help ======= " << std::endl;
   std::cout << " ======= help ======= " << std::endl;
   std::cout << " ======= help ======= " << std::endl;
}

bool ModeGetReadOnly::getReadOnly(
      Node& mgmtNode,
      const std::string& path,
      const std::string& entryID)
{
   GetReadOnlyMsg msg(entryID);

   // 发送了信息，所以这个 MessagingTk，很有可能就是负责 Msg 所有相关动作的。
   const auto respMsg = MessagingTk::requestResponse(
         mgmtNode,
         msg,
         NETMSGTYPE_GetReadOnlyResp);
   if(!respMsg)
   {
      std::cerr << "Failed to communicate with node: " << mgmtNode.getTypedNodeID() << std::endl;
      return false;
   }

   GetReadOnlyRespMsg* respMsgCast = (GetReadOnlyRespMsg*)respMsg.get();

   std::string entryIDs = respMsgCast->getEntryIDs();
   if(entryID == entryIDs)
   {
      std::cout << path << " : readonly" << std::endl;
   } else {
      std::cout << path << " readwrite" << std::endl;
   }

   return true;

}























