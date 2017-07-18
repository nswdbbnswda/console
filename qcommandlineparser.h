#ifndef QCOMMANDLINEPARSER_H
#define QCOMMANDLINEPARSER_H
#include<QObject>


class QCommandLineParser : public QObject
{
    Q_OBJECT
public:
    explicit QCommandLineParser();
    virtual ~QCommandLineParser();

};

#endif // QCOMMANDLINEPARSER_H
