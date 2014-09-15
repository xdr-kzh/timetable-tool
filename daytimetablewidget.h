#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

#include <QMap>
#include <QList>

class DayTimeTableWidget : public QGroupBox
{
    Q_OBJECT
public:
    typedef QMap< QString, QString> ValueMap;
    typedef QSharedPointer<ValueMap> ValueMapPtr;
    typedef QList< ValueMapPtr> ValueList;

public:
    explicit DayTimeTableWidget( int dayNumber, QWidget *parent = 0);
    int getDayNumber(){ return dayNumber_;}

public slots:
    ValueList* saveTask();
    void clear();

private slots:
    void addTask();

private:
    QPushButton* addTaskButton_;
    QLabel* nameLabel_;
    QTableWidget* tableWidget_;
    QVBoxLayout* mainLayout_;

    int dayNumber_;
    ValueList valueList_;

    static const int EVENT_COUNT;
    static const int MINIMUM_HEIGHT;
    static const int MILITARY_EVENT_NUMBER;
};

#endif // ADDTASKWIDGET_H
