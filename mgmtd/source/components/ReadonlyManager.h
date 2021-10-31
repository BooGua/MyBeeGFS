#ifndef READONLYMANAGER_H_
#define READONLYMANAGER_H_

#include <app/config/Config.h>
#include <common/app/log/LogContext.h>
#include <common/Common.h>
#include <common/toolkit/StringTk.h>

class ReadonlyManager
{
   public:
      ReadonlyManager() {}

      // 读取 readOnly 文件中的 dentryies 保存到 set 中。
      bool loadFromFile();

      bool saveToFile();
      bool isEntryReadonly(const std::string& entryID);
      bool updateReadonlyEntries(const std::string& entryID, bool setOrNot);
      std::string getReadonlyEntries();
      // bool loadReadonlyEntries();
      // bool updateReadonlyEntries(const std::string& entryID, bool setOrNot);
      // std::string GetReadOnlyEntries();
      // bool isEntryReadonly(const std::string& entryID);

   protected:
      LogContext log;

   private:
      Mutex storeMutex;
      // 保存 readonly 的文件。
      std::string storePath;
      StringSet readonlyEntries;

   public:
      // getters & setters
      void setStoragePath(std::string storePath)
      {
         this->storePath = storePath;
      }
};

#endif /*READONLYMANAGER_H_*/
