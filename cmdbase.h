#ifndef CMDBASE_H
#define CMDBASE_H
#include<QObject>

class CmdBase : public QObject
{
    Q_OBJECT
public:
    explicit CmdBase();
    virtual ~CmdBase();
public slots:
    void InputCmd();
signals:
    void HasNewInput(QString strInput);
};

#endif // CMDBASE_H
