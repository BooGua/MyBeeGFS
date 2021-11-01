#include <mutex>
#include <common/toolkit/StorageTk.h>

#include <program/Program.h>
#include "ReadonlyManager.h"

// 读取 readOnly 文件中的 dentryies 保存到 set 中。
bool ReadonlyManager::loadFromFile()
{
   LogContext log("ReadonlyManager (load)");

   bool retVal = false;
   int readRes;

   if (!StorageTk::pathExists(storePath))
   {
      return true;
   }

   const std::lock_guard<Mutex> lock(storeMutex);

   // 准备打开 storePath 这个文件了。
   int fd = open(storePath.c_str(), O_RDONLY, 0);
   if (fd == -1)
   {
      log.logErr("Unable to open file:" + storePath + ". SysErr: " + 
            System::getErrString());
      return false;
   }

   struct stat stat;
   if (fstat(fd, &stat) != 0)
   {
      log.logErr("Unable to open file:" + storePath + ". SysErr: " + 
            System::getErrString());
      close(fd);
      return false;
   }

   char buf[stat.st_size + 1]; // 读取文件内容。
   readRes = read(fd, buf, stat.st_size);
   buf[stat.st_size] = '\0';
   if(readRes <= 0)
   {
      log.logErr("Unable to read file:" + storePath + ". SysErr: " + 
            System::getErrString());
   } else {
      // 用空格连接 entry。
      StringTk::explodeEx(buf, ' ', true, &readonlyEntries);
      retVal = true;
   }

   close(fd);
   return retVal;
}

bool ReadonlyManager::saveToFile()
{
   LogContext log("ReadonlyManager (save)");
   bool retVal = false;
   const std::lock_guard<Mutex> lock(storeMutex);

   if (readonlyEntries.empty())
   {
      // art TODO 这是干啥呢？
      int unlinkRes = unlink(storePath.c_str());
      if ((unlinkRes != 0) && (errno != ENOENT))
      {
         log.logErr("Could not delete file: " + storePath + ". SysErr: " + System::getErrString());
         return false;
      }
      return true;
   }

   std::string buf = StringTk::implode(' ', readonlyEntries, true);
   ssize_t writeRes;
   int renameRes;

   std::string storePathTmp(storePath + ".tmp");

   // create/trunc file
   int openFlags = O_CREAT|O_TRUNC|O_WRONLY;
   int fd = open(storePathTmp.c_str(), openFlags, 0666);
   if (fd == -1)
   {
      log.logErr("Unable to create file: " + storePath + ". SysErr: " + System::getErrString());
      return false;
   }

   writeRes = write(fd, buf.c_str(), buf.length());
   if (writeRes != (int)buf.length())
   {
      log.logErr("Unable to store file: " + storePath + ". SysErr: " + System::getErrString());
      goto err_closefile;
   }

   close(fd);

   renameRes = rename(storePathTmp.c_str(), storePath.c_str());
   if(renameRes == -1)
   {
      log.logErr("Unable to rename file: " + storePath + ". SysErr: " + System::getErrString());
      goto err_unlink;
   }

   retVal = true;

   LOG_DEBUG_CONTEXT(log, Log_DEBUG, "readonlyEntries file stored: " + storePath);
   return retVal;

   // error compensation
err_closefile:
   close(fd);

err_unlink:
   unlink(storePathTmp.c_str());

   return retVal;
}

bool ReadonlyManager::isEntryReadonly(const std::string& entryID)
{
   return readonlyEntries.find(entryID) != readonlyEntries.end();
}

bool ReadonlyManager::updateReadonlyEntries(const std::string& entryID, bool setOrNot)
{
   if (setOrNot)
   {
      readonlyEntries.insert(entryID);
   } else {
      readonlyEntries.erase(entryID);
   }
   return saveToFile();
}

std::string ReadonlyManager::getReadonlyEntries()
{
   return StringTk::implode(' ', readonlyEntries, true);
}
