PRESET_DEBUG       := debug

BUILD_DIR_DEBUG      := build/$(PRESET_DEBUG)

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES := $(call rwildcard,include/,*.hpp) \
           $(call rwildcard,tests/,*.cpp)

.PHONY: help configure build test rebuild \
        clean format format-check tidy compdb

help:
	@echo "Comandos disponiveis:"
	@echo "  make build            - Configura (se necessario) e compila em Debug"
	@echo "  make test             - Roda os testes (build automatico antes, se preciso)"
	@echo "  make rebuild          - Limpa e recompila do zero em Debug"
	@echo "  make format           - Formata todo o codigo com clang-format"
	@echo "  make format-check     - So verifica formatacao, sem alterar arquivos (usado no CI)"
	@echo "  make tidy             - Roda clang-tidy em todo o codigo"
	@echo "  make compdb           - Recria o link compile_commands.json na raiz (para clangd)"
	@echo "  make clean            - Remove todos os diretorios de build"

# -----------------------------------------------------------------------------
# Build Debug
# -----------------------------------------------------------------------------

$(BUILD_DIR_DEBUG)/CMakeCache.txt: CMakeLists.txt CMakePresets.json
	cmake --preset $(PRESET_DEBUG)

configure: $(BUILD_DIR_DEBUG)/CMakeCache.txt

build: configure
	cmake --build --preset $(PRESET_DEBUG)

test: build
	ctest --preset $(PRESET_DEBUG) --output-on-failure -V

rebuild: clean build

# -----------------------------------------------------------------------------
# Formatacão e lint
# -----------------------------------------------------------------------------

format:
	@echo "Formatando $(words $(SOURCES)) arquivo(s)..."
	clang-format -i $(SOURCES)

format-check:
	clang-format --dry-run --Werror $(SOURCES)

tidy: configure
	clang-tidy -p $(BUILD_DIR_DEBUG) $(SOURCES)

# -----------------------------------------------------------------------------
# compile_commands.json na raiz (para clangd no VSCode/nvim)
# -----------------------------------------------------------------------------
compdb: configure
	ln -sf $(BUILD_DIR_DEBUG)/compile_commands.json compile_commands.json

# -----------------------------------------------------------------------------
# Limpeza
# -----------------------------------------------------------------------------
clean:
	rm -rf build
