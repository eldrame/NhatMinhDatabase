#include <string>
#include<vector>
#include<map>


class ColumnDef {
public:
    ColumnDef(const std::string& name, const std::string& type, int width)
        : name_(name), type_(type), width_(width) {}

    const std::string& getName() const {
        return name_;
    }

    const std::string& getType() const {
        return type_;
    }

    int getwidth() const {
        return width_;
    }

private:
    std::string name_;
    std::string type_;
    int width_;
};

class ColumnDefs {
public:
    std::vector<ColumnDef> columns_;

    void addColumn(const ColumnDef& column) {
        columns_.push_back(column);
    }

    const st::vector<ColumnDef> getColumn() const {
        return columns_;
    }
};

class Table {
public:
    Table(const std::string& name, const std::vector<ColumnDef>& columns)
        : name_(name), columns_(columns) {}

    const std::string& getName() const {
        return name_;
    }

    const std::vector<ColumnDef>& getColumns() const {
        return columns_;
    }

    const std::vector<ColumnDef> getColumnDefs() const {
        ColumnDefs columndefs;
        columndefs.columns_ = columns_;
        columndefs.getColumn();
    }

private:
    std::string name_;
    std::vector<ColumnDef> columns_;
};