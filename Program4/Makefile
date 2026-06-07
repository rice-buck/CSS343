CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC := src
BIN := bin

CORE := $(SRC)/Movie.cpp $(SRC)/Comedy.cpp $(SRC)/Drama.cpp $(SRC)/Classic.cpp \
        $(SRC)/Customer.cpp $(SRC)/Database.cpp $(SRC)/Operations.cpp \
        $(SRC)/Transaction.cpp $(SRC)/Borrow.cpp $(SRC)/Return.cpp \
        $(SRC)/History.cpp $(SRC)/Inventory.cpp

.PHONY: all run test clean

all: $(BIN)/program4

$(BIN):
	mkdir -p $(BIN)

$(BIN)/program4: $(SRC)/main.cpp $(CORE) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BIN)/program4
	./$(BIN)/program4

# ---- unit / integration tests -------------------------------------------

TESTS := test_movie test_customer test_hashtable test_database test_operations test_endtoend

test: $(addprefix $(BIN)/,$(TESTS))
	@for t in $(TESTS); do \
		echo "== $$t =="; \
		./$(BIN)/$$t || exit 1; \
		echo; \
	done

$(BIN)/test_movie: $(SRC)/tests/test_movie.cpp $(CORE) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_customer: $(SRC)/tests/test_customer.cpp $(SRC)/Customer.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_hashtable: $(SRC)/tests/test_hashtable.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_database: $(SRC)/tests/test_database.cpp $(CORE) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_operations: $(SRC)/tests/test_operations.cpp $(CORE) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_endtoend: $(SRC)/tests/test_endtoend.cpp $(CORE) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BIN)
