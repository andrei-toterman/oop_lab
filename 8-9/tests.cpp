﻿#include "tests.h"
#include "controller.h"
#include "repository.h"
#include <cassert>

void test_repo_add() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.add(Movie("g", "e", 5, 6, "i"));
    int size = repo.size();
    assert(size == 3);
    size = static_cast<int>(repo.get_genres().size());
    assert(size == 2);
    repo.add(Movie("j", "k", 7, 8, "l"));
}

void test_repo_remove() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.remove("a_1");
    int size = repo.size();
    assert(size == 1);
    size = static_cast<int>(repo.get_genres().size());
    assert(size == 1);
    try {
        repo.remove("a_1");
        assert(false);
    } catch (...) { assert(true); }
}

void test_repo_index_operator() {
    MovieRepo repo;
    Movie     m{ "a", "b", 1, 2, "c" };
    repo.add(m);
    assert(repo[0] == m);
    try {
        repo[1];
        assert(false);
    } catch (...) { assert(true); }
}

void test_repo_find() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    auto movie_iter = repo.find("a_1");
    auto repo_end   = repo.get_movies().end();
    assert(movie_iter != repo_end);
    movie_iter = repo.find("a_2");
    repo_end   = repo.get_movies().end();
    assert(movie_iter == repo_end);
}

void test_repo_assign_operator() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    MovieRepo new_repo;
    new_repo = repo;
    int size = new_repo.size();
    assert(size == 1);
    assert(new_repo[0] == repo[0]);
}

void test_repo_update() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.update("a_1", Movie{ "c", "d", 2, 3, "e" });
    auto movie_iter = repo.find("c_2");
    auto repo_end   = repo.get_movies().end();
    assert(movie_iter != repo_end);
    movie_iter = repo.find("a_1");
    repo_end   = repo.get_movies().end();
    assert(movie_iter == repo_end);
}

void test_repo_filter() {
    MovieRepo repo;
    repo.populate();
    MovieRepo filtered{ repo.filter_by([](Movie m) { return m.get_genre() == "action"; }) };
    int       size = filtered.size();
    assert(size == 3);
    assert(filtered[0].get_id() == "Avengers_2019");
    assert(filtered[1].get_id() == "Avengers_2018");
    assert(filtered[2].get_id() == "Batman_2008");
}

void test_repo() {
    test_repo_add();
    test_repo_remove();
    test_repo_index_operator();
    test_repo_assign_operator();
    test_repo_find();
    test_repo_update();
    test_repo_filter();
}

void test_ctrl_add() {
    MovieRepo  database;
    Controller ctrl{ database, new MovieRepo() };
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    int size = ctrl.get_database().size();
    assert(size == 1);
    try {
        ctrl.database_add("a", "b", 1, 2, "www.google.com");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("", "a", 1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("a", "", 1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("a", "b", -1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("a", "b", 1, -2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("a", "b", 1, 2, "");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_add("a", "b", 1, 2, "dbkjgrbg");
        assert(false);
    } catch (...) { assert(true); }

    ctrl.watchlist_add(database[0]);
    size = ctrl.get_watchlist()->size();
    assert(size == 1);
}

void test_ctrl_remove() {
    MovieRepo  database;
    Controller ctrl{ database, new MovieRepo() };
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.database_add("b", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.watchlist_add(database[1]);
    ctrl.database_remove("a_1");
    int size = ctrl.get_database().size();
    assert(size == 1);
    size = ctrl.get_watchlist()->size();
    assert(size == 1);
    ctrl.watchlist_remove("b_1", true);
    size = ctrl.get_watchlist()->size();
    assert(size == 0);
    try {
        ctrl.database_remove("adwada");
        assert(false);
    } catch (...) { assert(true); }
}

void test_ctrl_update() {
    MovieRepo  database;
    MovieRepo* watchlist = new MovieRepo();
    Controller ctrl{ database, watchlist };
    Movie      m{ "b", "c", 2, 3, "www.reddit.com" };
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.database_update("a_1", "b", "c", 2, 3, "www.reddit.com");
    assert(database[0] == m);
    assert((*watchlist)[0] == m);
    try {
        ctrl.database_update("a_1", "b", "c", 2, 3, "www.reddit.com");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "", "a", 1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "a", "", 1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "a", "b", -1, 2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "a", "b", 1, -2, "www.");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "a", "b", 1, 2, "");
        assert(false);
    } catch (...) { assert(true); }
    try {
        ctrl.database_update("b_2", "a", "b", 1, 2, "dbkjgrbg");
        assert(false);
    } catch (...) { assert(true); }
}

void test_ctrl() {
    test_ctrl_add();
    test_ctrl_remove();
    test_ctrl_update();
}
