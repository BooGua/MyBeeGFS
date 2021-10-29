#include <common/toolkit/BuildTypeTk.h>
#include "Program.h"
#include <iostream>

App* Program::app = NULL;

int Program::main(int argc, char** argv)
{
   BuildTypeTk::checkDebugBuildTypes();

   // must be called before creating a new App
   AbstractApp::runTimeInitsAndChecks();

   app = new App(argc, argv);

   app->startInCurrentThread(); // go 202110291800

   int appRes = app->getAppResult();

   delete app;

   return appRes;
}


