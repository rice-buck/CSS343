#include "Borrow.h"
#include "Return.h"
#include "History.h"
#include "Inventory.h"
#include "Customer.h"
#include "Comedy.h"
#include "Classic.h"
#include "Drama.h"

//test transactions
int main() {
    //create some movies and customers
    Comedy movie1('F', 5, "The Big Lebowski", "Joel Coen", 1998);
    Drama movie2('D', 3, "The Shawshank Redemption", "Frank Darabont", 1994);
    Classic movie3('C', 2, "Casablanca", "Michael Curtiz", "Joe schmo", 12, 1942);
    Customer customer1(1, "Joe", "Mama");
    Customer customer2(2, "Jane", "Doe");
    //add movies and customers to database
    Database db;
    db.addMovie(movie1);
    db.addMovie(movie2);
    db.addMovie(movie3);
    db.addCustomer(customer1);
    db.addCustomer(customer2);
    Transaction* trans1 = new Borrow(db, 1, movie1.getMovieID());
    trans1->doTrans();
    Transaction* trans2 = new Borrow(db, 2, movie2.getMovieID());
    trans2->doTrans();
    Transaction* trans3 = new Return(db, 1, movie1.getMovieID());
    trans3->doTrans();
    Transaction* trans4 = new History(db, customer1.getID());
    trans4->doTrans();
    Transaction* trans5 = new Inventory(db);
    trans5->doTrans();
}
