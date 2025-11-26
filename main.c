#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileManipulators.h"

int main(){
    int option = 0;
    while(option == 0){
        printf("\nPor favor, digite um dos numeros abaixo: \n");
        printf("Digite 1 para adicionar um aluno. \n");
        printf("Digite 2 para deletar um aluno. \n");
        printf("Digite 3 para buscar um aluno. \n");
        printf("Digite 4 para listar todos os alunos. \n");
        printf("Digite 5 alterar um aluno. \n");
        printf("Digite 6 para sair do programa\n");
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
                alterar_aluno();
                option = 0;
                break;
            case 6:
                return 0;
            default:
                printf("POR FAVOR, DIGITE UM NUMERO VALIDO!!!!\n");
                option = 0;
        }
    }

}