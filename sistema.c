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

// Função para verificar se uma string contém apenas números
int is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0; // Não é numérico
        }
        str++;
    }
    return 1; // É numérico
}

// Função para verificar se uma string contém apenas letras e espaços
int is_valid_name(const char *str) {
    while (*str) {
        if (!isalpha(*str) && !isspace(*str)) {
            return 0; // Não é uma letra ou espaço
        }
        str++;
    }
    return 1; // É válido
}

void cadastrar_aluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    Aluno novo_aluno;

    // Validar o nome do aluno
    printf("Digite o nome do aluno: ");
    while (1) {
        scanf(" %[^\n]", novo_aluno.nome);
        if (!is_valid_name(novo_aluno.nome)) {
            printf("Entrada invalida! O nome do aluno deve conter apenas letras e espaços. Tente novamente: ");
        } else {
            break;
        }
    }

    // Validar ID de matrícula
    printf("Digite o ID de matricula (somente numeros): ");
    while (1) {
        if (scanf("%d", &novo_aluno.id_matricula) != 1) {
            printf("Entrada invalida! Digite apenas numeros: ");
            while (getchar() != '\n'); // Limpa o buffer
        } else {
            break;
        }
    }

    for (int i = 0; i < MAX_MATERIAS; i++) {
        // Validar o nome da matéria
        printf("Digite o nome da materia %d: ", i + 1);
        while (1) {
            scanf(" %[^\n]", novo_aluno.materias[i]);
            if (is_numeric(novo_aluno.materias[i])) {
                printf("Entrada invalida! O nome da materia nao pode ser um numero. Tente novamente: ");
            } else {
                break;
            }
        }

        // Validar carga horária
        while (1) {
            printf("Digite a carga horaria da materia %d (numero positivo): ", i + 1);
            if (scanf("%d", &novo_aluno.carga_horaria[i]) != 1 || novo_aluno.carga_horaria[i] <= 0) {
                printf("Entrada invalida! Digite um numero positivo: ");
                while (getchar() != '\n'); // Limpa o buffer
            } else {
                break;
            }
        }

        // Validar o nome do professor
        printf("Digite o nome do professor da materia %d: ", i + 1);
        while (1) {
            scanf(" %[^\n]", novo_aluno.professores[i]);
            if (!is_valid_name(novo_aluno.professores[i])) {
                printf("Entrada invalida! O nome do professor deve conter apenas letras e espaços. Tente novamente: ");
            } else {
                break;
            }
        }

        // Validar taxa de presença
        while (1) {
            printf("Digite a taxa de presenca da materia %d (0 a 100): ", i + 1);
            if (scanf("%f", &novo_aluno.taxa_presenca[i]) != 1 || novo_aluno.taxa_presenca[i] < 0 || novo_aluno.taxa_presenca[i] > 100) {
                printf("Entrada invalida! Digite um valor entre 0 e 100: ");
                while (getchar() != '\n'); // Limpa o buffer
            } else {
                break;
            }
        }
    }

    alunos[total_alunos++] = novo_aluno;
    printf("Aluno cadastrado com sucesso!\n");

    // Salvar os dados em um arquivo específico para o aluno
    char arquivo_aluno[MAX_NOME + 20]; // Para o nome do aluno e ".txt"
    snprintf(arquivo_aluno, sizeof(arquivo_aluno), "%s.txt", novo_aluno.nome);
    FILE *file_aluno = fopen(arquivo_aluno, "w");
    if (file_aluno == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados do aluno!\n");
        return;
    }

    // Formatar e salvar os dados do aluno no arquivo específico em formato tabular
    fprintf(file_aluno, "%-20s %-10s %-15s %-10s %-15s %-15s\n", 
            "Nome", "ID Matricula", "Materia", "Carga Horaria", "Professor", "Taxa Presenca");
    
    for (int j = 0; j < MAX_MATERIAS; j++) {
        // Se for a primeira linha, imprime o nome do aluno
        if (j == 0) {
            fprintf(file_aluno, "%-20s %-10d %-15s %-10d %-15s %.2f%%\n", 
                    novo_aluno.nome, novo_aluno.id_matricula, novo_aluno.materias[j], 
                    novo_aluno.carga_horaria[j], novo_aluno.professores[j], 
                    novo_aluno.taxa_presenca[j]);
        } else {
            // Para as linhas subsequentes
            fprintf(file_aluno, "                  %-15s %-10d %-15s %.2f%%\n", 
                    novo_aluno.materias[j], novo_aluno.carga_horaria[j], 
                    novo_aluno.professores[j], novo_aluno.taxa_presenca[j]);
        }
    }

    fclose(file_aluno);

    // Atualizar o arquivo com todos os alunos
    FILE *file_todos = fopen(ARQUIVO_TODOS, "a");
    if (file_todos == NULL) {
        printf("Erro ao abrir o arquivo para salvar todos os alunos!\n");
        return;
    }

    // Formatar e salvar os dados do aluno no arquivo geral
    fprintf(file_todos, "%-20s %-10d %-15s %-10d %-15s %.2f%%\n", 
            novo_aluno.nome, novo_aluno.id_matricula, novo_aluno.materias[0], 
            novo_aluno.carga_horaria[0], novo_aluno.professores[0], 
            novo_aluno.taxa_presenca[0]);
    
    for (int j = 1; j < MAX_MATERIAS; j++) {
        fprintf(file_todos, "                  %-15s %-10d %-15s %.2f%%\n", 
                novo_aluno.materias[j], novo_aluno.carga_horaria[j], 
                novo_aluno.professores[j], novo_aluno.taxa_presenca[j]);
    }
    
    fprintf(file_todos, "\n");
    fclose(file_todos);
}

