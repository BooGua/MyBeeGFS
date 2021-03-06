#ifndef CONFIG_H_
#define CONFIG_H_

#include <common/app/config/AbstractConfig.h>
#include <modes/Mode.h>

#include <memory>


#define CONFIG_DEFAULT_CFGFILENAME "/etc/beegfs/beegfs-client.conf"

#define RUNMODE_HELP_KEY_STRING     "--help" /* key for usage help */

struct RunModesElem // from 202110301156
{
   const char* modeString;
   bool hidden;

   std::unique_ptr<Mode> (*instantiate)();
   void (*printHelp)();

   bool needsCommunication;
};

extern RunModesElem const __RunModes[];

// AbstractConfig go 202110301100
class Config : public AbstractConfig // from 202110301054
{
   public:
      Config(int argc, char** argv);

      const RunModesElem* determineRunMode();

   private:

      // configurables
      std::string mount; //special argument to detect configuration file by mount point

      bool        logEnabled;

      std::string connInterfacesFile;

      unsigned    tuneNumWorkers;
      std::string tunePreferredMetaFile;
      std::string tunePreferredStorageFile;

      bool        runDaemonized;

      // internals

      virtual void loadDefaults(bool addDashes) override;
      virtual void applyConfigMap(bool enableException, bool addDashes) override;
      virtual void initImplicitVals() override;
      std::string createDefaultCfgFilename() const;


   public:
      // getters & setters
      StringMap* getUnknownConfigArgs() // note: modified inside the modes
      {
         return &configMap;
      }

      const std::string& getMount() const
      {
         return mount;
      }

      bool getLogEnabled() const
      {
         return logEnabled;
      }

      const std::string& getConnInterfacesFile() const
      {
         return connInterfacesFile;
      }

      unsigned getTuneNumWorkers() const
      {
         return tuneNumWorkers;
      }

      const std::string& getTunePreferredMetaFile() const
      {
         return tunePreferredMetaFile;
      }

      const std::string& getTunePreferredStorageFile() const
      {
         return tunePreferredStorageFile;
      }

      bool getRunDaemonized() const
      {
         return runDaemonized;
      }
};

#endif /*CONFIG_H_*/
