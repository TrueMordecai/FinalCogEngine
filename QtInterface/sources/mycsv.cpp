#include "mycsv.h"
#include <QDebug>
#include <string>

myCsv::myCsv(std::string filename)
{
    if (!filename.empty())
        this->setFilename(filename);
    this->readFile();
}

std::string myCsv::getFileStream()
{
    return (this->m_fileStream);
}

QString myCsv::getNewLine()
{
    m_lineRead++;
    if (this->m_linesData[this->m_lineRead].size() > 1)
        return (this->m_linesData[this->m_lineRead].data());
    else {
        return (nullptr);
    }
}

void myCsv::readFile()
{
    m_lineRead = -1;
    std::ifstream file;
    std::string line;
    file.open(m_fileName.data());
    if (!file.good()) {
        file.open(C_DEFAULT_FILEPATH);
        setFilename(C_DEFAULT_FILEPATH);
    }
    if (file) {
        std::string data;
        while(getline(file, data)) {
            std::string datas = data;
            this->m_linesData.push_back(datas.data());
            this->m_fileStream += datas.data();
            this->m_fileStream += '\n';
        }
    }
}

void myCsv::setFilename(std::string filename)
{
    this->m_fileName = filename;
    m_fileName = std::string(filename);
}

std::string myCsv::getFilename()
{
    return (this->m_fileName);
}

std::string myCsv::getDefaultScenario()
{
    return (C_DEFAULT_FILEPATH);
}
