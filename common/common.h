#pragma once
#include <NTL/ZZ.h>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

NTL_CLIENT

ZZ generate_random_number(const ZZ &a, const ZZ &b);
string serialize_ZZ(const ZZ& num);
ZZ unserialize_ZZ(string str);