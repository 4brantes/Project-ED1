#ifndef PETSHOP_H
#define PETSHOP_H

// --- ESTRUTURAS DE DADOS ---

// Estrutura de usuário
typedef struct {
    char nome[50];
    char senha[50];
    char email[80];
} Usuario;

// Estrutura de produto/serviço
typedef struct {
    int id;
    char nome[100];
    char categoria[50];
    int quantidade;
    float preco;
} Produto;


// --- VARIÁVEL GLOBAL EXTERNA ---
// A palavra-chave 'extern' diz ao compilador que esta variável existe,
// mas está definida em outro arquivo (.c).
extern char usuario_logado[50];


// --- PROTÓTIPOS DAS FUNÇÕES ---
// Funções de utilidade
void limpar_tela();
void limpar_buffer();

// Funções de usuário
int validar_email(char email_novo[80]);
void cadastrar_usuario();
int fazer_login();

// Funções de produto
void cadastrar_produto();
void listar_produtos();
void editar_produto();
void excluir_produto();
void listar_por_categoria();
void comprar_produto();

// Funções de menu
void menu_principal();

#endif // PETSHOP_H