#include "crud.h"
#include <locale.h>
#include <time.h>
#include <string.h>
#include <strings.h>  // Para strcasecmp
#include <ctype.h>
#include <sys/stat.h>

#define MAX_TENTATIVAS 3
#define MIN_NOME_LENGTH 2

StatusCode validar_nome(const char* nome) {
    if (!nome || strlen(nome) == 0) {
        return ERRO_VALIDACAO;
    }

    // Verificar comprimento mínimo
    if (strlen(nome) < MIN_NOME_LENGTH) {
        return ERRO_VALIDACAO;
    }

    return SUCESSO;
}

#define BUFFER_SIZE 256

Registro registros[MAX_REGISTROS];
int total_registros = 0;

StatusCode validar_email(const char* email) {
    if (!email || strlen(email) == 0) {
        return ERRO_VALIDACAO;
    }

    // Verificar comprimento mínimo: x@y.z
    if (strlen(email) < 5) {
        return ERRO_EMAIL_INVALIDO;
    }

    // Procurar por @
    const char* arroba = strchr(email, '@');
    if (!arroba) {
        return ERRO_EMAIL_INVALIDO;
    }

    // Verificar se @ não é o primeiro caractere
    if (arroba == email) {
        return ERRO_EMAIL_INVALIDO;
    }

    // Procurar por . após @
    const char* ponto = strchr(arroba, '.');
    if (!ponto || ponto == arroba + 1) {
        return ERRO_EMAIL_INVALIDO;
    }

    // Verificar se tem caracteres após o .
    if (strlen(ponto + 1) < 2) {
        return ERRO_EMAIL_INVALIDO;
    }

    return SUCESSO;
}

StatusCode validar_telefone(const char* telefone) {
    if (!telefone || strlen(telefone) == 0) {
        printf("DEBUG: Telefone nulo ou vazio\n");
        return ERRO_VALIDACAO;
    }

    // Verificar comprimento
    size_t len = strlen(telefone);
    printf("DEBUG: Comprimento do telefone: %zu\n", len);
    if (len < 9 || len > 14) {
        printf("DEBUG: Comprimento inválido\n");
        return ERRO_TELEFONE_INVALIDO;
    }

    // Verificar se contém apenas números, parênteses, hífen ou espaço
    for (size_t i = 0; i < len; i++) {
        char c = telefone[i];
        if (!isdigit(c) && c != '(' && c != ')' && c != '-' && c != ' ') {
            printf("DEBUG: Caractere inválido encontrado: %c\n", c);
            return ERRO_TELEFONE_INVALIDO;
        }
    }

    // Contar dígitos
    int digitos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(telefone[i])) {
            digitos++;
        }
    }
    printf("DEBUG: Total de dígitos: %d\n", digitos);

    // Deve ter entre 9 e 11 dígitos
    if (digitos < 9 || digitos > 11) {
        printf("DEBUG: Número de dígitos inválido\n");
        return ERRO_TELEFONE_INVALIDO;
    }

    printf("DEBUG: Telefone válido: %s\n", telefone);
    return SUCESSO;
}

void inicializar_sistema() {
    setlocale(LC_ALL, "Portuguese");
    carregar_arquivo();
}

void menu_principal() {
    int opcao;
    do {
        printf("\n📋 === MENU PRINCIPAL ===\n");
        printf("1. 📝 Criar registro\n");
        printf("2. 📋 Listar registros\n");
        printf("3. 🔍 Busca avançada\n");
        printf("4. ✏️  Atualizar registro\n");
        printf("5. ❌ Deletar registro\n");
        printf("6. 📊 Exportar para CSV\n");
        printf("7. 💾 Criar backup\n");
        printf("8. 📥 Restaurar backup\n");
        printf("0. 🚪 Sair\n");
        printf("\nEscolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            // Limpar buffer em caso de entrada inválida
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("❌ Erro: Opção inválida! Por favor, digite um número.\n");
            continue;
        }

        switch(opcao) {
            case 1: criar_registro(); break;
            case 2: listar_registros(); break;
            case 3: busca_avancada(); break;
            case 4: atualizar_registro(); break;
            case 5: deletar_registro(); break;
            case 6: exportar_csv(); break;
            case 7: criar_backup(); break;
            case 8: restaurar_backup(); break;
            case 0: 
                salvar_arquivo(); 
                printf("👋 Até logo!\n");
                break;
            default: 
                printf("❌ Erro: Opção inválida! Escolha um número entre 0 e 8.\n");
        }
    } while(opcao != 0);
}

