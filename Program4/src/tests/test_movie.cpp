// Unit tests for Movie / Comedy / Drama / Classic.
//
// The most important property under test is genHashKey(): the lookup path
// (Operations::parseMovieIdentifier -> Database::find) only works if a movie
// built from the *command* line produces the exact same key as the matching
// movie built from the *catalog* line. These tests pin that contract down.
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../Comedy.h"
#include "../Drama.h"
#include "../Classic.h"

static void test_basic_accessors() {
    Comedy c('F', 10, "You've Got Mail", "Nora Ephron", 1998);
    assert(c.getGenre() == 'F');
    assert(c.getStock() == 10);
    assert(c.getMovieTitle() == "You've Got Mail");
    assert(c.getDirector() == "Nora Ephron");
    assert(c.getYear() == 1998);
}

static void test_set_stock_rejects_negative() {
    Comedy c('F', 10, "Title", "Director", 2000);
    c.setStock(-5);
    assert(c.getStock() == 10);   // unchanged
    c.setStock(3);
    assert(c.getStock() == 3);
}

static void test_movie_validation_throws() {
    bool threw = false;
    try { Comedy bad('Z', 10, "Title", "Director", 2000); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { Comedy bad('F', -1, "Title", "Director", 2000); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { Comedy bad('F', 10, "", "Director", 2000); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}

static void test_classic_validation_throws() {
    bool threw = false;
    try { Classic bad('C', 10, "Title", "Director", "", 9, 1938); }
    catch (const std::invalid_argument&) { threw = true; }   // empty actor
    assert(threw);

    threw = false;
    try { Classic bad('C', 10, "Title", "Director", "Actor", 13, 1938); }
    catch (const std::invalid_argument&) { threw = true; }   // bad month
    assert(threw);

    threw = false;
    try { Classic bad('C', 10, "Title", "Director", "Actor", 0, 1938); }
    catch (const std::invalid_argument&) { threw = true; }   // bad month
    assert(threw);
}

// ---- genHashKey(): catalog entry vs. command-stub must match -------------

static void test_comedy_hashkey_matches_stub() {
    Comedy catalog('F', 10, "You've Got Mail", "Nora Ephron", 1998);
    Comedy stub("You've Got Mail", 1998);          // built like a B/R command
    assert(catalog.genHashKey() == stub.genHashKey());

    Comedy other("Sleepless in Seattle", 1993);
    assert(catalog.genHashKey() != other.genHashKey());
}

static void test_drama_hashkey_matches_stub() {
    Drama catalog('D', 10, "Good Morning Vietnam", "Barry Levinson", 1988);
    Drama stub("Barry Levinson", "Good Morning Vietnam");
    assert(catalog.genHashKey() == stub.genHashKey());

    // Director order matters for the key (it's part of identity + sort order).
    Drama swapped("Good Morning Vietnam", "Barry Levinson");
    assert(catalog.genHashKey() != swapped.genHashKey());
}

static void test_classic_hashkey_matches_stub() {
    Classic catalog('C', 10, "Holiday", "George Cukor", "Katherine Hepburn", 9, 1938);
    Classic stub(9, 1938, "Katherine Hepburn");
    assert(catalog.genHashKey() == stub.genHashKey());

    // Same title, different actor edition => distinct identity (per spec).
    Classic grant('C', 10, "Holiday", "George Cukor", "Cary Grant", 9, 1938);
    assert(catalog.genHashKey() != grant.genHashKey());
}

static void test_classic_actor_and_month_preserved() {
    // Guards against object slicing: storing/retrieving as Movie* must not
    // lose Classic-only fields.
    Movie* m = new Classic('C', 10, "Casablanca", "Michael Curtiz",
                           "Ingrid Bergman", 8, 1942);
    Classic* c = dynamic_cast<Classic*>(m);
    assert(c != nullptr);
    assert(c->getActor() == "Ingrid Bergman");
    assert(c->getMonth() == 8);
    delete m;
}

int main() {
    test_basic_accessors();              std::cout << "  basic accessors          OK\n";
    test_set_stock_rejects_negative();   std::cout << "  setStock rejects <0      OK\n";
    test_movie_validation_throws();      std::cout << "  Movie ctor validation    OK\n";
    test_classic_validation_throws();    std::cout << "  Classic ctor validation  OK\n";
    test_comedy_hashkey_matches_stub();  std::cout << "  Comedy hashKey == stub   OK\n";
    test_drama_hashkey_matches_stub();   std::cout << "  Drama hashKey == stub    OK\n";
    test_classic_hashkey_matches_stub(); std::cout << "  Classic hashKey == stub  OK\n";
    test_classic_actor_and_month_preserved();
                                         std::cout << "  Classic survives Movie*  OK\n";
    std::cout << "All Movie tests passed.\n";
    return 0;
}
