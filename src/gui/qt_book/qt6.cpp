#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Enter Your Age");

    QSpinBox *spinBox = new QSpinBox;
    QSlider *slider = new QSlider(Qt::Horizontal);
    spinBox->setRange(0, 130);
    slider->setRange(0, 130);

    QObject::connect(spinBox, SIGNAL(valueChanged(int)),
                     slider, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)),
                     spinBox, SLOT(setValue(int)));
    spinBox->setValue(35);

    QPushButton *button = new QPushButton("Quit");
    QObject::connect(button, 
                     SIGNAL(clicked()),
                     &app, 
                     SLOT(quit()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(spinBox, 0, 0);
    layout->addWidget(slider, 0, 1);
    layout->addWidget(button, 1, 0, 1, 2);

    window->setLayout(layout);

    window->show();

    return app.exec();
}


