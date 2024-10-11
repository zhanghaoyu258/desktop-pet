#include "widget.h"
#include "QPaintEvent"
#include "QPainter"//绘图
#include "QPixmap"//图片
#include <QCursor>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),
    menu(new QMenu)
{
    this->setWindowFlag(Qt::FramelessWindowHint); //去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->installEventFilter(new DragFilter);

    connect(timer,&QTimer::timeout,[this](){
        static int index=0; //记录显示动作的当前图片
        auto paths = this->action_map.value(this->cur_role_act);
        this->cur_role_pix=paths[index++ % paths.size()];
        //paintEvent();  //不允许
        this->update();
    });
    initMenu();
    loadRoleActRes();

    showActAnination(RoleAct::Stat);
}

Widget::~Widget() {}

void Widget::showActAnination(RoleAct k)
{
    timer->stop();
    this->cur_role_act=k;
    timer->start(60);
}

void Widget::onMenuTriggered(QAction *action)
{
    QMetaEnum me=QMetaEnum::fromType<RoleAct>();
    bool ok;
    int k = me.keyToValue(action->text().toStdString().c_str(),&ok);
    if(!ok)
        return;

    showActAnination(static_cast<RoleAct>(k));
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pix;
    pix.load(this->cur_role_pix.toLocalFile());

    painter.drawPixmap(0,0,pix);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->popup(QCursor::pos());
}

void Widget::loadRoleActRes()
{
    auto addRes=[this](RoleAct k,QString path,int count)
    {
        QList<QUrl> paths;
        char buf[260];
        for(int i=0;i < count;i++){
            memset(buf,0,sizeof(buf));
            sprintf_s(buf,path.toStdString().c_str(),i);
            paths.append(QUrl::fromLocalFile(buf));
        }
        action_map.insert(k,paths);
    };

    addRes(RoleAct::Stat,":/static/img/static/b_%d.png",24);
    addRes(RoleAct::Sleep,":/sleep/img/sleep/sleep_%d.png",24);
    addRes(RoleAct::Swing,":/swings/img/swings/qing_%d.png",32);
}

void Widget::initMenu()
{
     menu->addAction("Stat");
    //menu->addAction("SayHello");
    menu->addAction("Sleep");
    menu->addAction("Swing");

    QAction* act = new QAction("hide");
    connect(act,&QAction::triggered,[this](){
        this->setVisible(false);
    });
    menu->addAction(act);
    connect(this->menu,&QMenu::triggered,this,&Widget::onMenuTriggered);
}
