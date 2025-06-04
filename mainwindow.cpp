#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mechclock("",0.0,Time(0,0,0),Date(0,0,0),0),
    eleclock("",0.0,Time(0,0,0),Date(0,0,0),100),
    mecheleclock("",0.0,Time(0,0,0),Date(0,0,0),0,100)
{
    //初始化三个钟
    QTime curtime=QTime::currentTime();
    Time time(curtime.hour(),curtime.minute(),curtime.second());
    QDate curdate=QDate::currentDate();
    Date date(curdate.year(),curdate.month(),curdate.day());
    mechclock=MechClock("brand",30.0,time,date,0);

    eleclock=EleClock("brand",10.0,time,date,100);

    mecheleclock=MechEleClock("brand",40.0,time,date,0,100);


    //设置中心的widget，覆盖在mainwindow上
    QWidget*center=new QWidget(this);
    setCentralWidget(center);
    setMinimumSize(800, 600);
    // 允许窗口自由缩放
    center->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //实现上下两部分的布局栏，还未放入任何部件
    layout_main=new QVBoxLayout(center);
    layout_clock=new QHBoxLayout;
    layout_but=new QHBoxLayout;


    //上部分两个表的初始化，ele还未设置值
    Mech_widget=new ClockWidget(mechclock,this);

    Ele_label=new QLabel(this);


    //选择钟的combobox的初始化
    Clock_selector=new QComboBox;
    Clock_selector->addItem("机械表");
    Clock_selector->addItem("电子表");
    Clock_selector->addItem("机械电子表");
    layout_but->addWidget(Clock_selector);

    //根据选择连接信号槽
    QObject::connect(Clock_selector,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
        //qDebug()<<"111";
        if(index==0){
            Ele_label->hide();
            Mech_widget->show();
        }
        if(index==1){
            Mech_widget->hide();
            Ele_label->show();
        }
        if(index==2){
            Mech_widget->show();
            Ele_label->show();
        }
    });

    //初始化两个数组，并将combobox放入了下部分布局栏
    Time_selector.resize(3);
    Date_selector.resize(3);

    for(int i=0;i<3;i++){
        Date_selector[i]=new QComboBox;
        layout_but->addWidget(Date_selector[i]);
    }

    for(int i=0;i<3;i++){
        Time_selector[i]=new QComboBox;
        layout_but->addWidget(Time_selector[i]);
    }

    //设置年的选项
    for (int year = curdate.year() - 10; year <= curdate.year() + 10; ++year) {
        Date_selector[0]->addItem(QString::number(year));
    }
    Date_selector[0]->setCurrentText(QString::number(curdate.year()));

    //设置月的选项
    for(int i=1;i<=12;i++){
        Date_selector[1]->addItem(QString::number(i));
    }
    Date_selector[1]->setCurrentText(QString::number(curdate.month()));

    //设置日的选项
    int daysInMonth = curdate.daysInMonth();
    for (int day = 1; day <= daysInMonth; ++day) {
        Date_selector[2]->addItem(QString::number(day));
    }
    Date_selector[2]->setCurrentText(QString::number(curdate.day()));

    //设置时的选项
    for (int hour = 0; hour < 24; ++hour) {
        Time_selector[0]->addItem(QString::number(hour));
    }
    Time_selector[0]->setCurrentText(QString::number(curtime.hour()));

    //设置分的选项
    for (int minute = 0; minute < 60; ++minute) {
        Time_selector[1]->addItem(QString::number(minute));
    }
    Time_selector[1]->setCurrentText(QString::number(curtime.minute()));

    //设置秒的选项
    for (int second = 0; second < 60; ++second) {
        Time_selector[2]->addItem(QString::number(second));
    }
    Time_selector[2]->setCurrentText(QString::number(curtime.second()));\


    //初始化设置时间的按钮，并将按钮放入下部分布局栏
    Set_time=new QPushButton("设置时间",this);
    //设置时间的连接
    QObject::connect(Set_time,&QPushButton::clicked,
                     this,&MainWindow::set_time);
    layout_but->addWidget(Set_time);

    layout_clock->setContentsMargins(10, 10, 10, 10); // 增加边距
    Mech_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Ele_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 创建两个垂直布局容器
    QVBoxLayout *mechLayout = new QVBoxLayout;
    QVBoxLayout *eleLayout = new QVBoxLayout;

    layout_clock->setAlignment(Qt::AlignCenter);
    layout_but->setAlignment(Qt::AlignHCenter);

    // 为电子表设置文本对齐
    Ele_label->setAlignment(Qt::AlignCenter);

    mechLayout->addWidget(Mech_widget);
    eleLayout->addWidget(Ele_label);

    // 将子布局添加到主时钟区域
    layout_clock->addLayout(mechLayout);
    layout_clock->addLayout(eleLayout);

    layout_main->addLayout(layout_clock,3);
    layout_main->addLayout(layout_but,1);

    //剩下两个待完成一是数字表的label还未赋值，时间更新还未完成
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::update);
    updateTimer->start(1000);

    // 初始更新
    update();

}

