#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *cData = "/home/mhbs/Documentos/CRUDE-in-C/data/alunos.dat";
const char *cTemp = "/home/mhbs/Documentos/CRUDE-in-C/data/temp.dat";

struct Aluno{
    char nome[50];
    int matricula;
    float nota;
};

int encontrar_aluno(int busca, int atual){
    if (busca == atual){
        return 1;
    }else{
        return 0;
    }
}

int encontrar_duplicidade_matricula(int matricula, FILE *data){
    struct Aluno aluno;
    rewind(data);
    while(fread(&aluno, sizeof(struct Aluno), 1, data) == 1){
        if (aluno.matricula == matricula){
            printf("\nO numero de matricula %d, ja esta sendo usado para o aluno %s\n\n", aluno.matricula, aluno.nome);
            return 1;
        }
    }
    return 0;
}

int encontrar_duplicidade_nome(char nome[50], FILE *data){
    struct Aluno aluno;
    rewind(data);
    while(fread(&aluno, sizeof(struct Aluno), 1, data) == 1){
        if (!strcmp(aluno.nome, nome)){
            printf("\nJa existe um aluno cadastrado com o exato nome de %s, e de matricula %d\n\n", aluno.nome, aluno.matricula);
            return 1;
        }
    }
    return 0;
}

int novo_aluno(){
    struct Aluno aluno;
    FILE *data = fopen(cData, "a+b");
    if (!data) {
        perror("Erro ao adicionar aluno");
        return 0;
    }
    do{
        printf("\nDigite o nome do aluno: "); fgets(aluno.nome, sizeof(aluno.nome), stdin);
        aluno.nome[strcspn(aluno.nome, "\n")] = '\0';  
    }while(encontrar_duplicidade_nome(aluno.nome, data));
    do{
        printf("\nDigite o numero de matricula: "); scanf("%d", &aluno.matricula);
    }while(encontrar_duplicidade_matricula(aluno.matricula, data));
    printf("\nDigite a nota: "); scanf("%f", &aluno.nota);
    fwrite(&aluno, sizeof(struct Aluno), 1, data);
    fclose(data);
    printf("\nAluno adicionado com sucesso!!! \n\n");
}


int deletar_aluno() {
    char buscarMatriculaStr[10];
    int buscarMatricula = 0;
    char confirmacao[4];
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
    printf("Digite o número de matrícula do aluno que você quer deletar ou tecle enter para voltar ao meu inicial: ");
    fgets(buscarMatriculaStr, sizeof(buscarMatriculaStr), stdin);
    if (strcmp(buscarMatriculaStr, "\n") == 0){
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
    printf("Você tem certeza que deseja excluí-lo? (Digite 'Sim' para confirmar e 'Nao' para cancelar): ");
    scanf("%3s", confirmacao);
    if (strcmp(confirmacao, "Sim") == 0 || strcmp(confirmacao, "sim") == 0) {
        if (remove(cData) != 0) {
            perror("Erro: Não foi possível remover o arquivo original");
            remove(cTemp); 
            return 0;
        }
        if (rename(cTemp, cData) != 0) {
            perror("Erro: Não foi possível renomear o arquivo temporário. O arquivo original foi perdido!");
            return 0;
        }
        printf("---------------------------------------------------------\n");
        printf("\nAluno deletado com sucesso!\n");
        printf("---------------------------------------------------------\n");
    } else {
        printf("Exclusão cancelada pelo usuário.\n");
        remove(cTemp);
        return 2; 
    }
}

int buscar_aluno(){
    int matricula = 0;
    struct Aluno aluno;
    FILE *data = fopen(cData, "rb");
    if(!data){
        perror("Erro ao abrir arquivo\n");
        return 0;
    }
    printf("Digite a matricula do aluno: "); scanf("%d", &matricula);
    while(fread(&aluno, sizeof(struct Aluno), 1, data) == 1){
        if(aluno.matricula == matricula){
            printf("---------------------------------------------------------\n");
            printf("\nAluno encontrado!!!\n");
            printf("Nome: %s\n", aluno.nome);
            printf("Matrícula: %d\n", aluno.matricula);
            printf("Nota: %.2f\n", aluno.nota);
            printf("---------------------------------------------------------\n");
            fclose(data);
        }
    }
    fclose(data);
    return 0;
}

/* void alterar_aluno(){
    int buscaMatricula = 0;
    char novoNome[50];
    int novaMatricula = 0;
    float novaNota = 0;

    struct Aluno aluno;
    FILE *data;
     data = fopen(cData, "awr");
    if (!data) {
        perror("Erro ao adicionar aluno");
        return;
    }else{
        printf("Digite o numero da matricula do aluno que ira ser alterado: "); scanf("%d", &buscaMatricula);
        while(fread(&aluno, sizeof(struct Aluno), 1, data) == 1){
            if (aluno.matricula == buscaMatricula){
                fseek(data, -sizeof(struct Aluno), SEEK_CUR);
                printf("Digite o novo nome(Ou deixe vazio para nao alterar): "); scanf("%s", &novoNome);
                if (novoNome != ""){
                    aluno.nome = novoNome;
                }
            }
        }
        
    }
    fclose(data);
} */

void listar_alunos(){
    struct Aluno aluno; 
    FILE *data;
    data = fopen(cData, "rb");
    if (!data) {
        perror("Erro ao abrir o arquivo");
        return;
    }else{
        printf("---------------------------------------------------------\n");
        while (fread(&aluno, sizeof(struct Aluno), 1, data) == 1){
            printf("Nome: %s\n", aluno.nome);
            printf("Matrícula: %d\n", aluno.matricula);
            printf("Nota: %.2f\n", aluno.nota);
            printf("---------------------------------------------------------\n");
        }
        fclose(data);
    }

}

int main(){
    int option = 0;
    while(option == 0){
        printf("\nPor favor, digite um dos numeros abaixo: \n");
        printf("Digite 1 para adicionar um aluno. \n");
        printf("Digite 2 para deletar um aluno. \n");
        printf("Digite 3 para buscar um aluno. \n");
        printf("Digite 4 para listar todos os alunos. \n");
        printf("Digite 5 para sair. \n");
        scanf("%d", &option);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        switch(option){
            case 1:
                novo_aluno();
                option = 0;
                break;
            case 2:
                int statusDel = 0;
                do{
                    statusDel = deletar_aluno();
                }while (statusDel == 2);
                option = 0;
                break;
            case 3:
                buscar_aluno();
                option = 0;
                break;
            case 4:
                listar_alunos();
                option = 0;
                break;
            case 5:
                return 0;
            default:
                printf("POR FAVOR, DIGITE UM NUMERO VALIDO!!!!\n");
                option = 0;
        }
    }

}