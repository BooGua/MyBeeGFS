#include <app/App.h>
#include <common/net/message/storage/readonly/SetReadOnlyMsg.h>
#include <common/net/message/storage/readonly/SetReadOnlyRespMsg.h>
#include <common/toolkit/MathTk.h> // art TODO Math?
#include <common/toolkit/MetadataTk.h>
#include <common/toolkit/NodesTk.h>
#include <common/toolkit/UnitTk.h>
#include <program/Program.h>
#include "ModeSetReadOnly.h"

// art TODO 这几个参数的用处？
#define MODESETREADONLY_ARG_SET                 "--set"
#define MODESETREADONLY_ARG_UNSET               "--unset"
#define MODESETREADONLY_ARG_UNMOUNTEDPATH       "--unmounted"
#define MODESETREADONLY_ARG_READFROMSTDIN       "-"

int ModeSetReadOnly::execute()
{
   int retVal = APPCODE_NO_ERROR;

   App* app = Program::getApp();
   NodeStoreServers* mgmtNodes = app->getMgmtNodes();
   auto mgmtNode = mgmtNodes->referenceFirstNode();

   StringMap* cfg = app->getConfig()->getUnknownConfigArgs();
   StringMapIter iter;

   // check arguments
   bool useMountedPath = true;
   iter = cfg->find(MODESETREADONLY_ARG_UNMOUNTEDPATH);
   if(iter != cfg->end())
   {
      useMountedPath = false;
      cfg->erase(iter);
   }

   if(cfg->empty())
   {
      std::cerr << "No path specified." << std::endl;
      return APPCODE_RUNTIME_ERROR;
   }

   std::string pathStr = cfg->begin()->first;
   cfg->erase(cfg->begin());

   if(ModeHelper::checkInvalidArgs(cfg))
   {
      return APPCODE_INVALID_CONFIG;
   }

   bool readFromStdin = (pathStr == MODESETREADONLY_ARG_READFROMSTDIN);

   if(readFromStdin)
   {
      // read first path from stdin
      pathStr.clear();
      getline(std::cin, pathStr);

      if(pathStr.empty())
      {
         return APPCODE_NO_ERROR;
      }
   }

   do
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
      if (entryInfo.getEntryType() != DirEntryType_DIRECTORY)
      { // setReadOnly 只适用于目录。
         retVal = APPCODE_RUNTIME_ERROR;
         std::cout << "Error: Readonly or readwrite only for directory" << std::endl;
         goto finish_this_entry;
      }

      setRes = setReadOnly(*mgmtNode, entryInfo.getEntryID());
      if(!setRes)
      {
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

void ModeSetReadOnly::printHelp()
{
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
   std::cout << "---------------------------------------" << std::endl;
}

bool ModeSetReadOnly::setReadOnly(Node& mgmtNode, const std::string& entryID)
{
   FhgfsOpsErr setRes;
   SetReadOnlyMsg msg(entryID, setOrNot);

   // 这里发送。
   const auto respMsg = MessagingTk::requestResponse(
         mgmtNode, 
         msg,
         NETMSGTYPE_SetReadOnlyResp);
   if(!respMsg)
   {
      std::cerr << "Failed to communicate with node: " << mgmtNode.getTypedNodeID() << std::endl;
      return false;
   }

   SetReadOnlyRespMsg* respMsgCast = (SetReadOnlyRespMsg*)respMsg.get();

   setRes = (FhgfsOpsErr)respMsgCast->getValue();
   if(setRes != FhgfsOpsErr_SUCCESS)
   {
      std::cerr << "Set dir readonly encountered an error. Please see the management daemon log file for more inforamtion." << std::endl;
      return false;
   }

   return true;
}

void ModeSetReadWrite::printHelp()
{
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
}
