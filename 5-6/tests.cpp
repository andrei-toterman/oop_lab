#include "tests.h"
#include "controller.h"
#include "dynamicarray.h"
#include "repository.h"
#include <cassert>

void test_dv_add() {
    DynamicVector<int> vector;
    assert(vector.size() == 0);
    vector.add(1);
    assert(vector.size() == 1);
    assert(vector[0] == 1);
}

void test_dv_remove() {
    DynamicVector<int> vector;
    vector.add(1);
    vector.remove(0);
    assert(vector.size() == 0);
    try {
        vector.remove(0);
        assert(false);
    } catch (...) { assert(true); }
}

void test_dv_index_operator() {
    DynamicVector<int> vector;
    vector.add(1);
    assert(vector[0] == 1);
    try {
        vector[1];
        assert(false);
    } catch (...) { assert(true); }
}

void test_dv_assign_operator() {
    DynamicVector<int> vector;
    vector.add(1);
    DynamicVector<int> vector2;
    vector2 = vector;
    assert(vector2.size() == 1);
    assert(vector2[0] == 1);
}

void test_dv_plus_operator() {
    DynamicVector<int> vector;
    assert(vector.size() == 0);
    vector + 1;
    2 + vector;
    assert(vector.size() == 2);
    assert(vector[0] == 1);
    assert(vector[1] == 2);
}

void test_dv() {
    test_dv_add();
    test_dv_remove();
    test_dv_index_operator();
    test_dv_assign_operator();
    test_dv_plus_operator();
}

void test_repo_add() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.add(Movie("g", "e", 5, 6, "i"));
    assert(repo.get_movies().size() == 3);
    assert(repo.get_genres().size() == 2);
    repo.add(Movie("j", "k", 7, 8, "l"));
    assert(repo.get_movies().capacity() == 6);
}

void test_repo_remove() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.remove(0);
    assert(repo.size() == 1);
    assert(repo.get_genres().size() == 1);
    try {
        repo.remove(1);
        assert(false);
    } catch (...) { assert(true); }
}

void test_repo_index_operator() {
    MovieRepo repo;
    Movie     m{"a", "b", 1, 2, "c"};
    repo.add(m);
    assert(repo.get_movies()[0] == m);
    try {
        repo.get_movies()[1];
        assert(false);
    } catch (...) { assert(true); }
}

void test_repo_find() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    assert(repo.find("a_1") >= 0);
    assert(repo.find("a_2") == -1);
}

void test_repo_assign_operator() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    MovieRepo new_repo;
    new_repo = repo;
    assert(new_repo.size() == 1);
    assert(new_repo.get_movies().capacity() == 3);
    assert(new_repo[0] == repo[0]);
}

void test_repo_update() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.update(0, Movie{"c", "d", 2, 3, "e"});
    assert(repo.find("c_2") >= 0);
    assert(repo.find("a_1") == -1);
}

void test_repo_filter() {
    MovieRepo repo;
    repo.populate();
    MovieRepo filtered{repo.filter_by([](Movie m) { return m.get_genre() == "action"; })};
    assert(filtered.size() == 3);
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
    MovieRepo  watchlist;
    Controller ctrl{database, watchlist};
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    assert(ctrl.get_database().size() == 1);
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
    assert(ctrl.get_watchlist().size() == 1);
}

void test_ctrl_remove() {
    MovieRepo  database;
    MovieRepo  watchlist;
    Controller ctrl{database, watchlist};
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.database_add("b", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.watchlist_add(database[1]);
    ctrl.database_remove("a_1");
    assert(ctrl.get_database().size() == 1);
    assert(ctrl.get_watchlist().size() == 1);
    ctrl.watchlist_remove("b_1", true);
    assert(ctrl.get_watchlist().size() == 0);
    try {
        ctrl.database_remove("adwada");
        assert(false);
    } catch (...) { assert(true); }
}

void test_ctrl_update() {
    MovieRepo  database;
    MovieRepo  watchlist;
    Controller ctrl{database, watchlist};
    Movie      m{"b", "c", 2, 3, "www.reddit.com"};
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.database_update("a_1", "b", "c", 2, 3, "www.reddit.com");
    assert(database[0] == m);
    assert(watchlist[0] == m);
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
