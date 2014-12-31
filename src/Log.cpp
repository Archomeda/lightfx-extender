#include <iostream>
#include <fstream>
#include <time.h>
#include "Log.h"

#define LOGFILE "E:\\AlienFXFramework.log"

using namespace std;
using namespace lightfx;


#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21
static string getDtTm() {
    char buff[DTTMSZ];
    time_t t = time(0);
    tm lt;
    localtime_s(&lt, &t);
    strftime(buff, DTTMSZ, DTTMFMT, &lt);
    return string(buff);
}


void lightfx::Log(string line) {
    //ofstream logfile;
    //logfile.open(LOGFILE, ios::out | ios::app);
    //logfile << getDtTm() << line << "\n";
    //logfile.close();
}
