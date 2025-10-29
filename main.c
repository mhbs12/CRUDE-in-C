#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

struct Aluno{
    char nome[50];
    int matricula;
    float nota;
};

int encontrar_aluno(int busca, int atual){
    if (busca == atual){
        return true;
    }else{
        return false;
    }
}

void novo_aluno(){
    struct Aluno aluno;
    FILE *data;
    printf("\nDigite o nome do aluno: "); scanf("%s", &aluno.nome);
    printf("\nDigite o numero de matricula: "); scanf("%d", &aluno.matricula);
    printf("\nDigite a nota: "); scanf("%f", &aluno.nota);
    data = fopen("/home/mhbs/Documentos/ProjetoC/data/alunos.dat", "awb");
    if (!data) {
        perror("Erro ao adicionar aluno");
        return;
    }else{
        fwrite(&aluno, sizeof(struct Aluno), 1, data);
        fclose(data);
        printf("Aluno adicionado com sucesso!!! \n");
    }
}

int deletar_aluno() {
    int buscarMatricula = 0;
    char confirmacao[4];
    struct Aluno aluno;
    
    char serDeletadoNome[50];
    int serDeletadoMatricula = -1;
    bool alunoEncontrado = false;
    const char *original = "/home/mhbs/Documentos/ProjetoC/data/alunos.dat";
    const char *temporario = "/home/mhbs/Documentos/ProjetoC/data/temp.dat";
    FILE *data = fopen(original, "rb");
    if (!data) {
        perror("Erro ao abrir o arquivo de alunos");
        return 0;
    }

    FILE *temp = fopen(temporario, "wb");
    if (!temp) {
        perror("Erro ao criar arquivo temporário");
        fclose(data);
        return 0;
    }
    printf("Digite o número de matrícula do aluno que você quer deletar ou tecle enter para voltar ao meu inicial: ");
    scanf("%d", &buscarMatricula);
    if (strcmp(buscarMatricula, "") == 0){
        return 0;
    }
    while (fread(&aluno, sizeof(struct Aluno), 1, data)) {
        if (encontrar_aluno(buscarMatricula, aluno.matricula) == false) {
            fwrite(&aluno, sizeof(struct Aluno), 1, temp);
        } else {
            strcpy(serDeletadoNome, aluno.nome);
            serDeletadoMatricula = aluno.matricula;
            alunoEncontrado = true;
        }
    }
    fclose(data);
    fclose(temp);
    if (!alunoEncontrado) {
        printf("Aluno com matrícula %d não encontrado.\n", buscarMatricula);
        remove(temporario);
        return 2;
    }
    printf("\nAluno encontrado: %s, Matrícula: %d\n", serDeletadoNome, serDeletadoMatricula);
    printf("Você tem certeza que deseja excluí-lo? (Digite 'Sim' para confirmar): ");
    scanf("%3s", confirmacao);
    if (strcmp(confirmacao, "Sim") == 0 || strcmp(confirmacao, "sim") == 0) {
        if (remove(original) != 0) {
            perror("Erro: Não foi possível remover o arquivo original");
            remove(temporario); 
            return 0;
        }
        if (rename(temporario, original) != 0) {
            perror("Erro: Não foi possível renomear o arquivo temporário. O arquivo original foi perdido!");
            return 0;
        }
        printf("Aluno excluído com sucesso!\n");
    } else {
        printf("Exclusão cancelada pelo usuário.\n");
        remove(temporario);
        return 0; 
    }
}

/* void alterar_aluno(){
    int buscaMatricula = 0;
    char novoNome[50];
    int novaMatricula = 0;
    float novaNota = 0;

    struct Aluno aluno;
    FILE *data;
     data = fopen("/home/mhbs/Documentos/ProjetoC/data/alunos.dat", "awr");
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
    data = fopen("/home/mhbs/Documentos/ProjetoC/data/alunos.dat", "rb");
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
        printf("Por favor, digite um dos numeros abaixo: \n");
        printf("Digite 1 para adicionar um aluno. \n");
        printf("Digite 2 para deletar um aluno. \n");
        printf("Digite 3 para listar todos os alunos. \n");
        printf("Digite 4 para sair. \n");
        scanf("%d", &option);
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
                listar_alunos();
                option = 0;
                break;
            case 4:
                return 0;
            default:
                printf("POR FAVOR, DIGITE UM NUMERO VALIDO!!!!\n");
                option = 0;
        }
    }

}