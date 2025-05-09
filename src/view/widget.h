#ifndef WIDGET_H
#define WIDGET_H
#include <QToolButton>
#include <QWidget>

#include "glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

  const float SCALE_TICK = 0.02f;
  const float MOVE_TICK = 0.0555f;
  const float ROTATE_TICK = 0.06283f;

 protected:
  void closeEvent(QCloseEvent *event) override;

 private slots:
  void onChangeFilePath();
  void onFileOpenButtonClick();
  void onChangeStats();

  void onChangePointStyle();
  void onChangeLineStyle();

  void onChangeScaleSlider();
  void onChangeMoveSlider();
  void onChangeRotateSlider();
  void onChangeProjectionType();

  void onScreenshotButtonClick();
  void onGifButtonClick();
  void onResetButtonClick();

  void onChangeEdgeColor();
  void onChangeVertexColor();
  void onChangeBackgroundColor();

 private:
  void loadSettings();
  void loadUi();
  void loadRouter();
  bool isValidObjFile(const QString &path);
  bool isValidImageFile(const QString &path, std::vector<QString> &ext);
  QColor chooseColor(QToolButton *button, const QColor &currColor);

  Ui::Widget *ui;
  GLWidget *glWidget;
};
#endif  // WIDGET_H
