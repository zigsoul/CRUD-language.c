# Guia do Desenvolvedor / Guía del Desarrollador / Developer's Guide

## 🇧🇷 Português

### Extendendo o Sistema

1. **Adicionar Novo Campo ao Registro**
```c
// 1. Atualizar struct em crud.h
typedef struct {
    // ... campos existentes ...
    char novo_campo[TAM_NOVO_CAMPO];
} Registro;

// 2. Atualizar funções de E/S
void criar_registro() {
    // ... código existente ...
    strncpy(novo.novo_campo, buffer, sizeof(novo.novo_campo) - 1);
}

// 3. Atualizar funções de arquivo
void salvar_arquivo() {
    fprintf(arquivo, "%s|", registro.novo_campo);
}
```

2. **Adicionar Nova Funcionalidade**
```c
// 1. Declarar em crud.h
void nova_funcao();

// 2. Implementar em crud.c
void nova_funcao() {
    // Implementação
}

// 3. Adicionar ao menu
printf("X. Nova funcionalidade\n");
case X: nova_funcao(); break;
```

### Boas Práticas

1. **Validação de Entrada**
```c
// Sempre limpar o buffer
int c;
while ((c = getchar()) != '\n' && c != EOF);

// Sempre validar tamanho
if (strlen(input) >= tamanho_maximo) {
    printf("❌ Erro: Entrada muito longa\n");
    return ERRO_VALIDACAO;
}
```

2. **Tratamento de Erros**
```c
// Verificar alocação de memória
if (!ponteiro) {
    printf("❌ Erro: Falha na alocação de memória\n");
    return ERRO_MEMORIA;
}

// Verificar operações de arquivo
if (!arquivo) {
    printf("❌ Erro: Falha ao abrir arquivo\n");
    return ERRO_ARQUIVO;
}
```

---

## 🇪🇸 Español

### Extendiendo el Sistema

1. **Agregar Nuevo Campo al Registro**
```c
// 1. Actualizar struct en crud.h
typedef struct {
    // ... campos existentes ...
    char nuevo_campo[TAM_NUEVO_CAMPO];
} Registro;

// 2. Actualizar funciones de E/S
void crear_registro() {
    // ... código existente ...
    strncpy(nuevo.nuevo_campo, buffer, sizeof(nuevo.nuevo_campo) - 1);
}

// 3. Actualizar funciones de archivo
void guardar_archivo() {
    fprintf(archivo, "%s|", registro.nuevo_campo);
}
```

2. **Agregar Nueva Funcionalidad**
```c
// 1. Declarar en crud.h
void nueva_funcion();

// 2. Implementar en crud.c
void nueva_funcion() {
    // Implementación
}

// 3. Agregar al menú
printf("X. Nueva funcionalidad\n");
case X: nueva_funcion(); break;
```

### Buenas Prácticas

1. **Validación de Entrada**
```c
// Siempre limpiar el buffer
int c;
while ((c = getchar()) != '\n' && c != EOF);

// Siempre validar tamaño
if (strlen(input) >= tamano_maximo) {
    printf("❌ Error: Entrada demasiado larga\n");
    return ERROR_VALIDACION;
}
```

2. **Manejo de Errores**
```c
// Verificar asignación de memoria
if (!puntero) {
    printf("❌ Error: Fallo en asignación de memoria\n");
    return ERROR_MEMORIA;
}

// Verificar operaciones de archivo
if (!archivo) {
    printf("❌ Error: Fallo al abrir archivo\n");
    return ERROR_ARCHIVO;
}
```

---

## 🇺🇸 English

### Extending the System

1. **Adding New Field to Record**
```c
// 1. Update struct in crud.h
typedef struct {
    // ... existing fields ...
    char new_field[NEW_FIELD_SIZE];
} Record;

// 2. Update I/O functions
void create_record() {
    // ... existing code ...
    strncpy(new.new_field, buffer, sizeof(new.new_field) - 1);
}

// 3. Update file functions
void save_file() {
    fprintf(file, "%s|", record.new_field);
}
```

2. **Adding New Functionality**
```c
// 1. Declare in crud.h
void new_function();

// 2. Implement in crud.c
void new_function() {
    // Implementation
}

// 3. Add to menu
printf("X. New functionality\n");
case X: new_function(); break;
```

### Best Practices

1. **Input Validation**
```c
// Always clear buffer
int c;
while ((c = getchar()) != '\n' && c != EOF);

// Always validate size
if (strlen(input) >= maximum_size) {
    printf("❌ Error: Input too long\n");
    return VALIDATION_ERROR;
}
```

2. **Error Handling**
```c
// Check memory allocation
if (!pointer) {
    printf("❌ Error: Memory allocation failed\n");
    return MEMORY_ERROR;
}

// Check file operations
if (!file) {
    printf("❌ Error: Failed to open file\n");
    return FILE_ERROR;
}
```

### Testing

1. **Unit Tests**
```c
void test_validar_email() {
    assert(validar_email("test@email.com") == SUCESSO);
    assert(validar_email("invalid") == ERRO_EMAIL_INVALIDO);
}

void test_validar_telefone() {
    assert(validar_telefone("1234567890") == SUCESSO);
    assert(validar_telefone("123") == ERRO_TELEFONE_INVALIDO);
}
```

2. **Memory Tests**
```c
// Use valgrind for memory leak detection
valgrind --leak-check=full ./crud
```

### Code Style Guide

1. **Naming Conventions**
- Use descriptive names in the respective language
- Functions: verb_noun format
- Constants: ALL_CAPS
- Variables: snake_case

2. **Comments**
- Add function documentation
- Explain complex logic
- Use language-specific comments for each section

3. **Error Messages**
- Always use emojis for visual feedback
- Include error description
- Provide user guidance when possible

### Version Control

1. **Commit Messages**
```
feat: add new field to record structure
fix: correct memory leak in file handling
docs: update developer documentation
test: add unit tests for email validation
```

2. **Branch Strategy**
- main: stable releases
- develop: development branch
- feature/*: new features
- fix/*: bug fixes

### Build System

1. **Makefile Targets**
```makefile
all: crud test_crud
crud: main.o crud.o
test_crud: test_crud.o crud.o
clean: rm -f *.o crud test_crud
```

2. **Compilation Flags**
```makefile
CFLAGS = -Wall -Wextra -pedantic
DEBUG_FLAGS = -g -DDEBUG
