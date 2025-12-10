# --- Configurazione ---
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -O0
TARGET   = btree_app

# --- File Sorgente ---
# Solo il file che contiene il main va qui!
MAIN_SRC = example.cpp
OBJ      = $(MAIN_SRC:.cpp=.o)

# --- Dipendenze (Header e Implementazioni Template) ---
# Qui elenchiamo tutti i file che, se modificati, richiedono una ricompilazione.
# Nota: Btree.cpp e BtreeNode.cpp sono qui perché sono template!
DEPS     = Btree.hpp Btree.cpp BtreeNode.hpp BtreeNode.cpp

# --- Regole ---

all: $(TARGET)

# Linkaggio
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)
	@echo "Build completata: ./$(TARGET)"

# Compilazione di example.o
# Questa regola dice: "Ricompila example.o se cambia example.cpp O uno qualsiasi dei file in DEPS"
$(OBJ): $(MAIN_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

check: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

.PHONY: all clean check