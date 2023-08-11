#ifndef PREVQWIDGET_H
#define PREVQWIDGET_H

#include <QWidget>
#include <QElapsedTimer>

class PrevQWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrevQWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void slotGetNewImage(const QImage& imgIN);

private:
    QImage img;

    unsigned int m_frames = 0;
    QElapsedTimer m_time;
};

#endif // PREVQWIDGET_H