char* ler_input_seguro(char* prompt, char* buffer, int tamanho) {
    int tentativas = 0;
    while (tentativas < MAX_TENTATIVAS) {
        printf("%s", prompt);
        
        // Limpar o buffer antes de ler
        memset(buffer, 0, tamanho);
        
        if (fgets(buffer, tamanho, stdin) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';  // Remover o \n
                
                // Verificar se a entrada está vazia
                if (strlen(buffer) == 0) {
                    printf("❌ Erro: A entrada não pode estar vazia. Tentativa %d de %d.\n", 
                           tentativas + 1, MAX_TENTATIVAS);
                    tentativas++;
                    continue;
                }
                
                return buffer;
            } else {
                // Limpar o buffer de entrada se a linha for muito longa
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("❌ Erro: Entrada muito longa (máximo %d caracteres). Tentativa %d de %d.\n", 
                       tamanho - 1, tentativas + 1, MAX_TENTATIVAS);
            }
        } else {
            printf("❌ Erro: Falha ao ler entrada. Tentativa %d de %d.\n", 
                   tentativas + 1, MAX_TENTATIVAS);
        }
        
        tentativas++;
        
        if (tentativas < MAX_TENTATIVAS) {
            printf("🔄 Por favor, tente novamente...\n");
        }
    }
    
    printf("❌ Erro: Número máximo de tentativas (%d) atingido.\n", MAX_TENTATIVAS);
    return NULL;
}

void criar_registro() {
    criar_registro_manual(NULL, NULL, NULL);
}

void criar_registro_manual(const char* nome, const char* email, const char* telefone) {
    if(total_registros >= MAX_REGISTROS) {
        printf("❌ Erro: Limite de registros atingido!\n");
        return;
    }

    Registro novo;
    novo.id = total_registros + 1;
    novo.data_criacao = time(NULL);
    novo.data_modificacao = novo.data_criacao;

    char buffer[BUFFER_SIZE];
    StatusCode status;

    // Se os parâmetros forem fornecidos, use-os (modo teste)
    if (nome != NULL && email != NULL && telefone != NULL) {
        printf("DEBUG: Criando registro com nome='%s', email='%s', telefone='%s'\n", nome, email, telefone);
        
        status = validar_nome(nome);
        if (status != SUCESSO) {
            printf("❌ Erro: Nome inválido!\n");
            return;
        }
        strncpy(novo.nome, nome, sizeof(novo.nome) - 1);
        novo.nome[sizeof(novo.nome) - 1] = '\0';
        
        status = validar_email(email);
        if (status != SUCESSO) {
            printf("❌ Erro: Email inválido!\n");
            return;
        }
        strncpy(novo.email, email, sizeof(novo.email) - 1);
        novo.email[sizeof(novo.email) - 1] = '\0';
        
        printf("DEBUG: Validando telefone: '%s'\n", telefone);
        status = validar_telefone(telefone);
        printf("DEBUG: Status da validação do telefone: %d\n", status);
        if (status != SUCESSO) {
            printf("❌ Erro: Telefone inválido (status=%d)!\n", status);
            return;
        }
        strncpy(novo.telefone, telefone, sizeof(novo.telefone) - 1);
        novo.telefone[sizeof(novo.telefone) - 1] = '\0';
    } else {
        // Modo interativo normal
        printf("\n📝 Novo Registro (ID: %d)\n", novo.id);
        
        // Limpar o buffer de entrada antes de ler
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // Nome
        if (!ler_input_seguro("Nome: ", buffer, sizeof(buffer))) {
            return;
        }
        strncpy(novo.nome, buffer, sizeof(novo.nome) - 1);
        novo.nome[sizeof(novo.nome) - 1] = '\0';
        
        // Email com validação
        do {
            if (!ler_input_seguro("Email: ", buffer, sizeof(buffer))) {
                return;
            }
            status = validar_email(buffer);
            if (status != SUCESSO) {
                printf("❌ Erro: Email inválido! Formato esperado: usuario@dominio.com\n");
            }
        } while (status != SUCESSO);
        strncpy(novo.email, buffer, sizeof(novo.email) - 1);
        novo.email[sizeof(novo.email) - 1] = '\0';
        
        // Telefone com validação
        do {
            if (!ler_input_seguro("Telefone: ", buffer, sizeof(buffer))) {
                return;
            }
            status = validar_telefone(buffer);
            if (status != SUCESSO) {
                printf("❌ Erro: Telefone inválido! Formato esperado: XXXXXXXXXX ou (XX)XXXXXXXX\n");
            }
        } while (status != SUCESSO);
        strncpy(novo.telefone, buffer, sizeof(novo.telefone) - 1);
        novo.telefone[sizeof(novo.telefone) - 1] = '\0';
    }

    registros[total_registros++] = novo;
    printf("✅ Registro criado com sucesso!\n");
}

