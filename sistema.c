#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ALUNOS 100
#define MAX_MATERIAS 5
#define MAX_NOME 100
#define MAX_PROFESSOR 100
#define ARQUIVO_TODOS "todos_alunos.txt"

typedef struct {
    char nome[MAX_NOME];
    int id_matricula;
    char materias[MAX_MATERIAS][MAX_NOME];
    int carga_horaria[MAX_MATERIAS];
    char professores[MAX_MATERIAS][MAX_PROFESSOR];
    float taxa_presenca[MAX_MATERIAS];
} Aluno;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

int is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0; 
        }
        str++;
    }
    return 1; 
}

int is_valid_name(const char *str) {
    while (*str) {
        if (!isalpha(*str) && !isspace(*str)) {
            return 0; 
        }
        str++;
    }
    return 1; 
}

void cadastrar_aluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    Aluno novo_aluno;

    printf("Digite o nome do aluno: ");
    while (1) {
        scanf(" %[^\n]", novo_aluno.nome);
        if (!is_valid_name(novo_aluno.nome)) {
            printf("Entrada invalida! O nome do aluno deve conter apenas letras e espaços. Tente novamente: ");
        } else {
            break;
        }
    }

    printf("Digite o ID de matricula (somente numeros): ");
    while (1) {
        if (scanf("%d", &novo_aluno.id_matricula) != 1) {
            printf("Entrada invalida! Digite apenas numeros: ");
            while (getchar() != '\n'); 
        } else {
            break;
        }
    }

    for (int i = 0; i < MAX_MATERIAS; i++) {
        printf("Digite o nome da materia %d: ", i + 1);
        while (1) {
            scanf(" %[^\n]", novo_aluno.materias[i]);
            if (is_numeric(novo_aluno.materias[i])) {
                printf("Entrada invalida! O nome da materia nao pode ser um numero. Tente novamente: ");
            } else {
                break;
            }
        }

        while (1) {
            printf("Digite a carga horaria da materia %d (numero positivo): ", i + 1);
            if (scanf("%d", &novo_aluno.carga_horaria[i]) != 1 || novo_aluno.carga_horaria[i] <= 0) {
                printf("Entrada invalida! Digite um numero positivo: ");
                while (getchar() != '\n'); // Limpa o buffer
            } else {
                break;
            }
        }

        printf("Digite o nome do professor da materia %d: ", i + 1);
        while (1) {
            scanf(" %[^\n]", novo_aluno.professores[i]);
            if (!is_valid_name(novo_aluno.professores[i])) {
                printf("Entrada invalida! O nome do professor deve conter apenas letras e espaços. Tente novamente: ");
            } else {
                break;
            }
        }

        while (1) {
            printf("Digite a taxa de presenca da materia %d (0 a 100): ", i + 1);
            if (scanf("%f", &novo_aluno.taxa_presenca[i]) != 1 || novo_aluno.taxa_presenca[i] < 0 || novo_aluno.taxa_presenca[i] > 100) {
                printf("Entrada invalida! Digite um valor entre 0 e 100: ");
                while (getchar() != '\n'); 
            } else {
                break;
            }
        }
    }

    alunos[total_alunos++] = novo_aluno;
    printf("Aluno cadastrado com sucesso!\n");

    char arquivo_aluno[MAX_NOME + 20]; 
    snprintf(arquivo_aluno, sizeof(arquivo_aluno), "%s.txt", novo_aluno.nome);
    FILE *file_aluno = fopen(arquivo_aluno, "w");
    if (file_aluno == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados do aluno!\n");
        return;
    }

    fprintf(file_aluno, "Nome: %s\n", novo_aluno.nome);
    fprintf(file_aluno, "ID de Matricula: %d\n", novo_aluno.id_matricula);
    
    for (int j = 0; j < MAX_MATERIAS; j++) {
        fprintf(file_aluno, "Materia %d: %s\n", j + 1, novo_aluno.materias[j]);
        fprintf(file_aluno, "Carga Horaria: %d\n", novo_aluno.carga_horaria[j]);
        fprintf(file_aluno, "Professor: %s\n", novo_aluno.professores[j]);
        fprintf(file_aluno, "Taxa de Presenca: %.2f%%\n\n", novo_aluno.taxa_presenca[j]);
    }

    fclose(file_aluno);

    FILE *file_todos = fopen(ARQUIVO_TODOS, "a");
    if (file_todos == NULL) {
        printf("Erro ao abrir o arquivo para salvar todos os alunos!\n");
        return;
    }

    fprintf(file_todos, "Nome: %s\n", novo_aluno.nome);
    fprintf(file_todos, "ID de Matricula: %d\n", novo_aluno.id_matricula);
    
    for (int j = 0; j < MAX_MATERIAS; j++) {
        fprintf(file_todos, "Materia %d: %s\n", j + 1, novo_aluno.materias[j]);
        fprintf(file_todos, "Carga Horaria: %d\n", novo_aluno.carga_horaria[j]);
        fprintf(file_todos, "Professor: %s\n", novo_aluno.professores[j]);
        fprintf(file_todos, "Taxa de Presenca: %.2f%%\n\n", novo_aluno.taxa_presenca[j]);
    }
    
    fclose(file_todos);
}

