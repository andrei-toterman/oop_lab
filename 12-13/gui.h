#pragma once

#include "controller.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>

class Admin : public QWidget {
    Q_OBJECT

    private:
    Movie current_movie;

    QLabel* errors;

    QListWidget* database_list;

    QPushButton* add_button;
    QPushButton* remove_button;
    QPushButton* update_button;

    QLineEdit* title_line;
    QLineEdit* genre_line;
    QLineEdit* year_line;
    QLineEdit* likes_line;
    QLineEdit* trailer_line;

    public:
    explicit Admin(QWidget* parent = nullptr);
    ~Admin() = default;

    void populate_list(vector<QString> movies);
    //    void set_current_movie(const Movie& m);
    void set_errors(const string& errors);

    signals:
    Movie get_movie(int index);
    void  database_add(const string& title, const string& genre, int year, int likes, const string& trailer);
    void  database_remove(const string& id);
    void  database_update(const string& id, const string& title, const string& genre, int year, int likes,
                          const string& trailer);
};

class User : public QWidget {
    Q_OBJECT

    private:
    vector<Movie>::iterator current_movie;
    vector<Movie>           genre_movies;
    string                  selected_id;

    QListWidget* watchlist_list;
    QComboBox*   genres_dropdown;

    QLabel* current_movie_label;

    QPushButton* add_button;
    QPushButton* remove_button;
    QPushButton* next_button;
    QPushButton* open_file_button;
    QCheckBox*   liked_checkbox;

    public:
    explicit User(QWidget* parent = nullptr);
    ~User() = default;

    void populate_dropdown(vector<QString> genres);
    void populate_list(vector<QString> movies);

    signals:
    Movie         get_movie(int index);
    vector<Movie> get_movies(const QString& genre);
    void          watchlist_add(const Movie& movie);
    void          watchlist_open();
    void          watchlist_remove(const string& id, bool liked);
};

class GUI : public QWidget {
    Q_OBJECT

    private:
    Controller& ctrl;

    QPushButton* choose_csv_button;
    QPushButton* choose_html_button;

    QTabWidget* tabs;

    Admin* admin;
    User*  user;

    public:
    explicit GUI(Controller& _ctrl, QWidget* parent = nullptr);
    ~GUI() = default;

    vector<QString> get_db_movies_string();
    vector<QString> get_wl_movies_string();
    vector<QString> get_genres_string();
};
