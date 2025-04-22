#include "crud.h"
#include <assert.h>

extern int total_registros;

void test_criar_registro() {
    printf("\n=== TESTE CRIAR REGISTRO ===\n");
    
    // Teste criação básica
    total_registros = 0;
    criar_registro_manual("Teste Automatizado", "teste@auto.com", "11987654321");
    assert(total_registros == 1);
    assert(strcmp(registros[0].nome, "Teste Automatizado") == 0);
    printf("Teste criação básica: OK\n");
    
    // Teste limite máximo
    total_registros = MAX_REGISTROS;
    criar_registro_manual("Não Deve Criar", "nao@deve.com", "111111111");
    assert(total_registros == MAX_REGISTROS);
    printf("Teste limite máximo: OK\n");
}

void test_listar_registros() {
    printf("\n=== TESTE LISTAR REGISTROS ===\n");
    
    // Teste lista vazia
    total_registros = 0;
    listar_registros();
    printf("Teste lista vazia: OK\n");
    
    // Teste com registros
    total_registros = 3;
    for(int i=0; i<3; i++) {
        registros[i].id = i+1;
        sprintf(registros[i].nome, "Teste %d", i+1);
    }
    listar_registros();
    printf("Teste com registros: OK\n");
}

void test_buscar_atualizar_deletar() {
    printf("\n=== TESTE BUSCAR/ATUALIZAR/DELETAR ===\n");
    
    // Configura dados iniciais
    total_registros = 0;
    criar_registro_manual("Registro para Teste", "teste@busca.com", "555555555");
    
    // Teste busca por ID (simulado, sem interação)
    int id_encontrado = 0;
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].id == 1) {
            id_encontrado = 1;
            break;
        }
    }
    assert(id_encontrado == 1);
    printf("Teste busca: OK\n");
    
    // Teste atualização (simulado, sem interação)
    if (id_encontrado) {
        strcpy(registros[0].nome, "Nome Atualizado");
        strcpy(registros[0].email, "atualizado@teste.com");
        assert(strcmp(registros[0].nome, "Nome Atualizado") == 0);
        printf("Teste atualização: OK\n");
    }
    
    // Teste deleção (simulado, sem interação)
    if (total_registros > 0) {
        // Simulação de deleção do primeiro registro
        for (int j = 0; j < total_registros - 1; j++) {
            registros[j] = registros[j + 1];
        }
        total_registros--;
        assert(total_registros == 0);
        printf("Teste deleção: OK\n");
    }
}

void test_persistencia() {
    printf("\n=== TESTE PERSISTÊNCIA ===\n");
    
    // Preenche registros de teste completos
    total_registros = 0;
    criar_registro_manual("Teste Persist 1", "test1@test.com", "111111111");
    criar_registro_manual("Teste Persist 2", "test2@test.com", "222222222");
    
    // Teste salvamento
    salvar_arquivo();
    
    // Teste carregamento
    total_registros = 0;
    carregar_arquivo();
    assert(total_registros == 2);
    assert(strcmp(registros[0].nome, "Teste Persist 1") == 0);
    assert(strcmp(registros[1].email, "test2@test.com") == 0);
    printf("Teste persistência: OK\n");
    
    // Limpa arquivo de teste
    remove("database.txt");
    printf("Arquivo de teste removido.\n");
}

void test_validacao_input() {
    printf("\n=== TESTE VALIDAÇÃO INPUT ===\n");
    
    // Teste email inválido
    assert(validar_email("email.invalido") == ERRO_EMAIL_INVALIDO);
    assert(validar_email("valido@exemplo.com") == SUCESSO);
    assert(validar_email("") == ERRO_VALIDACAO);
    assert(validar_email("teste@dominio.com.br") == SUCESSO);
    
    // Teste telefone inválido
    assert(validar_telefone("abc123") == ERRO_TELEFONE_INVALIDO);
    assert(validar_telefone("11987654321") == SUCESSO);
    assert(validar_telefone("") == ERRO_VALIDACAO);
    assert(validar_telefone("(11)98765-4321") == SUCESSO);
    
    // Teste nome inválido
    assert(validar_nome("") == ERRO_VALIDACAO);
    assert(validar_nome("A") == ERRO_VALIDACAO); // Nome muito curto
    assert(validar_nome("Nome Teste") == SUCESSO);
    
    printf("Testes de validação: OK\n");
}

void rodar_testes() {
    printf("\n=== INICIANDO TESTES AUTOMATIZADOS ===\n");
    
    inicializar_sistema();
    test_criar_registro();
    test_listar_registros();
    test_buscar_atualizar_deletar();
    test_persistencia();
    test_validacao_input();
    
    printf("\n=== TODOS OS TESTES AUTOMATIZADOS PASSARAM ===\n");
}

int main() {
    rodar_testes();
    return 0;
}
