#include "prevqwidget.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>

PrevQWidget::PrevQWidget(QWidget *parent) : QWidget(parent)
{
}

void PrevQWidget::paintEvent(QPaintEvent *event)
{
    if (img.isNull()) return;
    QPainter painter(this);
    painter.drawImage(this->rect(), img);

    if (const int elapsed = m_time.elapsed())
    {
        QString framesPerSecond;
        framesPerSecond.setNum(m_frames / (elapsed / 1000.0), 'f', 2);
        painter.setPen(Qt::red);
        painter.drawText(10, 20, framesPerSecond + " fps");
    }

    painter.end();

    if (!(m_frames % 100)) {
        m_time.start();
        m_frames = 0;
    }
    ++m_frames;

    img = QImage();
    //QApplication::processEvents();
    return;
}

void PrevQWidget::slotGetNewImage(const QImage &imgIN)
{
    img = imgIN.copy();
    this->update();
    return;
}


