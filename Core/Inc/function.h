#pragma once

#include <functional>


template<class...Args>
using Callback = std::function<void(Args...)>;
