// main.c

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "petshop.h" // A única inclusão que você precisa do seu projeto

int main() {
    setlocale(LC_ALL, "Portuguese");
    int op, logado = 0;

    while (1) {
        if (!logado) {
            limpar_tela();
            printf("********** PET SHOP ABRANTES **********\n");
            printf("1. Login\n2. Cadastrar Usuario\n3. Sair\nEscolha: ");
            scanf("%d", &op);
            limpar_buffer();

            switch (op) {
                case 1:
                    logado = fazer_login();
                    break;
                case 2:
                    cadastrar_usuario();
                    break;
                case 3:
                    exit(0);
                default:
                    printf("Opcao invalida!\n");
                    getchar();
            }
        } else {
            menu_principal();
            logado = 0; // Faz o logout para voltar à tela de login
        }
    }

    return 0;
}