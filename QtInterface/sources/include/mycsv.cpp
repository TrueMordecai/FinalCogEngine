#include "mycsv.h"
#include <QDebug>
#include <string>

myCsv::myCsv(std::string filename)
{
    if (!filename.empty())
        this->set_filename(filename);
    this->read_file();
}

std::string myCsv::getFileStream()
{
    return (this->m_filestream);
}

QString myCsv::getNewLine()
{
    m_line_read++;
    if (this->m_linesData[this->m_line_read].size() > 1)
        return (this->m_linesData[this->m_line_read].data());
    else {
        return (nullptr);
    }
}

void myCsv::read_file()
{
    m_line_read = -1;
    std::ifstream file;
    std::string line;
    file.open(m_filename.data());
    if (!file.good()) {
        file.open(C_DEFAULT_FILEPATH);
        set_filename(C_DEFAULT_FILEPATH);
    }
    if (file) {
        std::string data;
        while(getline(file, data)) {
            std::string datas = data;
            this->m_linesData.push_back(datas.data());
            this->m_filestream += datas.data();
            this->m_filestream += '\n';
        }
    }
}

void myCsv::set_filename(std::string filename)
{
    this->m_filename = filename;
    m_filename = std::string(filename);
    qDebug() << m_filename.data();
}

std::string myCsv::getFilename()
{
    return (this->m_filename);
}

std::string myCsv::getDefaultScenario()
{
    return (C_DEFAULT_FILEPATH);
}
