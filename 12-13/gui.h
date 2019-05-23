#pragma once

#include "controller.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>

class GUI : public QWidget {
    Q_OBJECT

    private:
    Controller&    ctrl;
    vector<string> movies_database_list;

    QTabWidget* tabs;

    public:
    explicit GUI(Controller& _ctrl, QWidget* parent = nullptr);
    ~GUI() = default;
};

class Admin : public QWidget {
    Q_OBJECT

    private:
    Controller& ctrl;

    QListWidget* database_list;

    QPushButton* add_button;
    QPushButton* remove_button;
    QPushButton* update_button;

    QLineEdit* title_line;
    QLineEdit* genre_line;
    QLineEdit* year_line;
    QLineEdit* likes_line;
    QLineEdit* trailer_line;

    void refresh_list();

    public:
    explicit Admin(Controller& _ctrl, QWidget* parent = nullptr);
    ~Admin() = default;
};

class User : public QWidget {
    Q_OBJECT

    private:
    Controller& ctrl;

    QListWidget* watchlist_list;
    QListWidget* genres_list;

    QPushButton* choose_csv_button;
    QPushButton* choose_html_button;
    QPushButton* add_button;
    QPushButton* remove_button;
    QPushButton* next_button;
    QPushButton* open_file_button;
    QCheckBox*   liked_checkbox;
    QLabel*      current_movie;

    public:
    explicit User(Controller& _ctrl, QWidget* parent = nullptr);
    ~User() = default;
};
