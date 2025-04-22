# Documentação Técnica / Documentación Técnica / Technical Documentation

## 🇧🇷 Português

### Estrutura do Código

1. **Arquivos Principais**
   - `crud.h`: Definições de estruturas e protótipos
   - `crud.c`: Implementação das funções
   - `main.c`: Ponto de entrada do programa
   - `test_crud.c`: Testes unitários

2. **Estruturas de Dados**
```c
typedef struct {
    int id;
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char telefone[TAM_TELEFONE];
    time_t data_criacao;
    time_t data_modificacao;
} Registro;
```

3. **Funções Principais**
```c
void criar_registro();
void listar_registros();
void busca_avancada();
void atualizar_registro();
void deletar_registro();
```

4. **Validações**
```c
StatusCode validar_email(const char* email);
StatusCode validar_telefone(const char* telefone);
```

### Exemplos de Uso

1. **Criar Registro**
```c
Registro novo;
novo.id = total_registros + 1;
strncpy(novo.nome, "João Silva", TAM_NOME);
strncpy(novo.email, "joao@email.com", TAM_EMAIL);
```

2. **Exportar CSV**
```c
void exportar_csv() {
    // Nome do arquivo com timestamp
    strftime(nome_arquivo, sizeof(nome_arquivo), 
             "exportacao_%Y%m%d_%H%M%S.csv", timeinfo);
}
```

---

## 🇪🇸 Español

### Estructura del Código

1. **Archivos Principales**
   - `crud.h`: Definiciones de estructuras y prototipos
   - `crud.c`: Implementación de funciones
   - `main.c`: Punto de entrada del programa
   - `test_crud.c`: Pruebas unitarias

2. **Estructuras de Datos**
```c
typedef struct {
    int id;
    char nombre[TAM_NOMBRE];
    char email[TAM_EMAIL];
    char telefono[TAM_TELEFONO];
    time_t fecha_creacion;
    time_t fecha_modificacion;
} Registro;
```

3. **Funciones Principales**
```c
void crear_registro();
void listar_registros();
void busqueda_avanzada();
void actualizar_registro();
void eliminar_registro();
```

4. **Validaciones**
```c
StatusCode validar_email(const char* email);
StatusCode validar_telefono(const char* telefono);
```

### Ejemplos de Uso

1. **Crear Registro**
```c
Registro nuevo;
nuevo.id = total_registros + 1;
strncpy(nuevo.nombre, "Juan Silva", TAM_NOMBRE);
strncpy(nuevo.email, "juan@email.com", TAM_EMAIL);
```

2. **Exportar CSV**
```c
void exportar_csv() {
    // Nombre del archivo con marca de tiempo
    strftime(nombre_archivo, sizeof(nombre_archivo), 
             "exportacion_%Y%m%d_%H%M%S.csv", timeinfo);
}
```

---

## 🇺🇸 English

### Code Structure

1. **Main Files**
   - `crud.h`: Structure definitions and prototypes
   - `crud.c`: Function implementations
   - `main.c`: Program entry point
   - `test_crud.c`: Unit tests

2. **Data Structures**
```c
typedef struct {
    int id;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char phone[PHONE_SIZE];
    time_t creation_date;
    time_t modification_date;
} Record;
```

3. **Main Functions**
```c
void create_record();
void list_records();
void advanced_search();
void update_record();
void delete_record();
```

4. **Validations**
```c
StatusCode validate_email(const char* email);
StatusCode validate_phone(const char* phone);
```

### Usage Examples

1. **Create Record**
```c
Record new;
new.id = total_records + 1;
strncpy(new.name, "John Smith", NAME_SIZE);
strncpy(new.email, "john@email.com", EMAIL_SIZE);
```

2. **Export CSV**
```c
void export_csv() {
    // Filename with timestamp
    strftime(filename, sizeof(filename), 
             "export_%Y%m%d_%H%M%S.csv", timeinfo);
}
```

### Compilation and Execution

```bash
# Compile
gcc -o crud main.c crud.c -Wall

# Run
./crud

# Run tests
gcc -o test_crud test_crud.c crud.c -Wall
./test_crud
```

---

## 🔧 Common Error Codes / Códigos de Erro / Códigos de Error

```c
typedef enum {
    SUCESSO / SUCCESS / ÉXITO,
    ERRO_VALIDACAO / VALIDATION_ERROR / ERROR_VALIDACION,
    ERRO_ARQUIVO / FILE_ERROR / ERROR_ARCHIVO,
    ERRO_MEMORIA / MEMORY_ERROR / ERROR_MEMORIA,
    ERRO_EMAIL_INVALIDO / INVALID_EMAIL_ERROR / ERROR_EMAIL_INVALIDO,
    ERRO_TELEFONE_INVALIDO / INVALID_PHONE_ERROR / ERROR_TELEFONO_INVALIDO,
    ERRO_MAX_TENTATIVAS / MAX_ATTEMPTS_ERROR / ERROR_MAX_INTENTOS
} StatusCode;
```

## 📚 Constants / Constantes

```c
#define MAX_REGISTROS / MAX_RECORDS / MAX_REGISTROS 100
#define TAM_NOME / NAME_SIZE / TAM_NOMBRE 50
#define TAM_EMAIL / EMAIL_SIZE / TAM_EMAIL 50
#define TAM_TELEFONE / PHONE_SIZE / TAM_TELEFONO 15
#define MAX_TENTATIVAS / MAX_ATTEMPTS / MAX_INTENTOS 3
#define BUFFER_SIZE 256
