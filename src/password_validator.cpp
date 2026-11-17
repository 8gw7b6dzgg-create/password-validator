#include "password_validator.h"
namespace password_validator {
namespace {
QString string_arg(const QVariant& value) {
    if (value.metaType().id() != QMetaType::QString)
        throw std::invalid_argument("ERR_INVALID_ARGUMENT");
    return value.toString();
}
int int_arg(const QVariant& value) {
    if (value.metaType().id() != QMetaType::Int)
        throw std::invalid_argument("ERR_INVALID_ARGUMENT");
    return value.toInt();
}
qsizetype length(const QString& text) { return text.toUcs4().size(); }
}
bool check_length(const QVariant& password, const QVariant& min_len, const QVariant& max_len) {
    const auto text = string_arg(password);
    const int min = int_arg(min_len), max = int_arg(max_len);
    if (min < 1 || max < min) throw std::invalid_argument("ERR_INVALID_ARGUMENT");
    const auto size = length(text);
    return size >= min && size <= max;
}
bool has_digit(const QVariant& password) {
    const auto text = string_arg(password);
    for (const auto ch : text) if (ch >= u'0' && ch <= u'9') return true;
    return false;
}
bool has_upper_and_lower(const QVariant& password) {
    const auto text = string_arg(password);
    bool upper = false, lower = false;
    for (const auto ch : text) {
        upper |= ch >= u'A' && ch <= u'Z';
        lower |= ch >= u'a' && ch <= u'z';
    }
    return upper && lower;
}
bool contains_space(const QVariant& password) {
    const auto text = string_arg(password);
    for (const auto ch : text) if (ch.isSpace()) return true;
    return false;
}
QStringList validate_password(const QVariant& password) {
    const auto text = string_arg(password);
    QStringList errors;
    if (!check_length(password, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH))
        errors << (length(text) < MIN_PASSWORD_LENGTH ? "TOO_SHORT" : "TOO_LONG");
    if (!has_digit(password)) errors << "NO_DIGIT";
    if (!has_upper_and_lower(password)) errors << "NO_MIXED_CASE";
    if (contains_space(password)) errors << "HAS_SPACE";
    return errors;
}
}
