#include "widget.h"

#include <QCloseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include "../3rdparty/gif.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  glWidget = new GLWidget(this);
  ui->view->addWidget(glWidget);

  loadSettings();
  loadUi();
  loadRouter();
}

Widget::~Widget() {
  delete glWidget;
  delete ui;
}

void Widget::closeEvent(QCloseEvent *event) {
  QSettings settings("school21", "viewer");

  settings.setValue("background-color", glWidget->getBackgroundColor().name());
  settings.setValue("vertex-color", glWidget->getVertexColor().name());
  settings.setValue("edge-color", glWidget->getEdgeColor().name());

  settings.setValue("vertex-size", ui->slider_vert->sliderPosition());
  settings.setValue("edge-size", ui->slider_edges->sliderPosition());

  settings.setValue("vertex-type", ui->comboBox_vert->currentIndex());
  settings.setValue("edge-type", ui->comboBox_edges->currentIndex());
  settings.setValue("projection-type", ui->projection_box->currentIndex());

  settings.setValue("scale", ui->slider_scale->sliderPosition());

  settings.setValue("move-x", ui->slider_move_x->sliderPosition());
  settings.setValue("move-y", ui->slider_move_y->sliderPosition());
  settings.setValue("move-z", ui->slider_move_z->sliderPosition());

  settings.setValue("rotation-x", ui->slider_rotate_x->sliderPosition());
  settings.setValue("rotation-y", ui->slider_rotate_y->sliderPosition());
  settings.setValue("rotation-z", ui->slider_rotate_z->sliderPosition());

  event->accept();
}

void Widget::loadSettings() {
  QSettings settings("school21", "viewer");

  auto bColor = settings.value("background-color", "#334d4d").value<QColor>();
  auto vColor = settings.value("vertex-color", "#9a9996").value<QColor>();
  auto eColor = settings.value("edge-color", "#c64600").value<QColor>();

  const int vertexSize = settings.value("vertex-size").value<int>();
  const int edgeSize = settings.value("edge-size").value<int>();

  const int vertexType = settings.value("vertex-type").value<int>();
  const int edgeType = settings.value("edge-type").value<int>();
  const int projectionType = settings.value("projection-type").value<int>();

  const int scale = settings.value("scale", 50).value<int>();

  const int moveX = settings.value("move-x", 50).value<int>();
  const int moveY = settings.value("move-y", 50).value<int>();
  const int moveZ = settings.value("move-z", 50).value<int>();

  const int rotationX = settings.value("rotation-x").value<int>();
  const int rotationY = settings.value("rotation-y").value<int>();
  const int rotationZ = settings.value("rotation-z").value<int>();

  ui->slider_vert->setSliderPosition(vertexSize);
  ui->slider_edges->setSliderPosition(edgeSize);

  ui->comboBox_vert->setCurrentIndex(vertexType);
  ui->comboBox_edges->setCurrentIndex(edgeType);
  ui->projection_box->setCurrentIndex(projectionType);

  ui->slider_scale->setSliderPosition(scale);

  ui->slider_move_x->setSliderPosition(moveX);
  ui->slider_move_y->setSliderPosition(moveY);
  ui->slider_move_z->setSliderPosition(moveZ);

  ui->slider_rotate_x->setSliderPosition(rotationX);
  ui->slider_rotate_y->setSliderPosition(rotationY);
  ui->slider_rotate_z->setSliderPosition(rotationZ);

  glWidget->setBackgroundColor(bColor);
  glWidget->setVertexColor(vColor);
  glWidget->setEdgeColor(eColor);

  glWidget->setProjection(projectionType);
  glWidget->setLineStyle(edgeType, edgeSize);
  glWidget->setPointStyle(vertexType, vertexSize);

  glWidget->setScale(SCALE_TICK * scale);
  glWidget->move(MOVE_TICK * (moveX - 50), MOVE_TICK * (moveY - 50),
                 MOVE_TICK * (moveZ - 50));
  glWidget->rotate(ROTATE_TICK * rotationX, ROTATE_TICK * rotationY,
                   ROTATE_TICK * rotationZ);
}

void Widget::loadUi() {
  setWindowTitle("3D Viewer");
  setMinimumSize(800, 600);

  QList<QWidget *> controlElements = QList(std::initializer_list<QWidget *>{
      ui->color_button, ui->color_edges, ui->color_vert, ui->screen_button,
      ui->reset_button, ui->gif_button, ui->open_button, ui->comboBox_edges,
      ui->comboBox_vert, ui->projection_box, ui->slider_edges, ui->slider_vert,
      ui->slider_scale, ui->slider_move_x, ui->slider_move_y, ui->slider_move_z,
      ui->slider_rotate_x, ui->slider_rotate_y, ui->slider_rotate_z});

  for (auto element : controlElements) {
    element->setCursor(Qt::PointingHandCursor);
  }

  auto colorTemplate = QString("background-color: %1;");
  ui->color_button->setStyleSheet(
      colorTemplate.arg(glWidget->getBackgroundColor().name()));

  ui->color_edges->setStyleSheet(
      colorTemplate.arg(glWidget->getEdgeColor().name()));

  ui->color_vert->setStyleSheet(
      colorTemplate.arg(glWidget->getVertexColor().name()));
}