void listar_alunos() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("%-20s %-10s %-15s %-10s %-15s %-15s\n", 
           "Nome", "ID Matricula", "Materia", "Carga Horaria", "Professor", "Taxa Presenca");
    
    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        
        for (int j = 0; j < MAX_MATERIAS; j++) {
            // Se for a primeira linha, imprime o nome do aluno
            if (j == 0) {
                printf("%-20s %-10d %-15s %-10d %-15s %.2f%%\n", 
                       aluno->nome, aluno->id_matricula, aluno->materias[j], 
                       aluno->carga_horaria[j], aluno->professores[j], 
                       aluno->taxa_presenca[j]);
            } else {
                // Para as linhas subsequentes
                printf("                  %-15s %-10d %-15s %.2f%%\n", 
                       aluno->materias[j], aluno->carga_horaria[j], 
                       aluno->professores[j], aluno->taxa_presenca[j]);
            }
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
            // Editar nome
            printf("Digite o novo nome do aluno: ");
            scanf(" %[^\n]", aluno->nome);

            // Editar ID de matrícula
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

void salvar_dados() {
    FILE *file = fopen(ARQUIVO_TODOS, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar todos os alunos!\n");
        return;
    }

    fprintf(file, "%-20s %-10s %-15s %-10s %-15s %-15s\n", 
            "Nome", "ID Matricula", "Materia", "Carga Horaria", "Professor", "Taxa Presenca");
    
    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        
        for (int j = 0; j < MAX_MATERIAS; j++) {
            // Se for a primeira linha, imprime o nome do aluno
            if (j == 0) {
                fprintf(file, "%-20s %-10d %-15s %-10d %-15s %.2f%%\n", 
                        aluno->nome, aluno->id_matricula, aluno->materias[j], 
                        aluno->carga_horaria[j], aluno->professores[j], 
                        aluno->taxa_presenca[j]);
            } else {
                // Para as linhas subsequentes
                fprintf(file, "                  %-15s %-10d %-15s %.2f%%\n", 
                        aluno->materias[j], aluno->carga_horaria[j], 
                        aluno->professores[j], aluno->taxa_presenca[j]);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Dados salvos com sucesso!\n");
}

int main() {
    int opcao;

    while (1) {
        printf("1. Cadastrar Aluno\n");
        printf("2. Listar Alunos\n");
        printf("3. Editar Aluno\n");
        printf("4. Excluir Aluno\n");
        printf("5. Salvar Dados\n");
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
                salvar_dados();
                break;
            case 0:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}
