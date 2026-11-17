#include "password_validator.h"
#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
using namespace password_validator;
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Валидатор пароля");
    QWidget window; window.setWindowTitle("Валидатор пароля"); window.resize(940,600);
    window.setStyleSheet(R"(
        QWidget { background: #F4F5F4; color: #202923; font-family: 'Arial'; font-size: 15px; }
        QFrame#side { background: #E3EBE5; border-radius: 16px; }
        QFrame#side QLabel { background: transparent; }
        QLabel#title { font-size: 32px; font-weight: 700; }
        QLabel#brand { font-size: 26px; font-weight: 700; }
        QLabel#muted { color: #526058; }
        QLineEdit { background: white; border: 1px solid #8C9D90; border-radius: 9px; padding: 14px; font-size: 21px; selection-background-color: #D4E5D8; selection-color: #202923; }
        QLineEdit:focus { border: 2px solid #456A50; padding: 13px; }
        QPushButton { background: #315A3E; color: white; border: none; border-radius: 8px; padding: 13px 20px; font-weight: 700; }
        QPushButton:hover { background: #24462E; }
        QPushButton#clear { background: #E2E7E3; color: #202923; }
        QCheckBox { spacing: 8px; }
        QLabel#status { font-size: 21px; font-weight: 700; }
    )");
    auto root = new QHBoxLayout(&window); root->setContentsMargins(24,24,24,24); root->setSpacing(30);
    auto side = new QFrame; side->setObjectName("side"); side->setFixedWidth(245);
    auto sl = new QVBoxLayout(side); sl->setContentsMargins(26,30,26,30);
    auto brand = new QLabel("Валидатор\nпароля"); brand->setObjectName("brand"); sl->addWidget(brand);
    auto sub = new QLabel("Регистрация клиента"); sub->setObjectName("muted"); sl->addWidget(sub); sl->addSpacing(30);
    auto info = new QLabel("Проверьте пароль\nперед регистрацией.\n\nОт 8 до 64 символов\nЦифра от 0 до 9\nЛатинские A–Z и a–z\nБез пробельных символов"); info->setWordWrap(true); sl->addWidget(info);
    sl->addStretch(); auto privacy = new QLabel("Проверка на устройстве.\nПароли не сохраняются."); privacy->setObjectName("muted"); sl->addWidget(privacy); root->addWidget(side);
    auto main = new QVBoxLayout; main->setSpacing(14); main->setContentsMargins(0,18,8,10); root->addLayout(main,1);
    auto title = new QLabel("Проверка пароля"); title->setObjectName("title"); main->addWidget(title);
    auto hint = new QLabel("Введите пароль и проверьте все правила."); hint->setObjectName("muted"); main->addWidget(hint); main->addSpacing(8);
    auto field = new QLineEdit; field->setPlaceholderText("Введите пароль"); field->setEchoMode(QLineEdit::Password); field->setAccessibleName("Пароль"); main->addWidget(field);
    auto row = new QHBoxLayout; auto show = new QCheckBox("Показать пароль"); row->addWidget(show); row->addStretch(); auto count = new QLabel("0 / 64"); row->addWidget(count); main->addLayout(row);
    auto actions = new QHBoxLayout; auto check = new QPushButton("Проверить пароль"); auto clear = new QPushButton("Очистить"); clear->setObjectName("clear"); actions->addWidget(check); actions->addWidget(clear); actions->addStretch(); main->addLayout(actions); main->addSpacing(8);
    auto status = new QLabel("Введите пароль"); status->setObjectName("status"); main->addWidget(status);
    QList<QLabel*> rules; for(int i=0;i<4;++i){auto l=new QLabel; rules<<l; main->addWidget(l);}
    auto codes = new QLabel; codes->setWordWrap(true); codes->setObjectName("muted"); main->addWidget(codes); main->addStretch();
    auto update = [=]{
        const QString p=field->text(); auto errors=validate_password(p);
        count->setText(QString::number(p.toUcs4().size())+" / 64");
        status->setText(errors.isEmpty()?"Пароль принят": "Нужно исправить пароль");
        status->setStyleSheet(errors.isEmpty()?"color: #315A3E":"color: #873E2E");
        const bool ok[]={check_length(p,8,64),has_digit(p),has_upper_and_lower(p),!contains_space(p)};
        const QString labels[]={"Длина от 8 до 64 символов","Есть цифра от 0 до 9","Есть заглавная и строчная латинские буквы","Нет пробелов и табуляции"};
        for(int i=0;i<4;++i){rules[i]->setText((ok[i]?"✓  ":"○  ")+labels[i]);rules[i]->setStyleSheet(ok[i]?"color: #315A3E":"color: #873E2E");}
        codes->setText(errors.isEmpty()?"Все правила выполнены.":"Нарушено: "+errors.join(", "));
    };
    QObject::connect(field,&QLineEdit::textChanged,&window,[=]{update();});
    QObject::connect(check,&QPushButton::clicked,&window,update);
    QObject::connect(field,&QLineEdit::returnPressed,&window,update);
    QObject::connect(clear,&QPushButton::clicked,field,[=]{field->clear();field->setFocus();});
    QObject::connect(show,&QCheckBox::toggled,field,[=](bool on){field->setEchoMode(on?QLineEdit::Normal:QLineEdit::Password);});
    update(); window.show(); field->setFocus();
    // Reproducible captures of the actual Qt application, using public example passwords.
    if(app.arguments().contains("--screenshots")) {
        const QString dir=app.arguments().value(app.arguments().indexOf("--screenshots")+1,"screenshots"); QDir().mkpath(dir);
        QTimer::singleShot(300,&window,[=,&window]{show->setChecked(true);field->setText("abc def");field->clearFocus();window.grab().save(dir+"/invalid.png");});
        QTimer::singleShot(700,&window,[=,&window]{field->setText("Sport2024");window.grab().save(dir+"/valid.png");});
        QTimer::singleShot(1000,&app,&QApplication::quit);
    }
    return app.exec();
}