void ordenar_registros(int campo, int ordem) {
    int i, j;
    Registro temp;
    
    for (i = 0; i < total_registros - 1; i++) {
        for (j = 0; j < total_registros - i - 1; j++) {
            int comparacao = 0;
            
            switch(campo) {
                case 1: // ID
                    comparacao = registros[j].id - registros[j + 1].id;
                    break;
                case 2: // Nome
                    comparacao = strcasecmp(registros[j].nome, registros[j + 1].nome);
                    break;
                case 3: // Email
                    comparacao = strcasecmp(registros[j].email, registros[j + 1].email);
                    break;
                case 4: // Data de criação
                    comparacao = registros[j].data_criacao - registros[j + 1].data_criacao;
                    break;
                case 5: // Data de modificação
                    comparacao = registros[j].data_modificacao - registros[j + 1].data_modificacao;
                    break;
            }
            
            if (ordem * comparacao > 0) {
                temp = registros[j];
                registros[j] = registros[j + 1];
                registros[j + 1] = temp;
            }
        }
    }
}

void listar_registros_paginados(int pagina, int registros_por_pagina) {
    if (total_registros == 0) {
        printf("ℹ️ Nenhum registro cadastrado!\n");
        return;
    }

    int total_paginas = (total_registros + registros_por_pagina - 1) / registros_por_pagina;
    
    if (pagina < 1 || pagina > total_paginas) {
        printf("❌ Erro: Página inválida!\n");
        return;
    }

    int inicio = (pagina - 1) * registros_por_pagina;
    int fim = inicio + registros_por_pagina;
    if (fim > total_registros) {
        fim = total_registros;
    }

    printf("\n📋 === LISTA DE REGISTROS (Página %d de %d) ===\n", pagina, total_paginas);
    printf("📊 Total de registros: %d\n", total_registros);
    
    for (int i = inicio; i < fim; i++) {
        exibir_registro(&registros[i]);
    }

    printf("\n📄 Navegação:\n");
    if (pagina > 1) {
        printf("< Página anterior (digite %d)\n", pagina - 1);
    }
    if (pagina < total_paginas) {
        printf("> Página seguinte (digite %d)\n", pagina + 1);
    }
    printf("0. Voltar ao menu\n");
}

void listar_registros_ordenados(int campo, int ordem) {
    if (total_registros == 0) {
        printf("ℹ️ Nenhum registro cadastrado!\n");
        return;
    }
    
    ordenar_registros(campo, ordem);
    
    const char* ordem_str = (ordem == ORDEM_CRESCENTE) ? "crescente" : "decrescente";
    const char* campo_str;
    
    switch(campo) {
        case 1: campo_str = "ID"; break;
        case 2: campo_str = "Nome"; break;
        case 3: campo_str = "Email"; break;
        case 4: campo_str = "Data de criação"; break;
        case 5: campo_str = "Data de modificação"; break;
        default: campo_str = "desconhecido";
    }

    int registros_por_pagina = 5;  // Número de registros por página
    int pagina = 1;

    do {
        printf("\n📋 === LISTA DE REGISTROS (Ordenado por %s - %s) ===\n", campo_str, ordem_str);
        listar_registros_paginados(pagina, registros_por_pagina);

        printf("\nDigite o número da página (0 para voltar): ");
        if (scanf("%d", &pagina) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("❌ Erro: Entrada inválida!\n");
            continue;
        }

        // Limpar buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

    } while (pagina != 0);
}

