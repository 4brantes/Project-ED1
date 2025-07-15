// petshop.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "petshop.h" // Inclua o seu próprio header aqui

// Definição da variável global que foi declarada como 'extern' no .h
char usuario_logado[50];

void limpar_tela() {
    system("cls || clear");
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validar_email(char email_novo[80]) {
    if (strchr(email_novo, '@') == NULL) {
        printf("Erro: Email invalido (precisa de um '@').\n");
        return 0;
    }
    if (strlen(email_novo) > 80) {
        printf("Erro: Email muito grande (max 80 caracteres).\n");
        return 0;
    }

    FILE* arq = fopen("usuarios.txt", "r");
    if (arq != NULL) {
        Usuario u;
        while (fscanf(arq, "%s %s %s", u.nome, u.senha, u.email) != EOF) {
            if (strcmp(u.email, email_novo) == 0) {
                printf("Erro: Este email ja foi cadastrado!\n");
                fclose(arq);
                return 0;
            }
        }
        fclose(arq);
    }
    return 1;
}

void cadastrar_usuario() {
    Usuario novo;
    limpar_tela();
    printf("-- Cadastro de Novo Usuario --\n");

    printf("Digite o nome de usuario: ");
    scanf("%s", novo.nome);
    limpar_buffer();

    printf("Digite a senha: ");
    scanf("%s", novo.senha);
    limpar_buffer();

    do {
        printf("Digite o e-mail: ");
        scanf("%s", novo.email);
        limpar_buffer();
    } while (validar_email(novo.email) == 0);

    FILE* arq = fopen("usuarios.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo de usuarios!\n");
        return;
    }

    fprintf(arq, "%s %s %s\n", novo.nome, novo.senha, novo.email);
    fclose(arq);

    printf("\nUsuario cadastrado com sucesso!\nPressione Enter para continuar...");
    getchar();
}

int fazer_login() {
    char nome_login[50];
    char senha_login[50];
    Usuario u;

    limpar_tela();
    printf("-- TELA DE LOGIN --\n");

    do {
        printf("Usuario: ");
        scanf("%s", nome_login);
        limpar_buffer();

        printf("Senha: ");
        scanf("%s", senha_login);
        limpar_buffer();

        FILE* arq = fopen("usuarios.txt", "r");
        if (arq == NULL) {
            printf("\nNenhum usuario cadastrado!\nPressione Enter para continuar...");
            getchar();
            return 0;
        }

        while (fscanf(arq, "%s %s %s", u.nome, u.senha, u.email) != EOF) {
            if (strcmp(nome_login, u.nome) == 0 && strcmp(senha_login, u.senha) == 0) {
                strcpy(usuario_logado, u.nome);
                fclose(arq);
                printf("\nSeja bem-vindo(a) ao Pet Shop Abrantes, %s! 🐶🐱\nPressione Enter para continuar...", usuario_logado);
                getchar();
                return 1;
            }
        }

        fclose(arq);
        printf("\nUsuario ou senha incorretos! Tente novamente.\n");
    } while (1);
}

