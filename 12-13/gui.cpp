#include "gui.h"
#include "csvwatchlist.h"
#include "htmlwatchlist.h"

#include <QDebug>
#include <QFormLayout>

vector<QString> GUI::get_db_movies_string() {
    vector<QString> movies;
    for (Movie& m : this->ctrl.get_database().get_movies())
        movies.push_back(QString::fromStdString(m.to_string()));
    return movies;
}

vector<QString> GUI::get_wl_movies_string() {
    vector<QString> movies;
    for (Movie& m : this->ctrl.get_watchlist()->get_movies())
        movies.push_back(QString::fromStdString(m.to_string()));
    return movies;
}

vector<QString> GUI::get_genres_string() {
    vector<QString> genres;
    for (string& g : this->ctrl.get_database().get_genres())
        genres.push_back(QString::fromStdString(g));
    return genres;
}

GUI::GUI(Controller& _ctrl, QWidget* parent) :
    QWidget(parent),
    ctrl{ _ctrl },
    choose_csv_button{ new QPushButton("CSV") },
    choose_html_button{ new QPushButton{ "HTML" } },
    tabs{ new QTabWidget() },
    admin{ new Admin() },
    user{ new User() } {

    QVBoxLayout* layout               = new QVBoxLayout(this);
    QHBoxLayout* choose_format_layout = new QHBoxLayout();

    choose_format_layout->addWidget(this->choose_csv_button);
    choose_format_layout->addWidget(this->choose_html_button);

    this->tabs->addTab(this->admin, "Admin");
    this->tabs->addTab(this->user, "User");

    layout->addWidget(this->tabs);
    layout->addLayout(choose_format_layout);

    setWindowTitle("LMDB");
    this->tabs->setEnabled(false);
    this->admin->populate_list(this->get_db_movies_string());
    this->user->populate_dropdown(this->get_genres_string());

    connect(this->admin, &Admin::get_movie, [this](int index) {
        return index == -1 ? Movie{}
                           : this->ctrl.get_database().get_movies()[static_cast<unsigned long long>(index)];
    });

    connect(this->admin, &Admin::database_add,
            [this](const string& title, const string& genre, int year, int likes, const string& trailer) {
                try {
                    this->ctrl.database_add(title, genre, year, likes, trailer);
                    this->admin->set_errors("");
                } catch (MovieException& err) {
                    this->admin->set_errors(err.what());
                } catch (RepoException& err) { this->admin->set_errors(err.what()); }
                this->admin->populate_list(this->get_db_movies_string());
                this->user->populate_dropdown(this->get_genres_string());
            });

    connect(this->admin, &Admin::database_remove, [this](const string& id) {
        try {
            this->ctrl.database_remove(id);
            this->admin->set_errors("");
        } catch (RepoException& err) { this->admin->set_errors(err.what()); }
        this->admin->populate_list(this->get_db_movies_string());
        this->user->populate_list(this->get_wl_movies_string());
        this->user->populate_dropdown(this->get_genres_string());
    });

    connect(this->admin, &Admin::database_update,
            [this](const string& id, const string& title, const string& genre, int year, int likes,
                   const string& trailer) {
                try {
                    this->ctrl.database_update(id, title, genre, year, likes, trailer);
                    this->admin->set_errors("");
                } catch (MovieException& err) {
                    this->admin->set_errors(err.what());
                } catch (RepoException& err) { this->admin->set_errors(err.what()); }
                this->admin->populate_list(this->get_db_movies_string());
                this->user->populate_list(this->get_wl_movies_string());
                this->user->populate_dropdown(this->get_genres_string());
            });

    connect(this->user, &User::get_movie, [this](int index) {
        return index == -1 ? Movie{}
                           : this->ctrl.get_watchlist()->get_movies()[static_cast<unsigned long long>(index)];
    });

    connect(this->user, &User::get_movies, [this](const QString& genre) {
        return this->ctrl.get_database()
            .filter_by([genre](const Movie& movie) { return movie.get_genre() == genre.toStdString(); })
            .get_movies();
    });

    connect(this->user, &User::watchlist_add, [this](const Movie& m) {
        try {
            this->ctrl.watchlist_add(m);
        } catch (...) {}
        this->user->populate_list(this->get_wl_movies_string());
    });

    connect(this->user, &User::watchlist_remove, [this](const string& id, bool liked) {
        try {
            this->ctrl.watchlist_remove(id, liked);
        } catch (...) {}
        this->admin->populate_list(this->get_db_movies_string());
        this->user->populate_list(this->get_wl_movies_string());
    });

    connect(this->user, &User::watchlist_open, [this]() { this->ctrl.get_watchlist()->open(); });

    connect(this->choose_csv_button, &QPushButton::clicked, [this]() {
        this->ctrl.set_watchlist(new CsvWatchlist("watchlist.html"));
        this->tabs->setEnabled(true);
        this->choose_csv_button->setEnabled(false);
        this->choose_html_button->setEnabled(false);
    });

    connect(this->choose_html_button, &QPushButton::clicked, [this]() {
        this->ctrl.set_watchlist(new HtmlWatchlist("watchlist.html"));
        this->tabs->setEnabled(true);
        this->choose_csv_button->setEnabled(false);
        this->choose_html_button->setEnabled(false);
    });
}