void Widget::loadRouter() {
  connect(ui->color_button, &QPushButton::clicked, this,
          &Widget::onChangeBackgroundColor);
  connect(ui->color_edges, &QPushButton::clicked, this,
          &Widget::onChangeEdgeColor);
  connect(ui->slider_edges, &QSlider::valueChanged, this,
          &Widget::onChangeLineStyle);
  connect(ui->comboBox_edges, &QComboBox::currentIndexChanged, this,
          &Widget::onChangeLineStyle);

  connect(ui->color_vert, &QPushButton::clicked, this,
          &Widget::onChangeVertexColor);
  connect(ui->slider_vert, &QSlider::valueChanged, this,
          &Widget::onChangePointStyle);
  connect(ui->comboBox_vert, &QComboBox::currentIndexChanged, this,
          &Widget::onChangePointStyle);

  connect(ui->file_line, &QLineEdit::returnPressed, this,
          &Widget::onChangeFilePath);
  connect(ui->open_button, &QPushButton::clicked, this,
          &Widget::onFileOpenButtonClick);

  connect(ui->projection_box, &QComboBox::currentIndexChanged, this,
          &Widget::onChangeProjectionType);

  connect(ui->reset_button, &QPushButton::clicked, this,
          &Widget::onResetButtonClick);
  connect(ui->screen_button, &QPushButton::clicked, this,
          &Widget::onScreenshotButtonClick);
  connect(ui->gif_button, &QPushButton::clicked, this,
          &Widget::onGifButtonClick);

  connect(ui->slider_scale, &QSlider::valueChanged, this,
          &Widget::onChangeScaleSlider);

  connect(ui->slider_move_x, &QSlider::valueChanged, this,
          &Widget::onChangeMoveSlider);
  connect(ui->slider_move_y, &QSlider::valueChanged, this,
          &Widget::onChangeMoveSlider);
  connect(ui->slider_move_z, &QSlider::valueChanged, this,
          &Widget::onChangeMoveSlider);

  connect(ui->slider_rotate_x, &QSlider::valueChanged, this,
          &Widget::onChangeRotateSlider);
  connect(ui->slider_rotate_y, &QSlider::valueChanged, this,
          &Widget::onChangeRotateSlider);
  connect(ui->slider_rotate_z, &QSlider::valueChanged, this,
          &Widget::onChangeRotateSlider);

  connect(glWidget, &GLWidget::loaded, this, &Widget::onChangeStats);
}

QColor Widget::chooseColor(QToolButton *button, const QColor &currColor) {
  QColor color = QColorDialog::getColor(currColor, this, "Выберите цвет");
  if (color.isValid()) {
    button->setStyleSheet(QString("background-color: %1;").arg(color.name()));
  }

  return color;
}

bool Widget::isValidObjFile(const QString &path) {
  QFileInfo f = QFileInfo(path);
  QString message = "";

  if (!f.isFile()) {
    message = "Not a file.";
  } else if (!f.isReadable()) {
    message = "Can not read a file.";
  } else if (f.completeSuffix() != "obj") {
    message = "Incorrect file format.";
  }

  if (!message.isEmpty()) {
    QMessageBox::warning(this, "Error", message, QMessageBox::Ok);
    return false;
  }

  return true;
}

bool Widget::isValidImageFile(const QString &path, std::vector<QString> &ext) {
  QFileInfo f = QFileInfo(path);
  QFileInfo d = QFileInfo(f.dir(), path);
  QString message = "";

  if (!d.isWritable()) {
    message = "Permission denied.";
  }
  if (std::find(ext.begin(), ext.end(), f.suffix()) == ext.end()) {
    message = "Incorrect file format.";
  }

  if (message.isEmpty()) {
    QMessageBox::warning(this, "Error", message, QMessageBox::Ok);
    return false;
  }

  return true;
}

void Widget::onChangeEdgeColor() {
  QColor currColor = glWidget->getEdgeColor();
  QColor color = chooseColor(ui->color_edges, currColor);

  if (color.isValid()) {
    glWidget->setEdgeColor(color);
  }
}

void Widget::onChangeVertexColor() {
  QColor currColor = glWidget->getVertexColor();
  QColor color = chooseColor(ui->color_vert, currColor);

  if (color.isValid()) {
    glWidget->setVertexColor(color);
  }
}

void Widget::onChangeBackgroundColor() {
  QColor currColor = glWidget->getEdgeColor();
  QColor color = chooseColor(ui->color_button, currColor);

  if (color.isValid()) {
    glWidget->setBackgroundColor(color);
  }
}

