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
#include <QPoint>
#include <QResizeEvent>

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
    void customMenuRequested(QPoint pos);

private slots:
    void addTask();
    void removeTask( int row);

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
