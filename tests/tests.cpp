#include "password_validator.h"
#include <QTextStream>
#include <functional>
using namespace password_validator;
int main() {
    QTextStream out(stdout); int count = 0, failed = 0;
    auto test = [&](const QString& name, std::function<bool()> body) {
        bool ok = false; try { ok = body(); } catch (...) {}
        out << ++count << ". " << name << ": " << (ok ? "PASS" : "FAIL") << Qt::endl;
        if (!ok) ++failed;
    };
    auto invalid = [](std::function<void()> body) { try { body(); } catch (const std::invalid_argument& e) { return QString(e.what()) == "ERR_INVALID_ARGUMENT"; } return false; };
    test("Abcdefg1", []{return validate_password(QString("Abcdefg1")).isEmpty();});
    test("Secret2024", []{return validate_password(QString("Secret2024")).isEmpty();});
    test("Digit", []{return has_digit(QString("Pass9"));});
    test("Length 8", []{return check_length(QString("abcdefgh"),8,64);});
    test("Length 7", []{return !check_length(QString("abcdefg"),8,64);});
    test("Length 64", []{return validate_password("Aa1"+QString(61,'b')).isEmpty();});
    test("Non-string", [&]{return invalid([]{validate_password(12345678);});});
    test("Invalid range", [&]{return invalid([]{check_length(QString("abc"),10,5);});});
    test("Zero minimum", [&]{return invalid([]{check_length(QString("abc"),0,5);});});
    test("Empty", []{return validate_password(QString("")) == QStringList{"TOO_SHORT","NO_DIGIT","NO_MIXED_CASE"};});
    test("Error order", []{return validate_password(QString("abc def")) == QStringList{"TOO_SHORT","NO_DIGIT","NO_MIXED_CASE","HAS_SPACE"};});
    test("Cyrillic", []{return !has_upper_and_lower(QString("ПАРОЛЬ1a"));});
    test("Length 65", []{return validate_password("Aa1"+QString(62,'b')) == QStringList{"TOO_LONG"};});
    test("Tab", []{return contains_space(QString("Ab\t12345"));});
    test("Unicode code points", []{return check_length(QString::fromUtf8("😀😀😀😀😀😀😀😀"),8,8);});
    test("All functions reject type", [&]{return invalid([]{has_digit(1);}) && invalid([]{has_upper_and_lower(1);}) && invalid([]{contains_space(1);}) && invalid([]{check_length(1,8,64);});});
    test("Parameter types", [&]{return invalid([]{check_length(QString("abc"),8.0,64);});});
    out << "Total: " << count << ", passed: " << count-failed << ", failed: " << failed << Qt::endl;
    return failed ? 1 : 0;
}