void cadastrar_produto() {
    Produto p;
    limpar_tela();
    printf("-- Cadastro de Produto ou Servico --\n");

    int ultimo_id = 0;
    FILE* arq_leitura = fopen("produtos.txt", "r");
    if (arq_leitura != NULL) {
        while (fscanf(arq_leitura, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
            if (p.id > ultimo_id) ultimo_id = p.id;
        }
        fclose(arq_leitura);
    }
    p.id = ultimo_id + 1;
    printf("ID do produto (gerado): %d\n", p.id);

    printf("Nome: ");
    fgets(p.nome, 100, stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;

    printf("Categoria (racao, brinquedo, banho, consulta): ");
    fgets(p.categoria, 50, stdin);
    p.categoria[strcspn(p.categoria, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);
    printf("Preco: ");
    scanf("%f", &p.preco);
    limpar_buffer();

    FILE* arq = fopen("produtos.txt", "a");
    fprintf(arq, "%d;%s;%s;%d;%.2f\n", p.id, p.nome, p.categoria, p.quantidade, p.preco);
    fclose(arq);

    printf("\nProduto cadastrado com sucesso! Pressione Enter para continuar...");
    getchar();
}

void listar_produtos() {
    Produto p;
    limpar_tela();
    printf("-- LISTA DE PRODUTOS E SERVICOS --\n\n");
    FILE* arq = fopen("produtos.txt", "r");
    if (arq == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    while (fscanf(arq, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
        printf("ID: %d | Nome: %s | Categoria: %s | Estoque: %d | Preco: R$%.2f\n", p.id, p.nome, p.categoria, p.quantidade, p.preco);
    }
    fclose(arq);
}

void editar_produto() {
    listar_produtos();
    int id;
    printf("\nDigite o ID do produto que deseja editar: ");
    scanf("%d", &id);
    limpar_buffer();

    FILE *orig, *temp;
    Produto p;
    int achou = 0;

    orig = fopen("produtos.txt", "r");
    temp = fopen("temp.txt", "w");

    while (fscanf(orig, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
        if (p.id == id) {
            achou = 1;
            printf("Novo nome: ");
            fgets(p.nome, 100, stdin);
            p.nome[strcspn(p.nome, "\n")] = 0;
            printf("Nova categoria: ");
            fgets(p.categoria, 50, stdin);
            p.categoria[strcspn(p.categoria, "\n")] = 0;
            printf("Nova quantidade: ");
            scanf("%d", &p.quantidade);
            printf("Novo preco: ");
            scanf("%f", &p.preco);
            limpar_buffer();
        }
        fprintf(temp, "%d;%s;%s;%d;%.2f\n", p.id, p.nome, p.categoria, p.quantidade, p.preco);
    }
    fclose(orig);
    fclose(temp);
    remove("produtos.txt");
    rename("temp.txt", "produtos.txt");

    if (achou) printf("\nProduto atualizado com sucesso!\n");
    else printf("\nProduto nao encontrado.\n");
    printf("Pressione Enter para continuar..."); getchar();
}

void excluir_produto() {
    listar_produtos();
    int id;
    printf("\nDigite o ID do produto que deseja excluir: ");
    scanf("%d", &id);
    limpar_buffer();

    char confirm;
    printf("Tem certeza que deseja excluir? (s/n): ");
    scanf(" %c", &confirm);
    limpar_buffer();
    if (confirm != 's' && confirm != 'S') {
        printf("Operacao cancelada.\n");
        return;
    }

    FILE *orig, *temp;
    Produto p;
    int achou = 0;

    orig = fopen("produtos.txt", "r");
    temp = fopen("temp.txt", "w");

    while (fscanf(orig, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
        if (p.id != id) {
            fprintf(temp, "%d;%s;%s;%d;%.2f\n", p.id, p.nome, p.categoria, p.quantidade, p.preco);
        } else {
            achou = 1;
        }
    }
    fclose(orig);
    fclose(temp);
    remove("produtos.txt");
    rename("temp.txt", "produtos.txt");

    if (achou) printf("\nProduto excluido com sucesso!\n");
    else printf("\nProduto nao encontrado.\n");
    printf("Pressione Enter para continuar..."); getchar();
}

void listar_por_categoria() {
    char cat[50];
    Produto p;
    printf("\nDigite a categoria para filtrar: ");
    fgets(cat, 50, stdin);
    cat[strcspn(cat, "\n")] = 0;
    FILE* arq = fopen("produtos.txt", "r");
    if (arq == NULL) {
        printf("Arquivo nao encontrado.\n"); return;
    }
    int achou = 0;
    while (fscanf(arq, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
        if (strcasecmp(p.categoria, cat) == 0) {
            printf("ID: %d | Nome: %s | Estoque: %d | Preco: R$%.2f\n", p.id, p.nome, p.quantidade, p.preco);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum produto encontrado na categoria '%s'.\n", cat);
    fclose(arq);
    printf("Pressione Enter para continuar..."); getchar();
}

void comprar_produto() {
    listar_produtos();
    int id_busca;
    printf("\nDigite o ID do produto que deseja comprar: ");
    scanf("%d", &id_busca);
    limpar_buffer();

    FILE *original, *temp;
    Produto p;
    int achou = 0;

    original = fopen("produtos.txt", "r");
    temp = fopen("temp.txt", "w");

    if (original == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    while (fscanf(original, "%d;%[^;];%[^;];%d;%f\n", &p.id, p.nome, p.categoria, &p.quantidade, &p.preco) != EOF) {
        if (p.id == id_busca) {
            achou = 1;
            if (p.quantidade > 0) {
                p.quantidade--;
                printf("\nProduto '%s' comprado com sucesso! Preço: R$%.2f\n", p.nome, p.preco);
            } else {
                printf("\nProduto '%s' está fora de estoque!\n", p.nome);
            }
        }
        fprintf(temp, "%d;%s;%s;%d;%.2f\n", p.id, p.nome, p.categoria, p.quantidade, p.preco);
    }

    fclose(original);
    fclose(temp);

    remove("produtos.txt");
    rename("temp.txt", "produtos.txt");

    if (!achou) {
        printf("\nProduto com ID %d não encontrado.\n", id_busca);
    }
}


void menu_principal() {
    int op;
    do {
        limpar_tela();
        printf("********** PET SHOP ABRANTES **********\n");
        printf("Usuario logado: %s\n", usuario_logado);
        printf("=======================================\n");
        printf("1. Cadastrar Produto ou Servico\n");
        printf("2. Listar Produtos\n");
        printf("3. Editar Produto\n");
        printf("4. Excluir Produto\n");
        printf("5. Comprar Produto ou Servico\n");
        printf("6. Listar por Categoria\n");
        printf("0. Logout\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1: cadastrar_produto(); break;
            case 2: listar_produtos(); printf("\nPressione Enter..."); getchar(); break;
            case 3: editar_produto(); break;
            case 4: excluir_produto(); break;
            case 5: comprar_produto(); break; // Faltava uma pausa aqui, adicionei no main
            case 6: listar_por_categoria(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); getchar();
        }
    } while (op != 0);
}