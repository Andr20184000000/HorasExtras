#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "db_corrigido.h" // Inclui a nova estrutura e declarações

MYSQL *conectar_mysql(void) {
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "Erro ao inicializar MySQL: %s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_real_connect(conn,
                           "localhost",   // host
                           "user",        // usuário
                           "Abcaei12",    // senha
                           "HorasExtras", // banco
                           3306,          // porta
                           NULL,
                           0) == NULL) {
        fprintf(stderr, "Erro ao conectar ao MySQL: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    printf("Conexão com MySQL realizada com sucesso.\n\n");
    return conn;
}

int main(void) {
    MYSQL *conn = conectar_mysql();

    int opc;

    printf("Cadastrar Funcionarios: (0)\n");
    printf("Abrir programa (solicitar horas): (1)\n");
    printf("Aprovar horas extras (gestor): (2)\n");
    printf("Opcão: ");
    scanf("%d", &opc);

    if (opc == 0) {
        int e, t, g;
        char simnao[8];

        // ENCARREGADOS
        printf("Deseja cadastrar encarregados?\nUtilize 'sim' ou 'nao': ");
        scanf(" %7s", simnao);

        if (strcmp(simnao, "sim") == 0) {
            printf("Quantos encarregados deseja cadastrar? ");
            scanf("%d", &e);
            cadastrar_encarregados(conn, e);
        }

        // TÉCNICOS
        printf("Deseja cadastrar técnicos?\nUtilize 'sim' ou 'nao': ");
        scanf(" %7s", simnao);

        if (strcmp(simnao, "sim") == 0) {
            printf("Quantos técnicos deseja cadastrar? ");
            scanf("%d", &t);
            cadastrar_tecnicos(conn, t);
        }

        // GESTORES
        printf("Deseja cadastrar gestores?\nUtilize 'sim' ou 'nao': ");
        scanf(" %7s", simnao);

        if (strcmp(simnao, "sim") == 0) {
            printf("Quantos gestores deseja cadastrar? ");
            scanf("%d", &g);
            cadastrar_gestores(conn, g);
        }

    } else if (opc == 1) {
        solicitar_horas_extras(conn);

    } else if (opc == 2) {
	senha(conn);
	dias(conn);
        aprovar_horas_extras(conn);

    } else {
        printf("ERRO! Não existe função para esta entrada.\n");
    }

    mysql_close(conn);
    printf("Conexão encerrada.\n");
    return 0;
}
