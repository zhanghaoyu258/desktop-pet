#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>

class QPaintEvent;

namespace Act{
Q_NAMESPACE
enum RoleAct{
    Stat,
    Swing,
    Sleep,
    SayHello
};
Q_ENUM_NS(RoleAct)
}

using namespace Act ;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void showActAnination(RoleAct k);

public slots:
    void onMenuTriggered(QAction* action);

protected:
    void paintEvent(QPaintEvent* event)override;
    void contextMenuEvent(QContextMenuEvent* event);

private:
    void loadRoleActRes();
    void initMenu();

private:
    QMap<RoleAct,QList<QUrl>> action_map;
    QTimer* timer;
    RoleAct cur_role_act;
    QUrl cur_role_pix;
    QMenu* menu;
};

class DragFilter:public QObject{
public:
    bool eventFilter(QObject* obj,QEvent* event)
    {
        auto w=dynamic_cast<QWidget*>(obj);
        if(!w)
            return false;
        if(event->type() == QEvent::MouseButtonPress)
        {
            auto e=dynamic_cast<QMouseEvent*>(event);
            if(e)
            {
                pos=e->pos();
                //qDebug() << pos;
            }
        }
        else if(event->type() == QEvent::MouseMove)
        {
            auto e=dynamic_cast<QMouseEvent*>(event);
            //qDebug() << e->pos();
            if(e)
            {
                if(e->buttons()&Qt::MouseButton::LeftButton)
                {
//                    qDebug() << "move";
                    w->move(e->globalPosition().toPoint()-pos);
                }
            }
        }
        return  QObject::eventFilter(obj,event);
    }
private:
    QPoint pos;
};
#endif // WIDGET_H
