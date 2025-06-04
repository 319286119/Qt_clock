#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QString>
#include <QDebug>
#include <iostream>

class Time{
public:
    int hour,minute,second;
    Time(int h,int m,int s):hour(h),minute(m),second(s){}
    Time(const Time&t):hour(t.hour),minute(t.minute),second(t.second){};
};


class Date{
public:
    int year,month,day;
    Date(int y,int m,int d):year(y),month(m),day(d){};
    Date(const Date&d):year(d.year),month(d.month),day(d.day){};
};


class Clock{
public:
    Time time;
    Date date;
    std::string brand;
    float price;


public:
    Clock(const std::string b,const float p,const Time&t,const Date&d):brand(b),price(p),time(t),date(d){};

    Clock(const Clock& other) : brand(other.brand), price(other.price), time(other.time), date(other.date) {}

    void update(){
        //qDebug() << "Before update: " << time.hour << ":" << time.minute << ":" << time.second;
        time.second++;
        if(time.second>59){
            time.minute+=1;
            time.second=0;
        }
        if(time.minute>59){
            time.hour+=1;
            time.minute=0;
        }
        time.hour%=24;
        //qDebug() << "After update: " << time.hour << ":" << time.minute << ":" << time.second;
    }
    void settime(int h,int m,int s){
        time.hour=h;
        time.minute=m;
        time.second=s;
    }
    void setdate(int y,int m,int d){
        date.year=y;
        date.month=m;
        date.day=d;
    }

    Time gettime(){
        return time;
    }
};


class MechClock:virtual public Clock{
private:
    int spring;
public:

    MechClock(const std::string&b,float p,const Time&t,const Date&d,int s):Clock(b,p,t,d),spring(s){}
    MechClock(const MechClock&m):Clock(m),spring(m.spring){}
};

class EleClock:virtual public Clock{
private:
    int battery;

public:

    EleClock(const std::string&b,const float p,const Time&t,const Date&d,int ba):Clock(b,p,t,d),battery(ba){}
};



class MechEleClock:public EleClock,public MechClock{
public:
    MechEleClock(const std::string&b,const float p,const Time&t,const Date&d,int s,int ba)
        :Clock(b,p,t,d),EleClock(b,p,t,d,ba),MechClock(b,p,t,d,s){}
};

class ClockWidget : public QWidget
{
    Q_OBJECT

private:
    MechClock clock;
    QTimer* timer;
public:
    explicit ClockWidget(const MechClock& c,QWidget *parent = nullptr):clock(c),QWidget(parent){
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ClockWidget::updateClock);
        timer->start(1000);
    };
public slots:
    void syncTime(const Time& newTime) {
        clock.settime(newTime.hour, newTime.minute, newTime.second);
        update();
    }
protected:
    void paintEvent(QPaintEvent* event) override {
        //qDebug() << "paintEvent called";
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 移动坐标系到窗口中心
        int side = qMin(width(), height());
        painter.translate(width() / 2, height() / 2);
        painter.scale(side / 200.0, side / 200.0);

        // 绘制表盘
        drawDial(&painter);
        // 绘制指针
        drawHands(&painter);
    }
private:
    void updateClock();

    void drawDial(QPainter*painter);

    void drawHands(QPainter*painter);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void set_time();
    void update();

private:

    MechClock mechclock;
    EleClock eleclock;
    MechEleClock mecheleclock;

    QVBoxLayout*layout_main;
    QHBoxLayout*layout_clock;
    QHBoxLayout*layout_but;

    ClockWidget*Mech_widget;
    QLabel*Ele_label;

    QComboBox*Clock_selector;
    std::vector<QComboBox*>Time_selector;
    std::vector<QComboBox*>Date_selector;
    QPushButton*Set_time;

    QTimer* updateTimer;

};
#endif // MAINWINDOW_H