void listar_registros() {
    if (total_registros == 0) {
        printf("ℹ️ Nenhum registro cadastrado!\n");
        return;
    }

    int opcao, ordem;
    printf("\n📋 === LISTAR REGISTROS ===\n");
    printf("Ordenar por:\n");
    printf("1. 🔢 ID\n");
    printf("2. 👤 Nome\n");
    printf("3. 📧 Email\n");
    printf("4. 📅 Data de criação\n");
    printf("5. 🔄 Data de modificação\n");
    printf("0. ↩️  Voltar\n");
    printf("\nEscolha uma opção: ");

    if (scanf("%d", &opcao) != 1 || opcao < 0 || opcao > 5) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("❌ Erro: Opção inválida!\n");
        return;
    }

    if (opcao == 0) {
        return;
    }

    printf("\nOrdem:\n");
    printf("1. ⬆️  Crescente\n");
    printf("2. ⬇️  Decrescente\n");
    printf("\nEscolha uma opção: ");

    if (scanf("%d", &ordem) != 1 || ordem < 1 || ordem > 2) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("❌ Erro: Opção inválida!\n");
        return;
    }

    listar_registros_ordenados(opcao, ordem == 1 ? ORDEM_CRESCENTE : ORDEM_DECRESCENTE);
}

void busca_avancada() {
    char buffer[BUFFER_SIZE];
    int opcao;

    printf("\n🔍 === BUSCA AVANÇADA ===\n");
    printf("1. 👤 Buscar por nome\n");
    printf("2. 📧 Buscar por email\n");
    printf("3. 📱 Buscar por telefone\n");
    printf("4. 🔢 Buscar por ID\n");
    printf("0. ↩️  Voltar\n");
    printf("\nEscolha uma opção: ");

    if (scanf("%d", &opcao) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("❌ Erro: Opção inválida!\n");
        return;
    }

    // Limpar buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    switch(opcao) {
        case 1:
            if (ler_input_seguro("Digite o nome a buscar: ", buffer, sizeof(buffer)) != NULL) {
                buscar_por_nome(buffer);
            }
            break;
        case 2:
            if (ler_input_seguro("Digite o email a buscar: ", buffer, sizeof(buffer)) != NULL) {
                buscar_por_email(buffer);
            }
            break;
        case 3:
            if (ler_input_seguro("Digite o telefone a buscar: ", buffer, sizeof(buffer)) != NULL) {
                buscar_por_telefone(buffer);
            }
            break;
        case 4:
            buscar_registro();
            break;
        case 0:
            return;
        default:
            printf("❌ Erro: Opção inválida!\n");
    }
}

