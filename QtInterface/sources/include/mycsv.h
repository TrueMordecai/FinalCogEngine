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
    void read_file();
    void set_filename(std::string filename);
    std::string getFilename();
    std::string getDefaultScenario();
    std::string getFileStream();
    QString getNewLine();

private:
    const std::string C_DEFAULT_FILEPATH = "/home/ZeBoss/CognitiveEngine/CogEngineArduino/QtInterface/scn_0.csv";
    std::string m_filename;
    std::vector<std::string> m_linesData;
    std::string m_filestream;
    int m_line_read;
};

#endif // MYCSV_H
