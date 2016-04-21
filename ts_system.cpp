#include "ts_system.h"

#include "windows.h"
#include "TlHelp32.h"
#include "Psapi.h"
#include "BaseTsd.h"
#include "WinDef.h"
#include "WinNT.h"
#include <QFile>
#include <QDebug>
#include <fstream>

using namespace std;

TS_System::TS_System(QString cmd, QObject *parent)
    : QObject(parent)
{
    this->command = cmd;
    this->filename = "index.html";

    getInfoIntoFile();
    readData();
}

void TS_System::createIndex()
{
    this->private_createIndex();
}

void TS_System::slot_createIndex()
{
    this->createIndex();
}

void TS_System::private_createIndex()
{
    PROCESSENTRY32 peProcessEntry;
    PROCESS_MEMORY_COUNTERS pmc; // Память
    HANDLE hProcess; //Память
    int lnumber = 0;
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
                TH32CS_SNAPPROCESS, 0);
    if(INVALID_HANDLE_VALUE == hSnapshot) {
        return;
    }
    Process32First(hSnapshot, &peProcessEntry);

    ofstream myfile;
    myfile.open(this->filename.toStdString().c_str());
    myfile << "<table style='width:350px;margin: 0 auto;'>\n"
           << "\n<tr><td><b>Processes:</b></td>\n<td><b>Sizes:</b></td></tr>\n";
    do
    {
        ++lnumber;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, peProcessEntry.th32ProcessID );

        myfile << "\n<tr><td>" << lnumber << ") " << peProcessEntry.szExeFile << "</td>\n";

        GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
        myfile << "<td>" << pmc.WorkingSetSize << "</td></tr>\n";
        qDebug() << QString::number(lnumber);
    }
    while(Process32Next(hSnapshot, &peProcessEntry));
    myfile << "\n</table>\n";
    myfile.close();
    CloseHandle(hSnapshot);

}

void TS_System::readData()
{
    QFile file(this->filename);
    if (!file.open(QIODevice::ReadOnly))
        return;
    this->data = file.readAll();
}

void TS_System::getInfoIntoFile()
{
    this->private_createIndex();
}
