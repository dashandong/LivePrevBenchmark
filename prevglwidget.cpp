#include "prevglwidget.h"

#include <QPainter>

static const GLfloat vertData[] = {
     1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f, 0.0f
};

PrevGLWidget::PrevGLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{

}

PrevGLWidget::~PrevGLWidget()
{

}

void PrevGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    QOpenGLShader* vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char* vsrc =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader* fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char* fsrc =
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texc.st);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();

    vertexAttr = program->attributeLocation("vertex");
    texCoordAttr = program->attributeLocation("texCoord");
    matrixUniform = program->uniformLocation("matrix");
    textureUniform = program->uniformLocation("texture");

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData, 20*sizeof(GLfloat));
    vbo.release();

    program->bind();
    program->setUniformValue("texture", 0);
}

void PrevGLWidget::paintGL()
{
    if (img.isNull()) return;
    QPainter painter;
    painter.begin(this);

    painter.beginNativePainting();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 modelview;
    modelview.ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);

    texture = new QOpenGLTexture(img);

    program->bind();
    program->setUniformValue(matrixUniform, modelview);
    texture->bind();
    program->setUniformValue(textureUniform, 0);

    program->enableAttributeArray(vertexAttr);
    program->enableAttributeArray(texCoordAttr);
    vbo.bind();
    program->setAttributeBuffer(vertexAttr, GL_FLOAT, 0, 3, 5*sizeof(GL_FLOAT));
    program->setAttributeBuffer(texCoordAttr, GL_FLOAT, 3*sizeof(GL_FLOAT), 2, 5*sizeof(GL_FLOAT));
    vbo.release();

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    program->disableAttributeArray(texCoordAttr);
    program->disableAttributeArray(vertexAttr);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    painter.endNativePainting();

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
}

void PrevGLWidget::slotGetNewImage(const QImage &imgIN)
{
    img = imgIN.copy();
    this->update();
}
