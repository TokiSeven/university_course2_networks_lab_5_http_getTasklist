#include "ts_system.h"
#include <psapi.h>
#include <QDebug>

TS_System::TS_System()
{
    qDebug() << "TS_System >> Object created.";

    //hSnap - toolhelp for 32Snapshot
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //it should be not NULL, or we can't read processes
    if (hSnap == NULL)
    {
        //create QMessageBox to get allert to administrator (programmer, who will use this programm)
        QMessageBox::critical(0, "Error!", "Error Load ToolHelp", QMessageBox::Close);
        return;
    }

    //proc - current process
    PROCESSENTRY32 proc = { sizeof(proc) };

    //return null, if was been BAD
    if (Process32First(hSnap, &proc))
    {
        //some initialization
        QString filename;//filename of process
        TS_Process pr;//current process will be here
        HANDLE hProcess; // process for memory
        PROCESS_MEMORY_COUNTERS pmc; // memory's store

        //open information about current process
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);

        //modify name
        filename = copyToQString(proc.szExeFile);

        //get memory info about process
        GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));

        //initialization TS_Process
        pr.name = filename;
        pr.PID = proc.th32ProcessID;
        pr.memory = pmc.WorkingSetSize / 1024 / 8;

        //insert in DB current TS_Process
        win32sysMap[proc.th32ProcessID] = pr;

        //go to all process (Process32Next returns process, which there is after the last)
        while (Process32Next(hSnap, &proc))
        {
            //open information about current process
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);

            //modify name
            filename = copyToQString(proc.szExeFile);

            //get memory info about process
            GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));

            //initialization TS_Process
            pr.name = filename;
            pr.PID = proc.th32ProcessID;
            pr.memory = pmc.WorkingSetSize / 1024 / 8;

            //insert in DB current TS_Process
            win32sysMap[proc.th32ProcessID] = pr;
        }
    }
}

//name's process of WinAPI is WCHAR, this func convert it into QString
QString TS_System::copyToQString(WCHAR array[MAX_PATH])
{
    qDebug() << "TS_System >> copyToQString(WCHAR array[MAX_PATH]);";

    QString string;
    int i = 0;

    while (array[i] != 0)
    {
        string[i] = array[i];
        i++;
    }

    return string;
}

//return QList<TS_Process> from Map<int, TS_Process>
QList<TS_Process> TS_System::getAllProcessList()
{
    qDebug() << "TS_System >> getAllProcessList();";

    return win32sysMap.values();
}
