#include "fileManipulators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *cData = "data/alunos.dat";
const char *cTemp = "data/temp.dat";

struct Aluno {
    char nome[50];
    int matricula;
    float nota;
};

int eh_numero(char *n) {
    n[strcspn(n, "\n")] = '\0';
    if (strlen(n) == 0) return 0;
    
    for (int i = 0; n[i] != '\0'; i++) {
        if (n[i] < '0' || n[i] > '9') {
            printf("\n%s nao e um numero valido!!\n", n);
            return 0;
        }
    }
    return 1;
}

int encontrar_aluno(int busca, int atual) {
    if (busca == atual) {
        return 1;
    } else {
        return 0;
    }
}

int encontrar_duplicidade_matricula(int matricula, FILE *data) {
    struct Aluno aluno;
    rewind(data);
    while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1) {
        if (aluno.matricula == matricula) {
            printf("\n---------------------------------------------------------\n");
            printf("O numero de matricula %d ja esta sendo usado por %s\n", aluno.matricula, aluno.nome);
            printf("---------------------------------------------------------\n");
            return 1;
        }
    }
    return 0;
}

int encontrar_duplicidade_nome(char nome[50], FILE *data) {
    struct Aluno aluno;
    rewind(data);
    while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1) {
        if (!strcmp(aluno.nome, nome)) {
            printf("\n---------------------------------------------------------\n");
            printf("Ja existe um aluno cadastrado com o nome %s\n", aluno.nome);
            printf("---------------------------------------------------------\n");
            return 1;
        }
    }
    return 0;
}

int novo_aluno() {
    struct Aluno aluno;
    char buffer[20];
    FILE *data = fopen(cData, "a+b");
    if (!data) {
        perror("Erro ao adicionar aluno.");
        return 0;
    }
    do {
        printf("\nDigite o nome do aluno ou enter para voltar: ");
        fgets(aluno.nome, sizeof(aluno.nome), stdin);
        aluno.nome[strcspn(aluno.nome, "\n")] = '\0';
        if (strlen(aluno.nome) == 0) return 0;
    } while (encontrar_duplicidade_nome(aluno.nome, data));

    do {
        printf("\nDigite o numero de matricula: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (eh_numero(buffer)) {
            aluno.matricula = atoi(buffer);
            if (!encontrar_duplicidade_matricula(aluno.matricula, data)) break;
        }
    } while (1);

    printf("\nDigite a nota: ");
    fgets(buffer, sizeof(buffer), stdin);
    aluno.nota = strtof(buffer, NULL);

    fwrite(&aluno, sizeof(struct Aluno), 1, data);
    fclose(data);
    printf("\nAluno adicionado com sucesso!!! \n\n");
    return 1;
}

int deletar_aluno() {
    char buscarMatriculaStr[10];
    int buscarMatricula = 0;
    char confirmacao[8];
    struct Aluno aluno;
    char serDeletadoNome[50];
    int serDeletadoMatricula = -1;
    int alunoEncontrado = 0;

    FILE *data = fopen(cData, "rb");
    if (!data) {
        perror("Erro ao abrir o arquivo de alunos");
        return 0;
    }
    FILE *temp = fopen(cTemp, "wb");
    if (!temp) {
        perror("Erro ao criar arquivo temporário");
        fclose(data);
        return 0;
    }

    printf("Digite a matricula para deletar ou enter para voltar: ");
    fgets(buscarMatriculaStr, sizeof(buscarMatriculaStr), stdin);
    if (!eh_numero(buscarMatriculaStr)) {
        fclose(data);
        fclose(temp);
        remove(cTemp);
        return 0;
    }
    buscarMatricula = atoi(buscarMatriculaStr);

    while (fread(&aluno, sizeof(struct Aluno), 1, data)) {
        if (encontrar_aluno(buscarMatricula, aluno.matricula) == 0) {
            fwrite(&aluno, sizeof(struct Aluno), 1, temp);
        } else {
            strcpy(serDeletadoNome, aluno.nome);
            serDeletadoMatricula = aluno.matricula;
            alunoEncontrado = 1;
        }
    }
    fclose(data);
    fclose(temp);

    if (!alunoEncontrado) {
        printf("Aluno com matrícula %d não encontrado.\n", buscarMatricula);
        remove(cTemp);
        return 2;
    }

    printf("\nAluno encontrado: %s, Matrícula: %d\n", serDeletadoNome, serDeletadoMatricula);
    printf("Confirmar exclusao? (Sim/Nao): ");
    fgets(confirmacao, sizeof(confirmacao), stdin);
    confirmacao[strcspn(confirmacao, "\n")] = '\0';

    if (strcasecmp(confirmacao, "Sim") == 0) {
        remove(cData);
        rename(cTemp, cData);
        printf("\nAluno deletado com sucesso!\n");
    } else {
        printf("Exclusão cancelada.\n");
        remove(cTemp);
    }
    return 0;
}

