#include "glwidget.h"

extern "C" {
#include "../model/affine.h"
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      obj(nullptr),
      VAO(0),
      VBO(0),
      EBO(0),
      lineWidth(0),
      lineDashed(false),
      dashSize(2.0f),
      gapSize(2.0f),
      pointStyle(PointStyle::NONE),
      pointSize(0.0f) {}

GLWidget::~GLWidget() { cleanup(); }

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

  prog.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl");
  prog.link();
  prog.bind();

  moveLoc = prog.uniformLocation("move");
  scaleLoc = prog.uniformLocation("scale");
  transformLoc = prog.uniformLocation("transform");
  projectionLoc = prog.uniformLocation("projection");
  colorLoc = prog.uniformLocation("uColor");

  uDashedLoc = prog.uniformLocation("uDashed");
  uDashSizeLoc = prog.uniformLocation("uDashSize");
  uGapSizeLoc = prog.uniformLocation("uGapSize");
  uPointSizeLoc = prog.uniformLocation("uPointSize");

  prog.release();
  resetUniforms();
}

void GLWidget::paintGL() {
  glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  prog.bind();

  if (obj != nullptr) {
    glLineWidth(lineWidth);
    glUniform3fv(colorLoc, 1, eColor);
    glUniform1i(uDashedLoc, lineDashed);
    glUniform1f(uDashSizeLoc, dashSize);
    glUniform1f(uGapSizeLoc, gapSize);

    glUniform3fv(moveLoc, 1, moveVector);
    glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, scaleMatrix);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rotationMatrix);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);

    glDrawElements(GL_LINE_LOOP, obj->n_faces * 3, GL_UNSIGNED_INT, 0);

    if (pointStyle != PointStyle::NONE) {
      (pointStyle == PointStyle::CIRCLE) ? glEnable(GL_POINT_SMOOTH)
                                         : glDisable(GL_POINT_SMOOTH);
      glUniform3fv(colorLoc, 1, vColor);
      glUniform1f(uPointSizeLoc, pointSize);
      glDrawElements(GL_POINTS, obj->n_faces * 3, GL_UNSIGNED_INT, 0);
    }
  }

  prog.release();
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

QColor GLWidget::getEdgeColor() {
  QColor c = QColor();
  c.setRedF(eColor[0]);
  c.setGreenF(eColor[1]);
  c.setBlueF(eColor[2]);

  return c;
}

QColor GLWidget::getVertexColor() {
  QColor c = QColor();
  c.setRedF(vColor[0]);
  c.setGreenF(vColor[1]);
  c.setBlueF(vColor[2]);

  return c;
}

QColor GLWidget::getBackgroundColor() {
  QColor c = QColor();
  c.setRedF(bgColor[0]);
  c.setGreenF(bgColor[1]);
  c.setBlueF(bgColor[2]);

  return c;
}

std::string GLWidget::getObjectName() {
  return obj != nullptr ? obj->name : "";
}

std::size_t GLWidget::getVerticesAmount() {
  return obj != nullptr ? obj->n_vertices : 0;
}

std::size_t GLWidget::getFacesAmount() {
  return obj != nullptr ? obj->n_faces : 0;
}

void GLWidget::loadObjectFromFile(const QString &path) {
  cleanup();
  makeCurrent();

  obj = create_obj(path.toStdString().c_str());
  if (obj == nullptr) {
    return;
  }

  const GLuint size_v = sizeof(float) * obj->n_vertices * 3;
  const GLuint size_f = sizeof(int) * obj->n_faces * 3;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size_v, obj->vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_f, obj->faces, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  update();
  doneCurrent();
  emit loaded();
}

void GLWidget::cleanup() {
  makeCurrent();

  if (VBO) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  if (EBO) {
    glDeleteBuffers(1, &EBO);
    EBO = 0;
  }

  if (VAO) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }

  if (obj != nullptr) {
    destroy_obj(obj);
    obj = nullptr;
  }

  doneCurrent();
}

void GLWidget::setBackgroundColor(const QColor &color) {
  bgColor[0] = color.redF();
  bgColor[1] = color.greenF();
  bgColor[2] = color.blueF();
  update();
}

void GLWidget::setEdgeColor(const QColor &color) {
  eColor[0] = color.redF();
  eColor[1] = color.greenF();
  eColor[2] = color.blueF();
  update();
}

void GLWidget::setVertexColor(const QColor &color) {
  vColor[0] = color.redF();
  vColor[1] = color.greenF();
  vColor[2] = color.blueF();
  update();
}

void GLWidget::setProjection(const int type) {
  if (type) {
    perspective_projection(projectionMatrix);
  } else {
    ortho_projection(projectionMatrix);
  }

  update();
}

void GLWidget::setScale(const float val) {
  scale(val, scaleMatrix);
  update();
}

void GLWidget::move(const float x, const float y, const float z) {
  moveVector[0] = x;
  moveVector[1] = y;
  moveVector[2] = z;
  update();
}

void GLWidget::rotate(const float x, const float y, const float z) {
  // BUG: possible bottleneck
  // TODO: optimize mat4 multiplication
  float rot_x[16], rot_y[16], rot_z[16];
  rotate_x(x, rot_x);
  rotate_y(y, rot_y);
  rotate_z(z, rot_z);

  mat4_multiply(rot_z, rot_y, rotationMatrix);
  mat4_multiply(rotationMatrix, rot_x, rotationMatrix);
  update();
}

void GLWidget::setLineStyle(bool dashed, float width) {
  lineDashed = dashed;
  lineWidth = width;
  update();
}

void GLWidget::setPointStyle(int style, float size) {
  pointStyle = style;
  pointSize = size;
  update();
}

void GLWidget::resetUniforms() {
  makeCurrent();
  prog.bind();

  float identity[16] = {0};
  identity_matrix(identity);

  glUniform3f(moveLoc, 0.0f, 0.0f, 0.0f);
  glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, identity);
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, identity);
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, identity);

  update();
  prog.release();
  doneCurrent();
}