void listar_alunos() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        printf("Nome: %s\n", aluno->nome);
        printf("ID Matricula: %d\n", aluno->id_matricula);
        
        for (int j = 0; j < MAX_MATERIAS; j++) {
            printf("Materia %d: %s\n", j + 1, aluno->materias[j]);
            printf("Carga Horaria: %d\n", aluno->carga_horaria[j]);
            printf("Professor: %s\n", aluno->professores[j]);
            printf("Taxa Presenca: %.2f%%\n\n", aluno->taxa_presenca[j]);
        }
        printf("\n");
    }
}

void editar_aluno() {
    int id_matricula;
    printf("Digite o ID da matricula do aluno que deseja editar: ");
    scanf("%d", &id_matricula);

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id_matricula == id_matricula) {
            Aluno *aluno = &alunos[i];

            printf("Editando aluno: %s\n", aluno->nome);
            printf("Digite o novo nome do aluno: ");
            scanf(" %[^\n]", aluno->nome);
            printf("Digite o novo ID de matricula: ");
            scanf("%d", &aluno->id_matricula);

            for (int j = 0; j < MAX_MATERIAS; j++) {
                printf("Digite o novo nome da materia %d: ", j + 1);
                scanf(" %[^\n]", aluno->materias[j]);
                printf("Digite a nova carga horaria da materia %d: ", j + 1);
                scanf("%d", &aluno->carga_horaria[j]);
                printf("Digite o novo nome do professor da materia %d: ", j + 1);
                scanf(" %[^\n]", aluno->professores[j]);
                printf("Digite a nova taxa de presenca da materia %d: ", j + 1);
                scanf("%f", &aluno->taxa_presenca[j]);
            }
            printf("Aluno editado com sucesso!\n");
            return;
        }
    }

    printf("Aluno com ID de matricula %d nao encontrado!\n", id_matricula);
}

void excluir_aluno() {
    int id_matricula;
    printf("Digite o ID da matricula do aluno que deseja excluir: ");
    scanf("%d", &id_matricula);

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id_matricula == id_matricula) {
            for (int j = i; j < total_alunos - 1; j++) {
                alunos[j] = alunos[j + 1]; 
            }
            total_alunos--;
            printf("Aluno excluido com sucesso!\n");
            return;
        }
    }

    printf("Aluno com ID de matricula %d nao encontrado!\n", id_matricula);
}

void salvar_alunos_em_arquivo() {
    FILE *file = fopen(ARQUIVO_TODOS, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar alunos!\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        fprintf(file, "Nome: %s\n", aluno->nome);
        fprintf(file, "ID de Matricula: %d\n", aluno->id_matricula);
        
        for (int j = 0; j < MAX_MATERIAS; j++) {
            fprintf(file, "Materia %d: %s\n", j + 1, aluno->materias[j]);
            fprintf(file, "Carga Horaria: %d\n", aluno->carga_horaria[j]);
            fprintf(file, "Professor: %s\n", aluno->professores[j]);
            fprintf(file, "Taxa de Presenca: %.2f%%\n\n", aluno->taxa_presenca[j]);
        }
    }

    fclose(file);
    printf("Alunos salvos com sucesso!\n");
}

int main() {
    int opcao;

    do {
        printf("1. Cadastrar Aluno\n");
        printf("2. Listar Alunos\n");
        printf("3. Editar Aluno\n");
        printf("4. Excluir Aluno\n");
        printf("5. Salvar Alunos em Arquivo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_aluno();
                break;
            case 2:
                listar_alunos();
                break;
            case 3:
                editar_aluno();
                break;
            case 4:
                excluir_aluno();
                break;
            case 5:
                salvar_alunos_em_arquivo();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        printf("\n");
    } while (opcao != 0);

    return 0;
}