int buscar_aluno() {
    char matricula[10];
    struct Aluno aluno;
    FILE *data = fopen(cData, "rb");
    if (!data) {
        perror("Erro ao abrir arquivo\n");
        return 0;
    }
    printf("Digite a matricula ou enter para voltar: ");
    fgets(matricula, sizeof(matricula), stdin);
    if (!eh_numero(matricula)) {
        fclose(data);
        return 0;
    }
    int intMatricula = atoi(matricula);
    while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1) {
        if (aluno.matricula == intMatricula) {
            printf("---------------------------------------------------------\n");
            printf("Nome: %s\n", aluno.nome);
            printf("Matrícula: %d\n", aluno.matricula);
            printf("Nota: %.2f\n", aluno.nota);
            printf("---------------------------------------------------------\n");
        }
    }
    fclose(data);
    return 0;
}

void alterar_aluno() {
    char buffer[50];
    int buscaMatricula;
    struct Aluno aluno;
    int encontrado = 0;
    long posicaoOriginal;

    FILE *data = fopen(cData, "rb+");
    if (!data) {
        perror("Erro ao abrir arquivo");
        return;
    }

    printf("Digite a matricula do aluno para alterar: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (!eh_numero(buffer)) {
        fclose(data);
        return;
    }
    buscaMatricula = atoi(buffer);

    while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1) {
        if (aluno.matricula == buscaMatricula) {
            encontrado = 1;
            posicaoOriginal = ftell(data) - sizeof(struct Aluno);

            int opcao;
            printf("\n1 - Nome\n2 - Matricula\n3 - Nota\n4 - Todos\nEscolha: ");
            fgets(buffer, sizeof(buffer), stdin);
            if (eh_numero(buffer)) opcao = atoi(buffer);
            else opcao = 0;

            struct Aluno novo = aluno;
            int valido = 1;
            long posicaoBackup;

            switch (opcao) {
                case 1: 
                    printf("Novo nome: ");
                    fgets(novo.nome, sizeof(novo.nome), stdin);
                    novo.nome[strcspn(novo.nome, "\n")] = '\0';
                    if (strcmp(novo.nome, aluno.nome) != 0) {
                        posicaoBackup = ftell(data);
                        if (encontrar_duplicidade_nome(novo.nome, data)) valido = 0;
                        fseek(data, posicaoBackup, SEEK_SET);
                    }
                    break;
                case 2: 
                    printf("Nova matricula: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    if (eh_numero(buffer)) {
                        novo.matricula = atoi(buffer);
                        if (novo.matricula != aluno.matricula) {
                            posicaoBackup = ftell(data);
                            if (encontrar_duplicidade_matricula(novo.matricula, data)) valido = 0;
                            fseek(data, posicaoBackup, SEEK_SET);
                        }
                    } else valido = 0;
                    break;
                case 3: 
                    printf("Nova nota: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    novo.nota = strtof(buffer, NULL);
                    break;
                case 4: 
                    printf("Novo nome: ");
                    fgets(novo.nome, sizeof(novo.nome), stdin);
                    novo.nome[strcspn(novo.nome, "\n")] = '\0';
                    
                    printf("Nova matricula: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    if (eh_numero(buffer)) novo.matricula = atoi(buffer);
                    else valido = 0;

                    printf("Nova nota: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    novo.nota = strtof(buffer, NULL);

                    posicaoBackup = ftell(data);
                    if (strcmp(novo.nome, aluno.nome) != 0 && encontrar_duplicidade_nome(novo.nome, data)) valido = 0;
                    fseek(data, posicaoBackup, SEEK_SET);

                    posicaoBackup = ftell(data);
                    if (novo.matricula != aluno.matricula && encontrar_duplicidade_matricula(novo.matricula, data)) valido = 0;
                    fseek(data, posicaoBackup, SEEK_SET);
                    break;
                default:
                    printf("Opcao invalida!\n");
                    valido = 0;
            }

            if (valido) {
                printf("Confirmar alteracao? (s/n): ");
                fgets(buffer, sizeof(buffer), stdin);
                if (buffer[0] == 's' || buffer[0] == 'S') {
                    fseek(data, posicaoOriginal, SEEK_SET);
                    fwrite(&novo, sizeof(struct Aluno), 1, data);
                    printf("Aluno atualizado com sucesso!\n");
                } else {
                    printf("Alteracao cancelada.\n");
                }
            }
            break; 
        }
    }

    if (!encontrado) printf("Aluno nao encontrado!\n");
    fclose(data);
}

void listar_alunos() {
    struct Aluno aluno;
    FILE *data = fopen(cData, "rb");
    if (!data) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    printf("---------------------------------------------------------\n");
    while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1) {
        printf("Nome: %s\n", aluno.nome);
        printf("Matrícula: %d\n", aluno.matricula);
        printf("Nota: %.2f\n", aluno.nota);
        printf("---------------------------------------------------------\n");
    }
    fclose(data);
}