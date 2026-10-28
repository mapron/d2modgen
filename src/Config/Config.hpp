#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "StructBaseCpp.hpp"

namespace D2ModGen {

struct Config : public CommonStruct {
    struct Fruit : public CommonStruct {
        struct Leaf : public CommonStruct {
            std::string m_mode{};

            Leaf()
            {
                add("mode", m_mode);
            }
        };
        Leaf m_leaf{};

        int64_t m_weight{ 42 };

        bool m_flag{ true };

        std::string m_name{};

        Fruit()
        {
            add("leaf", m_leaf);
            add("weight", m_weight, 42, 1, 100);
            add("flag", m_flag, true);
            add("name", m_name);
        }
    };
    Fruit m_one{};

    Fruit m_two{};

    int64_t m_bar{ 77 };

    Config()
    {
        add("one", m_one);
        add("two", m_two);
        add("bar", m_bar, 77, 1, 111);
    }
};
}
