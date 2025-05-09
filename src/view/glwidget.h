#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

extern "C" {
#include "../model/object.h"
}

enum PointStyle { NONE = 0, CIRCLE, SQUARE };

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

 public:
  explicit GLWidget(QWidget *parent = nullptr);
  ~GLWidget();

  QColor getEdgeColor();
  QColor getVertexColor();
  QColor getBackgroundColor();

  std::string getObjectName();
  std::size_t getVerticesAmount();
  std::size_t getFacesAmount();

  void resetUniforms();
  void loadObjectFromFile(const QString &path);

  void setEdgeColor(const QColor &color);
  void setVertexColor(const QColor &color);
  void setBackgroundColor(const QColor &color);

  void setScale(const float val);
  void setProjection(const int type);
  void move(const float x, const float y, const float z);
  void rotate(const float x, const float y, const float z);

  void setLineStyle(bool dashed, float width);
  void setPointStyle(int style, float size);

 signals:
  void loaded();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  void cleanup();

  Object *obj;
  GLuint VAO, VBO, EBO;

  GLubyte lineWidth;
  bool lineDashed;
  float dashSize, gapSize;
  int pointStyle;
  GLfloat pointSize;
  float bgColor[3], eColor[3], vColor[3];

  float moveVector[3] = {0};
  float scaleMatrix[16] = {0}, rotationMatrix[16] = {0},
        projectionMatrix[16] = {0};

  QOpenGLShaderProgram prog;
  GLint moveLoc, scaleLoc, transformLoc, projectionLoc, colorLoc;
  GLint uDashedLoc, uDashSizeLoc, uGapSizeLoc, uPointSizeLoc;
};

#endif  // GLWIDGET_H
