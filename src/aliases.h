#pragma once

template<class T>
using v_expec = std::expected<void, T>;

using v_expec_str = std::expected<void, std::string>;