MainWindow::~MainWindow() {
    /*delete Ele_label;
    delete Clock_selector;
    delete Set_time;
    delete updateTimer;
    for(int i=0;i<3;i++){
        delete Time_selector[i];
        delete Date_selector[i];
    }*/

}

void MainWindow::set_time()
{
    //qDebug()<<"111";
    int year = Date_selector[0]->currentText().toInt();
    int month = Date_selector[1]->currentText().toInt();
    int day = Date_selector[2]->currentText().toInt();
    int hour = Time_selector[0]->currentText().toInt();
    int minute = Time_selector[1]->currentText().toInt();
    int second = Time_selector[2]->currentText().toInt();

    mechclock.settime(hour, minute, second);
    mechclock.setdate(year, month, day);

    eleclock.settime(hour, minute, second);
    eleclock.setdate(year, month, day);

    mecheleclock.settime(hour, minute, second);
    mecheleclock.setdate(year, month, day);
    //std::cout<<mecheleclock.Clock::time.hour;

    // 强制立即更新
    update();
    Mech_widget->update();
}

void MainWindow::update()
{
    //qDebug()<<"11";
    // 统一更新时间
    mechclock.update();
    eleclock.update();
    mecheleclock.update();

    // 同步所有显示
    Mech_widget->syncTime(mechclock.gettime());

    Time eleTime = eleclock.gettime();
    QString timeStr = QString("%1:%2:%3")
                          .arg(eleTime.hour, 2, 10, QChar('0'))
                          .arg(eleTime.minute, 2, 10, QChar('0'))
                          .arg(eleTime.second, 2, 10, QChar('0'));
    Ele_label->setText(timeStr);
}


void ClockWidget::updateClock()
{
    clock.Clock::update();
    update();
}

void ClockWidget::drawDial(QPainter *painter)
{
    painter->setPen(Qt::black);
    painter->drawEllipse(-96, -96, 192, 192);

    // 绘制刻度
    for (int i = 0; i < 60; ++i) {
        painter->save();
        painter->rotate(6 * i);
        if (i % 5 == 0) {
            painter->setPen(QPen(Qt::black, 3));
            painter->drawLine(0, -88, 0, -96);
        } else {
            painter->setPen(QPen(Qt::black, 1));
            painter->drawLine(0, -92, 0, -96);
        }
        painter->restore();
    }

    // 绘制数字
    for (int i = 1; i <= 12; ++i) {
        painter->save();
        painter->rotate(30 * i);
        painter->drawText(-5, -70, 10, 10, Qt::AlignCenter, QString::number(i));
        painter->restore();
    }
}

void ClockWidget::drawHands(QPainter *painter)
{
    Time time = clock.gettime();

    // 绘制时针
    painter->save();
    painter->rotate(30.0 * ((time.hour % 12) + time.minute / 60.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    QPolygon hourHand;
    hourHand << QPoint(7, 8) << QPoint(-7, 8) << QPoint(0, -40);
    painter->drawConvexPolygon(hourHand);
    painter->restore();

    // 绘制分针
    painter->save();
    painter->rotate(6.0 * (time.minute + time.second / 60.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    QPolygon minuteHand;
    minuteHand << QPoint(7, 8) << QPoint(-7, 8) << QPoint(0, -70);
    painter->drawConvexPolygon(minuteHand);
    painter->restore();

    // 绘制秒针
    painter->save();
    painter->rotate(6.0 * time.second);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QPolygon secondHand;
    secondHand << QPoint(2, 4) << QPoint(-2, 4) << QPoint(0, -90);
    painter->drawConvexPolygon(secondHand);
    painter->restore();
}
