#include "db_corrigido.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Para roundf

// DEFINIÇÃO DA FUNÇÃO execute_query
void execute_query(MYSQL *conn, const char *query, const char *error_msg) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s: %s\n", error_msg, mysql_error(conn));
    }
}

// --- FUNÇÕES DE CADASTRO ---

void cadastrar_funcionario(MYSQL *conn, int id, const char *nome, float ch, int dt, const char *cargo) {
    char query[512];
    float HorasSemana = ch * dt;

    if (HorasSemana > 44.0f) {
        printf("\nNotamos que a carga horária deste funcionário (%.2f h/semana) "
               "é superior ao limite de 44 horas semanais previsto na legislação.\n",
               HorasSemana);
        printf("Recomendamos que reveja as condições de trabalho deste funcionário.\n");
        printf("Este funcionário NÃO será inserido no banco de dados.\n\n");
        return;
    }
    if (ch > 10) {
        printf("\nNotamos que a carga horária diária deste funcionário (%.2f h) é superior ao limite de 10 horas por dia previsto na legislação.\n", ch);
        printf("Recomendamos que reveja as condições de trabalho deste funcionário.\n");
        printf("Este funcionário NÃO será inserido no banco de dados.\n\n");
        return;
    }

    sprintf(query,
        "INSERT INTO Funcionarios "
        "(ID_funcionario, NomeFuncionario, CargaHoraria, HorasSemanais, Cargo, HorasExtras, HorasExtrasPendentes) "
        "VALUES (%d, '%s', %.2f, %.2f, '%s', 0, 0);",
         id, nome, ch, HorasSemana, cargo);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro ao inserir %s %s: %s\n",
                cargo, nome, mysql_error(conn));
    } else {
        printf("%s(a) %s inserido(a) com sucesso no banco.\n\n", cargo, nome);
    }
}

void cadastrar_encarregados(MYSQL *conn, int e) {
    for (int i = 0; i < e; i++) {
        int id, dt;
        char nome[100];
        float ch;

        printf("=== Encarregado %d ===\n", i + 1);
        printf("Qual o nome do encarregado(a) %d? ", i + 1);
        scanf(" %99[^\n]", nome);
        printf("Qual o ID do encarregado %s? ", nome);
        scanf("%d", &id);
        printf("Qual a carga horária diária do encarregado(a) %s (em horas)? ", nome);
        scanf("%f", &ch);
        printf("Quantos dias o encarregado(a) %s trabalha na semana? ", nome);
        scanf("%d", &dt);
        printf("DEBUG INSERT SQL:\n%s\n", query);
        
        cadastrar_funcionario(conn, id, nome, ch, dt, "Encarregado");
    }
}

void cadastrar_gestores(MYSQL *conn, int g) {
    for (int i = 0; i < g; i++) {
        int id, dt;
        char nome[100];
        float ch;

        printf("=== Gestor %d ===\n", i + 1);
        printf("Qual o nome do gestor(a) %d? ", i + 1);
        scanf(" %99[^\n]", nome);
        printf("Qual o ID do gestor %s? ", nome);
        scanf("%d", &id);
        printf("Qual a carga horária diária do gestor(a) %s (em horas)? ", nome);
        scanf("%f", &ch);
        printf("Quantos dias o gestor(a) %s trabalha na semana? ", nome);
        scanf("%d", &dt);
        
        cadastrar_funcionario(conn, id, nome, ch, dt, "Gestor");
    }
}

void cadastrar_tecnicos(MYSQL *conn, int t) {
    for (int i = 0; i < t; i++) {
        int id, dt;
        char nome[100];
        float ch;

        printf("=== Técnico %d ===\n", i + 1);
        printf("Qual o nome do técnico(a) %d? ", i + 1);
        scanf(" %99[^\n]", nome);
        printf("Qual o ID do técnico %s? ", nome);
        scanf("%d", &id);
        printf("Qual a carga horária diária do técnico(a) %s (em horas)? ", nome);
        scanf("%f", &ch);
        printf("Quantos dias o técnico(a) %s trabalha na semana? ", nome);
        scanf("%d", &dt);

        cadastrar_funcionario(conn, id, nome, ch, dt, "Tecnico");
    }
}

