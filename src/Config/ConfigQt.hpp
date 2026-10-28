#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "Config.hpp"
#include "StructBaseQt.hpp"

namespace D2ModGen {

class Config_Fruit_Leaf_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(QString mode READ get_mode WRITE set_mode NOTIFY dataChanged)

public:
    explicit Config_Fruit_Leaf_Qt(Config::Fruit::Leaf& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    QString get_mode() const { return get(m_data.m_mode); }

    void set_mode(const QString& val) { return set(m_data.m_mode, val); }

private:
    Config::Fruit::Leaf& m_data;
};

class Config_Fruit_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_Fruit_Leaf_Qt* leaf READ get_leaf CONSTANT)
    Q_PROPERTY(qlonglong weight READ get_weight WRITE set_weight NOTIFY dataChanged)
    Q_PROPERTY(int weight_min READ get_weight_min CONSTANT)
    Q_PROPERTY(int weight_max READ get_weight_max CONSTANT)
    Q_PROPERTY(bool flag READ get_flag WRITE set_flag NOTIFY dataChanged)
    Q_PROPERTY(QString name READ get_name WRITE set_name NOTIFY dataChanged)

public:
    explicit Config_Fruit_Qt(Config::Fruit& data)
        : BaseStruct_Qt(data)
        , m_data(data)
        , m_leaf(data.m_leaf)
    {
    }

    Config_Fruit_Leaf_Qt* get_leaf() { return &m_leaf; }

    qlonglong get_weight() const { return get(m_data.m_weight); }

    void set_weight(qlonglong val) { return set(m_data.m_weight, val); }

    qlonglong get_weight_min() const { return 1; }

    qlonglong get_weight_max() const { return 100; }

    bool get_flag() const { return get(m_data.m_flag); }

    void set_flag(bool val) { return set(m_data.m_flag, val); }

    QString get_name() const { return get(m_data.m_name); }

    void set_name(const QString& val) { return set(m_data.m_name, val); }

private:
    Config::Fruit& m_data;

    Config_Fruit_Leaf_Qt m_leaf;
};

class Config_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_Fruit_Qt* one READ get_one CONSTANT)
    Q_PROPERTY(Config_Fruit_Qt* two READ get_two CONSTANT)
    Q_PROPERTY(qlonglong bar READ get_bar WRITE set_bar NOTIFY dataChanged)
    Q_PROPERTY(int bar_min READ get_bar_min CONSTANT)
    Q_PROPERTY(int bar_max READ get_bar_max CONSTANT)

public:
    explicit Config_Qt(Config& data)
        : BaseStruct_Qt(data)
        , m_data(data)
        , m_one(data.m_one)
        , m_two(data.m_two)
    {
    }

    Config_Fruit_Qt* get_one() { return &m_one; }

    Config_Fruit_Qt* get_two() { return &m_two; }

    qlonglong get_bar() const { return get(m_data.m_bar); }

    void set_bar(qlonglong val) { return set(m_data.m_bar, val); }

    qlonglong get_bar_min() const { return 1; }

    qlonglong get_bar_max() const { return 111; }

private:
    Config& m_data;

    Config_Fruit_Qt m_one;

    Config_Fruit_Qt m_two;
};

}