void Widget::onFileOpenButtonClick() {
  auto path = QFileDialog::getOpenFileName(this, "Choose model", ".", "*.obj");

  if (path.isEmpty() || !isValidObjFile(path)) {
    return;
  }

  ui->file_line->setText(path);
  glWidget->loadObjectFromFile(path.trimmed());
}

void Widget::onChangeFilePath() {
  auto prevPath = QString::fromStdString(glWidget->getObjectName());
  QString path = ui->file_line->text().trimmed();

  if (path.isEmpty() || !isValidObjFile(path)) {
    ui->file_line->setText(prevPath);
    return;
  }

  glWidget->loadObjectFromFile(path);
}

void Widget::onChangeStats() {
  auto objName = QString::fromStdString(glWidget->getObjectName());
  std::size_t verticesAmount = glWidget->getVerticesAmount();
  std::size_t facesAmount = glWidget->getFacesAmount();

  ui->file_name->setText(QFileInfo(objName).fileName());
  ui->vertices->setText(QString::number(verticesAmount));
  ui->edges->setText(QString::number(facesAmount));
}

void Widget::onChangePointStyle() {
  glWidget->setPointStyle(ui->comboBox_vert->currentIndex(),
                          ui->slider_vert->sliderPosition());
}

void Widget::onChangeLineStyle() {
  glWidget->setLineStyle(ui->comboBox_edges->currentIndex(),
                         ui->slider_edges->sliderPosition());
}

void Widget::onChangeScaleSlider() {
  const float ratio = SCALE_TICK * ui->slider_scale->sliderPosition();
  glWidget->setScale(ratio);
}

void Widget::onChangeMoveSlider() {
  const float x = MOVE_TICK * (ui->slider_move_x->sliderPosition() - 50);
  const float y = MOVE_TICK * (ui->slider_move_y->sliderPosition() - 50);
  const float z = MOVE_TICK * (ui->slider_move_z->sliderPosition() - 50);

  glWidget->move(x, y, z);
}

void Widget::onChangeRotateSlider() {
  const float x = ROTATE_TICK * ui->slider_rotate_x->sliderPosition();
  const float y = ROTATE_TICK * ui->slider_rotate_y->sliderPosition();
  const float z = ROTATE_TICK * ui->slider_rotate_z->sliderPosition();

  glWidget->rotate(x, y, z);
}

void Widget::onChangeProjectionType() {
  const int type = ui->projection_box->currentIndex();
  glWidget->setProjection(type);
}

void Widget::onScreenshotButtonClick() {
  auto path = QFileDialog::getSaveFileName(
      this, "Choose path to save the screenshot", ".", "*.jpeg, *.bmp");
  std::vector<QString> ext = {"bmp", "jpeg"};
  if (path.isEmpty() || !isValidImageFile(path, ext)) {
    return;
  }

  QImage screen = glWidget->grabFramebuffer();
  screen.save(path);
}

void Widget::onGifButtonClick() {
  const int fps = 10, durationSec = 5;
  const int totalFrames = fps * durationSec;
  const QSize gifSize(640, 480);
  const QString path =
      QString::fromStdString(glWidget->getObjectName()) + "_" +
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".gif";

  GifWriter *gif = new GifWriter;
  if (!GifBegin(gif, path.toLatin1(), gifSize.width(), gifSize.height(), fps)) {
    QMessageBox::warning(this, "Error", "Something went wrong.");
    delete gif;
    return;
  }

  QTimer *timer = new QTimer(this);
  int frameCount = 0;

  connect(timer, &QTimer::timeout, this, [=]() mutable {
    QImage frame = glWidget->grabFramebuffer()
                       .convertToFormat(QImage::Format_RGBA8888)
                       .scaled(gifSize);
    GifWriteFrame(gif, frame.bits(), frame.width(), frame.height(), fps);

    int secondsLeft = durationSec - (++frameCount / fps);
    ui->gif_button->setText(QString::number(secondsLeft) + "s");

    if (frameCount >= totalFrames) {
      timer->stop();
      GifEnd(gif);
      delete timer;
      delete gif;
      ui->gif_button->setText("");
      ui->gif_button->setEnabled(true);
      QMessageBox::information(this, "Gif ready",
                               "Gif successfully saved in " + path);
    }
  });

  ui->gif_button->setDisabled(true);
  ui->gif_button->setText(QString::number(durationSec) + "s");
  timer->start(1000 / fps);
}

void Widget::onResetButtonClick() {
  glWidget->resetUniforms();
  ui->slider_scale->setSliderPosition(50);
  ui->slider_move_x->setSliderPosition(50);
  ui->slider_move_y->setSliderPosition(50);
  ui->slider_move_z->setSliderPosition(50);
  ui->slider_rotate_x->setSliderPosition(0);
  ui->slider_rotate_y->setSliderPosition(0);
  ui->slider_rotate_z->setSliderPosition(0);
  ui->projection_box->setCurrentIndex(0);
}