//FUNCAO DA SENHA:
void senha(MYSQL *conn) {
int id1;
char cargo[20], sql[256];
printf("Digite seu ID:\n");
scanf("%d", &id1);
sprintf(sql,
        "SELECT Cargo "
        "FROM Funcionarios "
        "WHERE ID_funcionario = %d;",
        id1);
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "Erro ao realizar SELECT: %s\n",
        mysql_error(conn));
	}
 else {
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
 if (row == NULL) {
    printf("Nenhum registro encontrado.\n");
    mysql_free_result(res);
    return;
 strncpy(cargo, row[0], sizeof(cargo));
 cargo[sizeof(cargo) - 1] = '\0';
 if(strcmp(cargo, "Gestor") == 0) {
printf("Tudo certo! Agora Podemos Continuar.");
}
else {
printf("ERRO! ID invalido para esta função");
}
}
    }


}
// --- FUNÇÃO DE SOLICITAÇÃO DE HORAS (OPÇÃO 1) ---

void solicitar_horas_extras(MYSQL *conn) {
    int id1;
    char sql[512];
    char sn[4];
    int hora_extra;
    Funcionario f;

    printf("\nDigite seu ID: ");
    scanf("%d", &id1);

    // 1. SELECT para buscar dados do funcionário
    sprintf(sql,
        "SELECT ID_funcionario, NomeFuncionario, Cargo, "
        "CargaHoraria, HorasSemanais, HorasExtras, HorasExtrasPendentes "
        "FROM Funcionarios "
        "WHERE ID_funcionario = %d;",
        id1);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "Erro no SELECT: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Erro no mysql_store_result: %s\n", mysql_error(conn));
        return;
    }

    if (mysql_num_rows(res) == 0) {
        printf("ERRO: O ID %d não existe no banco de dados.\n", id1);
        mysql_free_result(res);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    
    // Preenche a struct Funcionario com os dados do banco
    f.ID_funcionario = atoi(row[0]);
    strncpy(f.nome, row[1], sizeof(f.nome) - 1); f.nome[sizeof(f.nome) - 1] = '\0';
    strncpy(f.cargo, row[2], sizeof(f.cargo) - 1); f.cargo[sizeof(f.cargo) - 1] = '\0';
    f.CargaHoraria  = atof(row[3]);
    f.HorasSemanais = atof(row[4]);
    f.HorasExtras   = row[5] ? atoi(row[5]) : 0;
    f.HorasExtrasPendentes = row[6] ? atoi(row[6]) : 0;

    mysql_free_result(res);
    
        // Saudações
        printf("\nBem-vindo, %s! Cargo: %s\n", f.nome, f.cargo);
        printf("Carga horária diária: %.0f h\n", (f.CargaHoraria));
        printf("Horas semanais contratadas: %.0f h\n", (f.HorasSemanais));
        printf("Horas extras já registradas: %d h\n", f.HorasExtras);
        
        if (f.HorasExtrasPendentes > 0) {
            printf("ATENÇÃO: Você já tem %d horas extras pendentes de aprovação.\n", f.HorasExtrasPendentes);
        }

        // Pergunta sobre novas horas extras
        printf("\nVocê deseja realizar hora(s) extra(s)?\nResponda com 'sim' ou 'nao': ");
        scanf("%3s", sn);

        if (strcmp(sn, "sim") == 0) {
            printf("Quantas horas extras você quer realizar? ");
            scanf("%d", &hora_extra);

            int total_semanais = f.HorasSemanais + f.HorasExtras + f.HorasExtrasPendentes + hora_extra;

            if (total_semanais > 44) {
                printf("\nEssa quantidade de horas é superior ao limite de 44 horas semanais ");
                printf("previsto na legislação trabalhista. Recomendamos que reveja o requerimento.\n");
            } else if ((f.CargaHoraria) + hora_extra > 10) {
                printf("\nEssa quantidade de horas é superior ao limite de 10 horas por dia.\n");
                printf("Recomendamos que reveja o requerimento.\n");
            } else {
                printf("\nHoras extras dentro do limite legal.\n");
                printf("Total estimado de horas semanais (contrato + extras): %d h\n",
                       total_semanais);
                printf("Sua solicitação foi enviada, um gestor irá avaliar a sua requisição.\n");

                sprintf(sql,
                    "UPDATE Funcionarios SET HorasExtrasPendentes = HorasExtrasPendentes + %d "
                    "WHERE ID_funcionario = %d;",
                    hora_extra, f.ID_funcionario);
                execute_query(conn, sql, "Erro ao registrar HorasExtrasPendentes");
}
}
}
//CONTAR DIAS DA SEMANA:

