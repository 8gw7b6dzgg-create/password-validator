#include "password_validator.h"
#include <QTextStream>
int main() {
    QTextStream out(stdout);
    for (const auto& password : QStringList{"sport2024", "Sport2024", "abc", "Ab 12345"}) {
        const auto errors = password_validator::validate_password(password);
        out << password << " → " << (errors.isEmpty() ? QString("Пароль принят") : "Нарушено: " + errors.join(", ")) << Qt::endl;
    }
    out << "Ab1 → " << (password_validator::check_length(QString("Ab1"), 8, 64) ? "Длина подходит" : "Длина не подходит") << Qt::endl;
}