void buscar_por_nome(const char* nome) {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int encontrados = 0;
    printf("\n🔍 Buscando registros com nome contendo '%s':\n", nome);

    for (int i = 0; i < total_registros; i++) {
        char nome_lower[TAM_NOME];
        char busca_lower[TAM_NOME];
        strncpy(nome_lower, registros[i].nome, TAM_NOME-1);
        strncpy(busca_lower, nome, TAM_NOME-1);
        nome_lower[TAM_NOME-1] = '\0';
        busca_lower[TAM_NOME-1] = '\0';
        
        // Converter para minúsculas
        for(int j = 0; nome_lower[j]; j++) {
            nome_lower[j] = tolower(nome_lower[j]);
        }
        for(int j = 0; busca_lower[j]; j++) {
            busca_lower[j] = tolower(busca_lower[j]);
        }
        
        if (strstr(nome_lower, busca_lower) != NULL) {
            exibir_registro(&registros[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("❌ Nenhum registro encontrado com o nome '%s'\n", nome);
    } else {
        printf("✅ Total de registros encontrados: %d\n", encontrados);
    }
}

void buscar_por_email(const char* email) {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int encontrados = 0;
    printf("\n🔍 Buscando registros com email contendo '%s':\n", email);

    for (int i = 0; i < total_registros; i++) {
        char email_lower[TAM_EMAIL];
        char busca_lower[TAM_EMAIL];
        strncpy(email_lower, registros[i].email, TAM_EMAIL-1);
        strncpy(busca_lower, email, TAM_EMAIL-1);
        email_lower[TAM_EMAIL-1] = '\0';
        busca_lower[TAM_EMAIL-1] = '\0';
        
        // Converter para minúsculas
        for(int j = 0; email_lower[j]; j++) {
            email_lower[j] = tolower(email_lower[j]);
        }
        for(int j = 0; busca_lower[j]; j++) {
            busca_lower[j] = tolower(busca_lower[j]);
        }
        
        if (strstr(email_lower, busca_lower) != NULL) {
            exibir_registro(&registros[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("❌ Nenhum registro encontrado com o email '%s'\n", email);
    } else {
        printf("✅ Total de registros encontrados: %d\n", encontrados);
    }
}

void buscar_por_telefone(const char* telefone) {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int encontrados = 0;
    printf("\n🔍 Buscando registros com telefone contendo '%s':\n", telefone);

    for (int i = 0; i < total_registros; i++) {
        if (strstr(registros[i].telefone, telefone) != NULL) {
            exibir_registro(&registros[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("❌ Nenhum registro encontrado com o telefone '%s'\n", telefone);
    } else {
        printf("✅ Total de registros encontrados: %d\n", encontrados);
    }
}

void buscar_registro() {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int id;
    printf("🔍 Digite o ID do registro: ");
    if (scanf("%d", &id) != 1) {
        // Limpar buffer em caso de entrada inválida
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("❌ Erro: ID inválido! Por favor, digite um número.\n");
        return;
    }

    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    for (int i = 0; i < total_registros; i++) {
        if (registros[i].id == id) {
            printf("\n✅ REGISTRO ENCONTRADO!\n");
            exibir_registro(&registros[i]);
            return;
        }
    }
    printf("❌ Erro: Registro com ID %d não encontrado!\n", id);
}

void atualizar_registro() {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int id;
    char buffer[BUFFER_SIZE];
    StatusCode status;
    printf("Digite o ID do registro: ");
    scanf("%d", &id);
    
    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    for (int i = 0; i < total_registros; i++) {
        if (registros[i].id == id) {
            printf("\n📝 Atualizando registro ID: %d\n", id);
            
            // Atualizar nome
            sprintf(buffer, "Novo Nome [%s]: ", registros[i].nome);
            if (ler_input_seguro(buffer, buffer, sizeof(buffer)) != NULL) {
                strncpy(registros[i].nome, buffer, sizeof(registros[i].nome) - 1);
                registros[i].nome[sizeof(registros[i].nome) - 1] = '\0';
            }
            
            // Atualizar email com validação
            do {
                sprintf(buffer, "Novo Email [%s]: ", registros[i].email);
                if (ler_input_seguro(buffer, buffer, sizeof(buffer)) == NULL) {
                    return;
                }
                if (strlen(buffer) > 0) {  // Se forneceu um novo email
                    status = validar_email(buffer);
                    if (status != SUCESSO) {
                        printf("❌ Erro: Email inválido! Formato esperado: usuario@dominio.com\n");
                        continue;
                    }
                    strncpy(registros[i].email, buffer, sizeof(registros[i].email) - 1);
                    registros[i].email[sizeof(registros[i].email) - 1] = '\0';
                }
                break;
            } while (1);
            
            // Atualizar telefone com validação
            do {
                sprintf(buffer, "Novo Telefone [%s]: ", registros[i].telefone);
                if (ler_input_seguro(buffer, buffer, sizeof(buffer)) == NULL) {
                    return;
                }
                if (strlen(buffer) > 0) {  // Se forneceu um novo telefone
                    status = validar_telefone(buffer);
                    if (status != SUCESSO) {
                        printf("❌ Erro: Telefone inválido! Formato esperado: XXXXXXXXXX ou (XX)XXXXXXXX\n");
                        continue;
                    }
                    strncpy(registros[i].telefone, buffer, sizeof(registros[i].telefone) - 1);
                    registros[i].telefone[sizeof(registros[i].telefone) - 1] = '\0';
                }
                break;
            } while (1);
            
            registros[i].data_modificacao = time(NULL);
            printf("✅ Registro atualizado com sucesso!\n");
            return;
        }
    }
    printf("❌ Erro: Registro não encontrado!\n");
}

void deletar_registro() {
    if (total_registros == 0) {
        printf("❌ Erro: Nenhum registro cadastrado!\n");
        return;
    }

    int id;
    printf("🔍 Digite o ID do registro: ");
    scanf("%d", &id);
    
    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    for (int i = 0; i < total_registros; i++) {
        if (registros[i].id == id) {
            char buffer[BUFFER_SIZE];
            char confirmacao = '\0';
            
            printf("\n⚠️ ATENÇÃO: Esta operação não pode ser desfeita!\n");
            if (ler_input_seguro("Tem certeza que deseja deletar o registro? (s/n): ", buffer, sizeof(buffer)) != NULL) {
                confirmacao = buffer[0];
            }
            
            if (confirmacao == 's' || confirmacao == 'S') {
                // Move os registros posteriores uma posição para frente
                for (int j = i; j < total_registros - 1; j++) {
                    registros[j] = registros[j + 1];
                }
                total_registros--;
                printf("✅ Registro deletado com sucesso!\n");
            } else {
                printf("🔄 Operação cancelada pelo usuário.\n");
            }
            return;
        }
    }
    printf("❌ Erro: Registro não encontrado!\n");
}
void salvar_arquivo() {
    FILE *arquivo = fopen("database.txt", "w");
    if (!arquivo) {
        printf("❌ Erro: Não foi possível abrir o arquivo para escrita!\n");
        return;
    }

    for (int i = 0; i < total_registros; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%ld|%ld\n", 
                registros[i].id,
                registros[i].nome,
                registros[i].email,
                registros[i].telefone,
                registros[i].data_criacao,
                registros[i].data_modificacao);
    }

    fclose(arquivo);
    printf("✅ Dados salvos com sucesso! Total: %d registros\n", total_registros);
}

void exportar_csv() {
    char nome_arquivo[BUFFER_SIZE];
    time_t agora = time(NULL);
    struct tm *timeinfo = localtime(&agora);
    
    // Criar nome do arquivo com timestamp
    strftime(nome_arquivo, sizeof(nome_arquivo), "exportacao_%Y%m%d_%H%M%S.csv", timeinfo);
    
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("❌ Erro: Não foi possível criar o arquivo CSV!\n");
        return;
    }

    // Escrever cabeçalho
    fprintf(arquivo, "ID,Nome,Email,Telefone,Data Criação,Última Modificação\n");

    // Escrever registros
    for (int i = 0; i < total_registros; i++) {
        char data_criacao[20], data_modificacao[20];
        formatar_data(registros[i].data_criacao, data_criacao);
        formatar_data(registros[i].data_modificacao, data_modificacao);

        fprintf(arquivo, "%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                registros[i].id,
                registros[i].nome,
                registros[i].email,
                registros[i].telefone,
                data_criacao,
                data_modificacao);
    }

    fclose(arquivo);
    printf("✅ Dados exportados com sucesso para '%s'!\n", nome_arquivo);
}

void criar_backup() {
    char nome_backup[BUFFER_SIZE];
    time_t agora = time(NULL);
    struct tm *timeinfo = localtime(&agora);
    
    // Criar diretório de backup se não existir
    struct stat st = {0};
    if (stat("backups", &st) == -1) {
        #ifdef _WIN32
            mkdir("backups");
        #else
            mkdir("backups", 0700);
        #endif
    }
    
    // Criar nome do arquivo de backup com timestamp
    strftime(nome_backup, sizeof(nome_backup), "backups/backup_%Y%m%d_%H%M%S.txt", timeinfo);
    
    FILE *arquivo = fopen(nome_backup, "w");
    if (!arquivo) {
        printf("❌ Erro: Não foi possível criar o arquivo de backup!\n");
        return;
    }

    // Salvar registros no backup
    for (int i = 0; i < total_registros; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%ld|%ld\n", 
                registros[i].id,
                registros[i].nome,
                registros[i].email,
                registros[i].telefone,
                registros[i].data_criacao,
                registros[i].data_modificacao);
    }

    fclose(arquivo);
    printf("✅ Backup criado com sucesso em '%s'!\n", nome_backup);
}

void restaurar_backup() {
    char nome_backup[BUFFER_SIZE];
    printf("\n📂 === RESTAURAR BACKUP ===\n");
    
    // Listar backups disponíveis
    printf("Backups disponíveis:\n");
    #ifdef _WIN32
        system("dir /B backups\\*.txt");
    #else
        system("ls -1 backups/*.txt 2>/dev/null");
    #endif
    
    if (ler_input_seguro("\nDigite o nome do arquivo de backup: ", nome_backup, sizeof(nome_backup)) == NULL) {
        return;
    }

    // Adicionar prefixo do diretório se necessário
    char caminho_completo[BUFFER_SIZE];
    if (strncmp(nome_backup, "backups/", 8) != 0) {
        snprintf(caminho_completo, sizeof(caminho_completo), "backups/%.*s", 
                (int)(sizeof(caminho_completo) - 9), nome_backup); // -9 para "backups/" e '\0'
    } else {
        strncpy(caminho_completo, nome_backup, sizeof(caminho_completo) - 1);
        caminho_completo[sizeof(caminho_completo) - 1] = '\0';
    }

    FILE *arquivo = fopen(caminho_completo, "r");
    if (!arquivo) {
        printf("❌ Erro: Arquivo de backup não encontrado!\n");
        return;
    }

    // Criar backup do estado atual antes de restaurar
    criar_backup();

    // Limpar registros atuais
    total_registros = 0;

    // Carregar registros do backup
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), arquivo) && total_registros < MAX_REGISTROS) {
        Registro reg;
        if (sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%ld|%ld\n",
                  &reg.id,
                  reg.nome,
                  reg.email,
                  reg.telefone,
                  &reg.data_criacao,
                  &reg.data_modificacao) == 6) {
            registros[total_registros++] = reg;
        }
    }

    fclose(arquivo);
    printf("✅ Backup restaurado com sucesso! Total: %d registros\n", total_registros);
}

void formatar_data(time_t timestamp, char* buffer) {
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, 20, "%d/%m/%Y %H:%M", timeinfo);
}

void exibir_registro(const Registro* reg) {
    char data_criacao[20], data_modificacao[20];
    formatar_data(reg->data_criacao, data_criacao);
    formatar_data(reg->data_modificacao, data_modificacao);
    
    printf("\n📋 === REGISTRO ID: %d ===\n", reg->id);
    printf("👤 Nome: %-30s\n", reg->nome);
    printf("📧 Email: %-30s\n", reg->email);
    printf("📱 Telefone: %-15s\n", reg->telefone);
    printf("📅 Criado em: %s\n", data_criacao);
    printf("🔄 Última modificação: %s\n", data_modificacao);
}

void carregar_arquivo() {
    FILE *arquivo = fopen("database.txt", "r");
    if (!arquivo) {
        printf("ℹ️ Arquivo de dados não encontrado. Criando novo banco de dados...\n");
        return;
    }

    total_registros = 0;
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), arquivo) && total_registros < MAX_REGISTROS) {
        Registro reg;
        if (sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%ld|%ld\n",
                  &reg.id,
                  reg.nome,
                  reg.email,
                  reg.telefone,
                  &reg.data_criacao,
                  &reg.data_modificacao) == 6) {
            registros[total_registros++] = reg;
        }
    }

    fclose(arquivo);
    if (total_registros > 0) {
        printf("✅ Dados carregados com sucesso! Total: %d registros\n", total_registros);
    } else {
        printf("ℹ️ Nenhum registro encontrado no arquivo.\n");
    }
}
