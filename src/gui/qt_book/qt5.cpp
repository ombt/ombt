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

#if 1
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(spinBox);
    hlayout->addWidget(slider);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addWidget(button);

    window->setLayout(vlayout);
#else
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(spinBox);
    vlayout->addWidget(slider);
    vlayout->addWidget(button);

    window->setLayout(vlayout);
#endif

    window->show();

    return app.exec();
}


