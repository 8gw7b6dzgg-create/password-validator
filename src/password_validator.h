#pragma once
#include <QVariant>
#include <QStringList>
#include <stdexcept>
namespace password_validator {
inline constexpr int MIN_PASSWORD_LENGTH = 8;
inline constexpr int MAX_PASSWORD_LENGTH = 64;
// QString and integral parameters are checked at runtime to match the specification.
bool check_length(const QVariant& password, const QVariant& min_len, const QVariant& max_len);
bool has_digit(const QVariant& password);
bool has_upper_and_lower(const QVariant& password);
bool contains_space(const QVariant& password);
QStringList validate_password(const QVariant& password);
}
