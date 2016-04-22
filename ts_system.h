#ifndef TS_SYSTEM_H
#define TS_SYSTEM_H

#include <windows.h>
#include <w32api.h>
#include <tlhelp32.h>

#include <QMap>
#include <QString>
#include <QMessageBox>

//TS_Process - ministructure for store one process (name, used memory, PID)
struct TS_Process
{
    QString name;
    SIZE_T memory;
    int PID;
};

class TS_System
{
public:
    //Here there are get all processes
    TS_System();

    //return all processes in QList<TS_Process>
    QList<TS_Process> getAllProcessList();
private:
    //there is a DB for store all process (int - 32PID)
    QMap<int, TS_Process> win32sysMap;

    //modify WCHAR to QString (name of process)
    QString copyToQString(WCHAR array[MAX_PATH]);

};

#endif // TS_SYSTEM_H
