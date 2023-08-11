#ifndef PREVGLWIDGET_H
#define PREVGLWIDGET_H

#include <QtOpenGLWidgets>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QElapsedTimer>

class PrevGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit PrevGLWidget(QWidget *parent = nullptr);
    ~PrevGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;

public slots:
    void slotGetNewImage(const QImage& imgIN);

private:
    QImage img;
    unsigned int paintN = 0;

    QOpenGLTexture* texture = nullptr;
    QOpenGLShaderProgram* program = nullptr;
    QOpenGLBuffer vbo;

    int vertexAttr = 0;
    int texCoordAttr = 0;
    int matrixUniform = 0;
    int textureUniform = 0;

    unsigned int m_frames = 0;
    QElapsedTimer m_time;
};

#endif // PREVGLWIDGET_H
