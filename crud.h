#ifndef CRUD_H
#define CRUD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_REGISTROS 100
#define TAM_NOME 50
#define TAM_EMAIL 50
#define TAM_TELEFONE 15
#define MAX_TENTATIVAS 3
#define FORMATO_DATA "%d/%m/%Y %H:%M:%S"

// Códigos de erro
typedef enum {
    SUCESSO,
    ERRO_VALIDACAO,
    ERRO_ARQUIVO,
    ERRO_MEMORIA,
    ERRO_EMAIL_INVALIDO,
    ERRO_TELEFONE_INVALIDO,
    ERRO_MAX_TENTATIVAS,
    ERRO_ENTRADA_MUITO_LONGA
} StatusCode;

typedef struct {
    int id;
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char telefone[TAM_TELEFONE];
    time_t data_criacao;
    time_t data_modificacao;
} Registro;

// Variáveis globais
extern Registro registros[MAX_REGISTROS];
extern int total_registros;

// Constantes de busca e ordenação
#define ORDEM_CRESCENTE 1
#define ORDEM_DECRESCENTE -1

// Protótipos das funções
void inicializar_sistema();
void menu_principal();
void criar_registro();
void criar_registro_manual(const char* nome, const char* email, const char* telefone);
void listar_registros();
void buscar_registro();
void atualizar_registro();
void deletar_registro();
void salvar_arquivo();
void carregar_arquivo();

// Funções de busca avançada
void busca_avancada();
void buscar_por_nome(const char* nome);
void buscar_por_email(const char* email);
void buscar_por_telefone(const char* telefone);

// Funções de exportação e backup
void exportar_csv();
void criar_backup();
void restaurar_backup();

// Funções de ordenação
void ordenar_registros(int campo, int ordem);
void listar_registros_ordenados(int campo, int ordem);

// Funções de validação e formatação
StatusCode validar_nome(const char* nome);
StatusCode validar_email(const char* email);
StatusCode validar_telefone(const char* telefone);
void formatar_data(time_t timestamp, char* buffer);
void exibir_registro(const Registro* reg);

// Funções de paginação
void listar_registros_paginados(int pagina, int registros_por_pagina);

#endif // CRUD_H
