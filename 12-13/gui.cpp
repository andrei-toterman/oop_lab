#include "gui.h"

#include <QFormLayout>

GUI::GUI(Controller& _ctrl, QWidget* parent) : QWidget(parent), ctrl{ _ctrl } {
    this->tabs = new QTabWidget();
    this->tabs->addTab(new Admin(this->ctrl), "Admin");
    this->tabs->addTab(new User(this->ctrl), "User");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(this->tabs);

    setWindowTitle("LMDB");
}

void Admin::refresh_list() {
    for (Movie& m : this->ctrl.get_database().get_movies())
        this->database_list->addItem(new QListWidgetItem(QString::fromStdString(m.to_string())));
}

Admin::Admin(Controller& _ctrl, QWidget* parent) :
    QWidget(parent),
    ctrl{ _ctrl },
    database_list{ new QListWidget() },
    add_button{ new QPushButton("Add") },
    remove_button{ new QPushButton("Remove") },
    update_button{ new QPushButton("Update") },
    title_line{ new QLineEdit() },
    genre_line{ new QLineEdit() },
    year_line{ new QLineEdit() },
    likes_line{ new QLineEdit() },
    trailer_line{ new QLineEdit() } {

    this->database_list->setSelectionMode(QAbstractItemView::SingleSelection);

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

    QHBoxLayout* buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(this->add_button);
    buttons_layout->addWidget(this->remove_button);
    buttons_layout->addWidget(this->update_button);
    right_side->addLayout(buttons_layout);

    layout->addLayout(left_side);
    layout->addLayout(right_side);

    this->refresh_list();
}

User::User(Controller& _ctrl, QWidget* parent) :
    QWidget(parent),
    ctrl{ _ctrl },
    watchlist_list{ new QListWidget() },
    genres_list{ new QListWidget() },
    choose_csv_button{ new QPushButton("CSV") },
    choose_html_button{ new QPushButton{ "HTML" } },
    add_button{ new QPushButton("Add") },
    remove_button{ new QPushButton("Remove") },
    next_button{ new QPushButton("Next") },
    open_file_button{ new QPushButton("Open file") },
    liked_checkbox{ new QCheckBox("Liked") },
    current_movie{ new QLabel("Current movie:\n") } {

    this->watchlist_list->setSelectionMode(QAbstractItemView::SingleSelection);
    this->genres_list->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout* layout                   = new QVBoxLayout(this);
    QHBoxLayout* choose_format_layout     = new QHBoxLayout();
    QHBoxLayout* main_layout              = new QHBoxLayout();
    QVBoxLayout* left_side                = new QVBoxLayout();
    QVBoxLayout* right_side               = new QVBoxLayout();
    QHBoxLayout* watchlist_buttons_layout = new QHBoxLayout();
    QHBoxLayout* play_layout              = new QHBoxLayout();

    choose_format_layout->addWidget(this->choose_csv_button);
    choose_format_layout->addWidget(this->choose_html_button);

    watchlist_buttons_layout->addWidget(this->open_file_button);
    watchlist_buttons_layout->addWidget(this->remove_button);
    watchlist_buttons_layout->addWidget(this->liked_checkbox);

    left_side->addWidget(new QLabel("Watchlist"));
    left_side->addWidget(this->watchlist_list);
    left_side->addLayout(watchlist_buttons_layout);

    play_layout->addWidget(this->next_button);
    play_layout->addWidget(this->add_button);

    right_side->addWidget(new QLabel("Genres"));
    right_side->addWidget(this->genres_list);
    right_side->addWidget(this->current_movie);
    right_side->addLayout(play_layout);

    main_layout->addLayout(left_side);
    main_layout->addLayout(right_side);

    layout->addLayout(choose_format_layout);
    layout->addLayout(main_layout);
}
