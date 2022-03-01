/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

namespace D2ModGen {

namespace details {

template<typename F>
struct ScopeExitFunctionWrapper {
    ScopeExitFunctionWrapper(const F& f)
        : f(f)
    {}

    ScopeExitFunctionWrapper(const ScopeExitFunctionWrapper&) = delete;
    ScopeExitFunctionWrapper(ScopeExitFunctionWrapper&&)      = default;

    ScopeExitFunctionWrapper& operator=(const ScopeExitFunctionWrapper&) = delete;
    ScopeExitFunctionWrapper& operator=(ScopeExitFunctionWrapper&&) = default;

    ~ScopeExitFunctionWrapper() { f(); }
    F f;
};

template<typename F>
static constexpr ScopeExitFunctionWrapper<F> createScopeExitFunctionWrapper(const F& f)
{
    return ScopeExitFunctionWrapper<F>(f);
}

}
#define MODGEN_DO_STRING_JOIN2(arg1, arg2) arg1##arg2
#define MODGEN_STRING_JOIN2(arg1, arg2) MODGEN_DO_STRING_JOIN2(arg1, arg2)

#define MODGEN_SCOPE_EXIT(...) \
    auto MODGEN_STRING_JOIN2(scope_exit_, __LINE__) = D2ModGen::details::createScopeExitFunctionWrapper(__VA_ARGS__); \
    (void) MODGEN_STRING_JOIN2(scope_exit_, __LINE__)

}
