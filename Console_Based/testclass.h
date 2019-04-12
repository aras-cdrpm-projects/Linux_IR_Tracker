#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>

class TestClass : public QObject
{
    Q_OBJECT
public:
    explicit TestClass(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TESTCLASS_H