void Admin::populate_list(vector<QString> movies) {
    this->database_list->clear();
    for (QString& m : movies)
        this->database_list->addItem(m);
    this->current_movie = Movie{};
}

void Admin::set_errors(const std::string& errors) {
    this->errors->setText(QString::fromStdString(errors));
}

Admin::Admin(QWidget* parent) :
    QWidget(parent),
    errors{ new QLabel() },
    database_list{ new QListWidget() },
    add_button{ new QPushButton("Add") },
    remove_button{ new QPushButton("Remove") },
    update_button{ new QPushButton("Update") },
    title_line{ new QLineEdit() },
    genre_line{ new QLineEdit() },
    year_line{ new QLineEdit("0") },
    likes_line{ new QLineEdit("0") },
    trailer_line{ new QLineEdit() } {

    this->database_list->setSelectionMode(QAbstractItemView::SingleSelection);
    this->year_line->setValidator(new QIntValidator(0, 9999, this));
    this->likes_line->setValidator(new QIntValidator(0, 9999, this));

    QHBoxLayout* layout     = new QHBoxLayout(this);
    QVBoxLayout* left_side  = new QVBoxLayout();
    QVBoxLayout* right_side = new QVBoxLayout();

    left_side->addWidget(new QLabel("Databse"));
    left_side->addWidget(this->database_list);

    QFormLayout* form = new QFormLayout();
    form->addRow("Title", this->title_line);
    form->addRow("Genre", this->genre_line);
    form->addRow("Year", this->year_line);
    form->addRow("Likes", this->likes_line);
    form->addRow("Trailer", this->trailer_line);
    right_side->addLayout(form);

    right_side->addWidget(this->errors);
    right_side->addStretch();

    QHBoxLayout* buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(this->add_button);
    buttons_layout->addWidget(this->remove_button);
    buttons_layout->addWidget(this->update_button);
    right_side->addLayout(buttons_layout);

    layout->addLayout(left_side);
    layout->addLayout(right_side);

    connect(this->database_list, &QListWidget::currentRowChanged, [this](int index) {
        this->current_movie = emit this->get_movie(index);
        this->title_line->setText(QString::fromStdString(this->current_movie.get_title()));
        this->genre_line->setText(QString::fromStdString(this->current_movie.get_genre()));
        this->year_line->setText(QString::number(this->current_movie.get_year()));
        this->likes_line->setText(QString::number(this->current_movie.get_likes()));
        this->trailer_line->setText(QString::fromStdString(this->current_movie.get_trailer()));
    });

    connect(this->add_button, &QPushButton::clicked, [this]() {
        string title   = this->title_line->text().toStdString();
        string genre   = this->genre_line->text().toStdString();
        int    year    = this->year_line->text().toInt();
        int    likes   = this->likes_line->text().toInt();
        string trailer = this->trailer_line->text().toStdString();
        emit this->database_add(title, genre, year, likes, trailer);
    });

    connect(this->remove_button, &QPushButton::clicked,
            [this]() { emit this->database_remove(this->current_movie.get_id()); });

    connect(this->update_button, &QPushButton::clicked, [this]() {
        string title   = this->title_line->text().toStdString();
        string genre   = this->genre_line->text().toStdString();
        int    year    = this->year_line->text().toInt();
        int    likes   = this->likes_line->text().toInt();
        string trailer = this->trailer_line->text().toStdString();
        emit this->database_update(this->current_movie.get_id(), title, genre, year, likes, trailer);
    });
}

