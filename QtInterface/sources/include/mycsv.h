#ifndef MYCSV_H
#define MYCSV_H

#include <iostream>
#include <fstream>
#include <vector>
#include <QString>

class myCsv
{
public:
    myCsv(std::string filename);
    void readFile();
    void setFilename(std::string filename);
    std::string getFilename();
    std::string getDefaultScenario();
    std::string getFileStream();
    QString getNewLine();

private:
    const std::string C_DEFAULT_FILEPATH = "/home/ZeBoss/CognitiveEngine/CogEngineArduino/QtInterface/scn_0.csv";
    std::string m_fileName;
    std::vector<std::string> m_linesData;
    std::string m_fileStream;
    int m_lineRead;
};

#endif // MYCSV_H
