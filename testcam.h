#ifndef TESTCAM_H
#define TESTCAM_H

#include <QThread>
#include <QMutex>
#include <QList>
#include <QImage>

// 虚拟相机类，定时发出图像
class TestCam : public QThread
{
    Q_OBJECT
public:
    TestCam();
    ~TestCam();

    void startCapture();
    void stopCapture();
signals:
    void imageReady(const QImage &img);
protected:
    void run() override;
private:
    uchar* buffer;
    bool startRunning = false;
    uint iterN = 0;
    QList<QImage> imgList;
    QMutex mutex;
};

#endif // TESTCAM_H