void User::populate_dropdown(vector<QString> genres) {
    this->genres_dropdown->clear();
    for (QString& g : genres)
        this->genres_dropdown->addItem(g);
}

void User::populate_list(vector<QString> movies) {
    this->watchlist_list->clear();
    for (QString& m : movies)
        this->watchlist_list->addItem(m);
}

User::User(QWidget* parent) :
    QWidget(parent),
    watchlist_list{ new QListWidget() },
    genres_dropdown{ new QComboBox() },
    current_movie_label{ new QLabel("Current movie:\n") },
    add_button{ new QPushButton("Add") },
    remove_button{ new QPushButton("Remove") },
    next_button{ new QPushButton("Next / Play") },
    open_file_button{ new QPushButton("Open file") },
    liked_checkbox{ new QCheckBox("Liked") } {

    this->watchlist_list->setSelectionMode(QAbstractItemView::SingleSelection);

    QHBoxLayout* layout                   = new QHBoxLayout(this);
    QVBoxLayout* left_side                = new QVBoxLayout();
    QVBoxLayout* right_side               = new QVBoxLayout();
    QHBoxLayout* watchlist_buttons_layout = new QHBoxLayout();
    QHBoxLayout* genres_layout            = new QHBoxLayout();
    QHBoxLayout* play_layout              = new QHBoxLayout();

    watchlist_buttons_layout->addWidget(this->open_file_button);
    watchlist_buttons_layout->addWidget(this->remove_button);
    watchlist_buttons_layout->addWidget(this->liked_checkbox);

    left_side->addWidget(new QLabel("Watchlist"));
    left_side->addWidget(this->watchlist_list);
    left_side->addLayout(watchlist_buttons_layout);

    genres_layout->addWidget(new QLabel("Genres"));
    genres_layout->addWidget(this->genres_dropdown);

    play_layout->addWidget(this->next_button);
    play_layout->addWidget(this->add_button);

    right_side->addLayout(genres_layout);
    right_side->addWidget(this->current_movie_label);
    right_side->addStretch();
    right_side->addLayout(play_layout);

    layout->addLayout(left_side);
    layout->addLayout(right_side);

    connect(this->genres_dropdown, &QComboBox::currentTextChanged, [this](const QString& genre) {
        this->genre_movies  = emit this->get_movies(genre);
        this->current_movie = this->genre_movies.end();
        this->current_movie_label->clear();
    });

    connect(this->next_button, &QPushButton::clicked, [this]() {
        if (this->current_movie >= this->genre_movies.end() - 1)
            this->current_movie = this->genre_movies.begin();
        else
            ++this->current_movie;
        this->current_movie_label->setText(QString::fromStdString(current_movie->to_string()));
        current_movie->play();
    });

    connect(this->watchlist_list, &QListWidget::currentRowChanged,
            [this](int row) { this->selected_id = emit this->get_movie(row).get_id(); });

    connect(this->add_button, &QPushButton::clicked,
            [this]() { emit this->watchlist_add(*this->current_movie); });

    connect(this->remove_button, &QPushButton::clicked,
            [this]() { emit this->watchlist_remove(this->selected_id, this->liked_checkbox->isChecked()); });

    connect(this->open_file_button, &QPushButton::clicked, [this]() { emit this->watchlist_open(); });
}
