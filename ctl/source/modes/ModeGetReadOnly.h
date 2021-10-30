#ifndef MODEGETREADONLY_H_
#define MODEGETREADONLY_H_

class ModeGetReadOnly:public Mode{
public:
   ModeGetReadOnly(){}

   virtual int execute();

   static void printHelp();

private:
   bool getReadOnly(
         Node& mgmtNode,
         const std::string& path,
         const std::string& entryID);
};

#endif /*MODEGETREADONLY_H_*/
