#pragma once
#include <functional>

#include "store.h"

using ChangeFunction = std::function<Value(Value)>;