//CONTAR DIAS DA SEMANA:
void dias(MYSQL *conn) {
    char resp[4];
    char query[256];
    int contador = 0;

    printf("Você gostaria de passar mais um dia? (sim/nao): ");
    scanf("%3s", resp);

    if (strcmp(resp, "sim") != 0) {
        printf("Ok, nenhum dia será adicionado.\n");
        return;
    }

    // 1) Consulta o último dia registrado
    sprintf(query,
            "SELECT DiaSemana FROM RegistroHoras "
            "ORDER BY IDregistro DESC LIMIT 1;");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro no SELECT contagem_semana: %s\n", mysql_error(conn));
        return;
    }

    // *** Padrão seguro: só chama mysql_store_result se houver result set ***
    if (mysql_field_count(conn) == 0) {
        // Isso não deveria acontecer com um SELECT, mas evita commands out of sync
        printf("Aviso: a consulta de contagem_semana não retornou conjunto de resultados.\n");
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Erro no mysql_store_result contagem_semana: %s\n", mysql_error(conn));
        return;
    }

    int linhas = mysql_num_rows(res);

    if (linhas > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row && row[0]) {
            contador = atoi(row[0]);   // último dia
        } else {
            contador = 0;
        }
    } else {
        contador = 0; // tabela vazia → começa em 0
    }

    mysql_free_result(res);  // <-- SEMPRE liberar antes de próxima query

    // 2) Incrementa o dia da semana
    contador += 1;
    if (contador > 7) {
        contador = 1;

        const char *sql_reset =
            "UPDATE Funcionarios "
            "SET HorasExtras = 0, HorasSemanais = 0;";

        if (mysql_query(conn, sql_reset) != 0) {
            fprintf(stderr, "Erro ao zerar horas: %s\n", mysql_error(conn));
            return;
        }

        printf("HorasExtras e HorasSemanais foram zeradas com sucesso.\n");
    }

    // 3) Registra o novo dia
    sprintf(query,
            "INSERT INTO RegistroHoras (DiaSemana) VALUES (%d);",
            contador);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erro ao inserir dia: %s\n", mysql_error(conn));
    } else {
        printf("Dia %d registrado com sucesso!\n", contador);
    }
}
// --- FUNÇÃO DE APROVAÇÃO DE HORAS (OPÇÃO 2) ---

void aprovar_horas_extras(MYSQL *conn) {
    char sql[512];
    int continuar;
while (continuar == 1) {
    // 1. Seleciona o primeiro funcionário com horas pendentes
    sprintf(sql,
        "SELECT ID_funcionario, NomeFuncionario, HorasExtrasPendentes "
        "FROM Funcionarios "
        "WHERE HorasExtrasPendentes > 0 LIMIT 1;");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "Erro no SELECT de solicitações pendentes: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL || mysql_num_rows(res) == 0) {
        printf("Nenhuma solicitação de horas extras pendente para aprovação.\n");
        if (res) mysql_free_result(res);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    int id_solicitante = atoi(row[0]);
    char nome_solicitante[100];
    strncpy(nome_solicitante, row[1], sizeof(nome_solicitante) - 1); nome_solicitante[sizeof(nome_solicitante) - 1] = '\0';
    int horas_pendentes = atoi(row[2]);
    
    mysql_free_result(res);

    // 2. Apresentar a solicitação e pedir aprovação
    printf("\nSolicitação de Horas Extras Pendente:\n");
    printf("Funcionário: %s (ID: %d)\n", nome_solicitante, id_solicitante);
    printf("Horas Solicitadas: %d h\n", horas_pendentes);
    printf("Digite 1 para aprovar, ou 0 para recusar: ");
    
    int aprovacao;
    scanf("%d", &aprovacao);
    
    // 3. Atualizar a tabela Funcionarios
    if (aprovacao == 1) {
        // Aprovar: Adiciona as horas pendentes a HorasExtras e zera HorasExtrasPendentes
        sprintf(sql,
            "UPDATE Funcionarios SET HorasExtras = HorasExtras + HorasExtrasPendentes, "
            "HorasExtrasPendentes = 0 WHERE ID_funcionario = %d;",
            id_solicitante);
        
        execute_query(conn, sql, "Erro ao aprovar e atualizar HorasExtras");
        printf("Solicitação de %s aprovada. %d horas adicionadas ao total.\n", nome_solicitante, horas_pendentes);
        
    } else if (aprovacao == 0) {
        // Recusar: Zera HorasExtrasPendentes
        sprintf(sql,
            "UPDATE Funcionarios SET HorasExtrasPendentes = 0 WHERE ID_funcionario = %d;",
            id_solicitante);

        execute_query(conn, sql, "Erro ao recusar HorasExtras");
        printf("Solicitação de %s recusada. Horas pendentes zeradas.\n", nome_solicitante);
    } else {
        printf("Opção inválida. Nenhuma ação tomada.\n");
    }
        printf("\nDeseja avaliar outra solicitação? (1 = sim, 0 = nao): ");
        scanf("%d", &continuar);
}
}
