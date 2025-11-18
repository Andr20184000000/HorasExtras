#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>

// Estrutura de dados unificada para Funcionário
typedef struct {
    int ID_funcionario;
    char nome[100];
    char cargo[50];
    float CargaHoraria;
    float HorasSemanais;
    int HorasExtras; // Horas já aprovadas
    int HorasExtrasPendentes; // Novas solicitações
} Funcionario;

// --- Declarações das Funções ---

// Função auxiliar para executar queries
void execute_query(MYSQL *conn, const char *query, const char *error_msg);

// Funções de Cadastro (Opção 0)
void cadastrar_encarregados(MYSQL *conn, int e);
void cadastrar_gestores(MYSQL *conn, int g);
void cadastrar_tecnicos(MYSQL *conn, int t);

//funcao de seguranca ("senha" para acessar a area dos gestores)
void senha(MYSQL *conn);
//funcao contar dias:
void dias(MYSQL *conn);
// Função para Funcionário solicitar horas (Opção 1)
void solicitar_horas_extras(MYSQL *conn);

// Função para Gestor aprovar horas (Opção 2)
void aprovar_horas_extras(MYSQL *conn);

#endif // DB_H
