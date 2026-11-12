#include <stdio.h> // padrão
#include <stdlib.h> // alocar informações na memória
#include <string.h> // leitura de strings
#include <ctype.h> // alterar strings (tolower)

#define ARQUIVO_DADOS "loja.txt"
#define ARQUIVO_TEMP "temp.txt"

typedef struct{
    int id;
    char nome [50];
    char categoria [30];
    float preco;
    int quantidade;
    char fornecedor [50];
} Produto; // Todas as categorias de nosso produto estão aqui

void removerNovaLinha(char *str) { // Remove sujeira do usuário ao mandar produto e clicar enter
    int tamanho = strlen(str);
    if (tamanho > 0 && str[tamanho - 1] == '\n') {
        str[tamanho - 1] = '\0';
    }
}

int compararStrings(const char *str1, const char *str2) { // Faz o C ignorar maiúsculo e minúsculo
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return 1;
        }
        str1++;
        str2++;
    }
    return (tolower((unsigned char)*str1) == tolower((unsigned char)*str2)) ? 0 : 1;
}

int verificarDuplicata(const char *nome) { // Verifica se há uma duplicata na parte de nome do produto
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r"); 
    if (arquivo == NULL) return 0; 

    Produto p;
    // Lê o arquivo separando os dados por ponto e vírgula (;)
    while (fscanf(arquivo, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {
        if (compararStrings(p.nome, nome) == 0) {
            fclose(arquivo);
            return 1; 
        }
    }
    fclose(arquivo);
    return 0; 
}

int gerarNovoID() { // Procura o maior ID guardado, para dar o ID + 1 para um novo produto cadastrado
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) return 1; 

    int ultimoID = 0;
    Produto p;
    
    while (fscanf(arquivo, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {
        if (p.id > ultimoID) {
            ultimoID = p.id;
        }
    }
    
    fclose(arquivo);
    return ultimoID + 1; 
}

void inserirProduto() { // Cadastra o produto no txt
    Produto p;

    printf("\n--- CADASTRO DE NOVO PRODUTO ---\n");
    getchar();

    printf("Nome do Produto: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    removerNovaLinha(p.nome);

    if (verificarDuplicata(p.nome)) {
        printf("ERRO: O Produto '%s' já está cadastrado\n");
        return;
    }

    printf("Categoria: ");
    fgets(p.categoria, sizeof(p.categoria), stdin);
    removerNovaLinha(p.categoria);

    printf("Preço (R$): ");
    scanf("%f", &p.preco);

    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    getchar ();
    printf("Fornecedor: ");
    fgets(p.fornecedor, sizeof(p.fornecedor), stdin);
    removerNovaLinha(p.fornecedor);

    p.id = gerarNovoID();

    FILE *arquivo = fopen(ARQUIVO_DADOS, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    fprintf(arquivo, "%d;%s;%s;%.2f;%d;%s\n", p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
    fclose(arquivo);

    printf("Produto cadastrado com sucesso! ID gerado: %d\n", p.id);
}

void listarProdutos() { // Lista o produto para os usuários
        FILE *arquivo = fopen(ARQUIVO_DADOS, "r");

        if (arquivo == NULL) {
            printf("\nNenhum produto cadastrado ainda.\n");
            return;
        }

        Produto p;

        printf("\n------------------------------------------------------------------------------------------------\n");
        printf("| %-4s | %-25s | %-20s | %-10s | %-4s | %-15s |\n", "ID", "Produto", "Categoria", "Preco(R$)", "Qtd", "Fornecedor");
        printf("------------------------------------------------------------------------------------------------\n");

        while (fscanf(arquivo,"%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {

            printf("| %-4d | %-25.25s | %-20.20s | %-10.2f | %-4d | %-15.15s |\n",
                p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
                
    }
    printf("------------------------------------------------------------------------------------------------\n");
    fclose(arquivo);
}

void buscarProduto() { // Busca o produto por nome ou categoria
    int opcao;
    char termo [50];

    printf("\n--- BUSCAR PRODUTO ---\n");
    printf("1. Buscar por Nome\n");
    printf("2. Buscar por Categoria\n");
    printf("Escolha: ");

    scanf("%d", &opcao);
    getchar();

    printf("Digite o termo da busca: ");
    fgets(termo, sizeof(termo), stdin);
    removerNovaLinha(termo);

    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto p;
    int encontrou = 0;

    while (fscanf(arquivo, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {
        int corresponde = 0;

        if (opcao == 1 && compararStrings(p.nome, termo) == 0) {
            corresponde = 1;
        }
        else if (opcao == 2 && compararStrings(p.categoria, termo) == 0) {
            corresponde = 1;
        }

        if (corresponde == 1) {
            if (encontrou == 0) {
                printf("\nResultados encontrados:\n");
            }
            printf("ID: %d | Nome: %s | Categoria: %s | Preco: R$%.2f | Qtd: %d | Fornecedor: %s\n",
                   p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
            encontrou = 1; 
        }
    }
    
    if (encontrou == 0) {
        printf("Nenhum registro corresponde a busca.\n");
    }
    
    fclose(arquivo);
}

void excluirProduto() { // Procura o ID de um produto e o esconde do usuário, o "excluindo"
    int idBusca;
    int encontrou = 0;

    printf("\n--- EXCLUIR PRODUTO ---\n");
    printf("Digite o ID do produto que deseja excluir: ");
    scanf("%d", &idBusca);

    FILE *arquivoOri = fopen(ARQUIVO_DADOS, "r");
    FILE *arquivoTemp = fopen(ARQUIVO_TEMP, "w");

    if (arquivoOri == NULL || arquivoTemp == NULL) {
        printf ("Erro ao processar arquivos.\n");
        return;
    }

    Produto p;

    while (fscanf(arquivoOri, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {

        if (p.id != idBusca) {
            fprintf(arquivoTemp, "%d;%s;%s;%.2f;%d;%s\n", p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
        } else {
            encontrou = 1;
        }
        }

    fclose(arquivoOri);
    fclose(arquivoTemp);

    remove (ARQUIVO_DADOS);
    rename(ARQUIVO_TEMP, ARQUIVO_DADOS);

    if (encontrou == 1){
        printf("Produto excluido com sucesso!\n");
    } else{
        printf("Erro: ID nao encontrado.\n");
    }
}

void alterarProduto() { // Altera os produtos
    int idBusca;
    int encontrou = 0;

    printf("\n--- ALTERAR PRODUTO ---\n");
    printf("Digite o ID do produto que deseja alterar: ");
    scanf("%d", &idBusca);

    FILE *arquivoOri = fopen(ARQUIVO_DADOS, "r");
    FILE *arquivoTemp = fopen(ARQUIVO_TEMP, "w");

    if (arquivoOri == NULL || arquivoTemp == NULL) {
        printf ("Erro ao processar arquivos.\n");
        return;
    }

    Produto p;
    while (fscanf(arquivoOri, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {    
        if (p.id == idBusca) {
            encontrou = 1;
            int opcaoAlterar;

            do { //Da opções para o usuário escolher o que ele quer alterar
                printf("\n--- DADOS ATUAIS ---\n");
                printf("1. Nome: %s\n", p.nome);
                printf("2. Categoria: %s\n", p.categoria);
                printf("3. Preco: R$%.2f\n", p.preco);
                printf("4. Quantidade: %d\n", p.quantidade);
                printf("5. Fornecedor: %s\n", p.fornecedor);
                printf("0. Salvar alteracoes e sair\n");
                printf("O que voce deseja alterar? Escolha uma opcao: ");
                
                scanf("%d", &opcaoAlterar);
                getchar();
                
                switch (opcaoAlterar) {
                    case 1:
                        printf("Digite o NOVO Nome: ");
                        fgets(p.nome, sizeof(p.nome), stdin);
                        removerNovaLinha(p.nome);
                        break;
                    case 2:
                        printf("Digite a NOVA Categoria: ");
                        fgets(p.categoria, sizeof(p.categoria), stdin);
                        removerNovaLinha(p.categoria);
                        break;
                    case 3:
                        printf("Digite o NOVO Preco (R$): ");
                        scanf("%f", &p.preco);
                        break;
                    case 4:
                        printf("Digite a NOVA Quantidade: ");
                        scanf("%d", &p.quantidade);
                        break;
                    case 5:
                        printf("Digite o NOVO Fornecedor: ");
                        fgets(p.fornecedor, sizeof(p.fornecedor), stdin);
                        removerNovaLinha(p.fornecedor);
                        break;
                    case 0:
                        printf("Alteracoes registradas. Salvando...\n");
                        break;
                    default:
                        printf("Opcao invalida!\n");
                }
            } while (opcaoAlterar != 0);
        }
        fprintf(arquivoTemp, "%d;%s;%s;%.2f;%d;%s\n", p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
    }
    
    fclose(arquivoOri);
    fclose(arquivoTemp);
    
    remove(ARQUIVO_DADOS); 
    rename(ARQUIVO_TEMP, ARQUIVO_DADOS); 

    if (encontrou == 1) {
        printf("Produto alterado com sucesso!\n");
    } else {
        printf("Erro: ID nao encontrado.\n");
    }
}

void bubblesort(Produto *lista, int total, int tipo) { //Coloca a lista de produtos na RAM, usa algoritmo bubble sort para organizar e mostrar na tela
    int i, j;
    Produto temp;

    for (i = 0; i < total - 1; i++) {
        for (j=0; j < total - i - 1; j++) {
            int trocar = 0;

            if (tipo ==1) { // Regra 1: Por Nome (A-Z)
                #ifdef _WIN32
                if (_stricmp(lista[j].nome, lista[j+1].nome) > 0) trocar = 1;
                #else
                if (strcasecmp(lista[j].nome, lista[j+1].nome) > 0) trocar = 1;
                #endif
            }
            else if (tipo ==2) { // Regra 2: Por Nome (Z-A)
                #ifdef _WIN32
                if (_stricmp(lista[j].nome, lista[j+1].nome) < 0) trocar = 1;
                #else
                if (strcasecmp(lista[j].nome, lista[j+1].nome) < 0) trocar = 1;
                #endif
            }
            else if (tipo == 3) { // Regra 3: Por Preco (Menor para Maior)
                if (lista[j].preco > lista[j+1].preco) trocar = 1;
            }
            else if (tipo == 4) { // Regra 4: Por Preco (Maior para Menor)
                if (lista[j].preco < lista[j+1].preco) trocar = 1;
            }
            if (trocar == 1) { // Se a regra disse que estão fora de ordem, fazemos a troca!
                temp = lista[j];          // 1. Guarda o da esquerda no temp
                lista[j] = lista[j+1];    // 2. Puxa o da direita para a esquerda
                lista[j+1] = temp;        // 3. Coloca o temp na direita
        }
        }
    }
}

void exibirListaOrdenada() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    int totalRegistros = 0; // Conta produtos no arquivo
    Produto p;
    while (fscanf(arquivo, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {
        totalRegistros++; // Soma +1 para cada linha encontrada
    }

    if (totalRegistros == 0) {
        printf("Nenhum produto cadastrado.\n");
        fclose(arquivo);
        return;
    }

    Produto *lista = (Produto *)malloc(totalRegistros * sizeof(Produto)); // Reserva espaço na memória

    rewind(arquivo); // manda o ponteiro para o início do arquivo para o próximo while ler

    int i = 0; // Lê os dados e os guarda na memória
    while (fscanf(arquivo, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &lista[i].id, lista[i].nome, lista[i].categoria, &lista[i].preco, &lista[i].quantidade, lista[i].fornecedor) != EOF) {
        i++;
    }
    fclose(arquivo); // Fecha o arquivo

    int tipoOrd;
    printf("\n--- ORDENAR LISTA ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Nome (Z-A)\n");
    printf("3. Por Preco (menor -> maior)\n");
    printf("4. Por Preco (maior -> menor)\n");
    printf("Escolha: ");
    scanf("%d", &tipoOrd);

    if (tipoOrd >= 1 && tipoOrd <= 4) { // Chama o bubblesort
        bubblesort(lista, totalRegistros, tipoOrd);

        printf("\n------------------------------------------------------------------------------------------------\n");
        printf("| %-4s | %-25s | %-20s | %-10s | %-4s | %-15s |\n", "ID", "Produto", "Categoria", "Preco(R$)", "Qtd", "Fornecedor");
        printf("------------------------------------------------------------------------------------------------\n");
        for (i = 0; i < totalRegistros; i++) {
            printf("| %-4d | %-25.25s | %-20.20s | %-10.2f | %-4d | %-15.15s |\n", 
                   lista[i].id, lista[i].nome, lista[i].categoria, lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
        }
        printf("------------------------------------------------------------------------------------------------\n");
    } else {
        printf("Opcao invalida!\n");
    }

    free (lista); // Devolve memória ao computador
}

void exportarParaCSV () { // Transforma o arquivo para ser lido em formato excel
    FILE *arquivoTxt = fopen(ARQUIVO_DADOS, "r");
    FILE *arquivoCsv = fopen("loja.csv", "w"); // Cria o arquivo loja.csv

    if (arquivoTxt == NULL || arquivoCsv == NULL) {
        printf("Erro ao processar arquivos.\n");
        return;
    }

    fprintf(arquivoCsv, "Id;Produto;Categoria;Preco;Quantidade;Fornecedor\n"); // Imprime o topo da tabela para o formato csv

    Produto p;
    while (fscanf(arquivoTxt, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]\n", &p.id, p.nome, p.categoria, &p.preco, &p.quantidade, p.fornecedor) != EOF) {
        fprintf(arquivoCsv, "%d;%s;%s;%.2f;%d;%s\n", p.id, p.nome, p.categoria, p.preco, p.quantidade, p.fornecedor);
    }

    fclose(arquivoTxt); 
    fclose(arquivoCsv);
    
    printf("Sucesso! O arquivo 'loja.csv' foi gerado e ja pode ser aberto no Excel.\n");
}

int main() {
    int opcao;

    do { // Lojinha de opções do Assis
        printf("\n============\n");
        printf("Loja do Assis");
        printf("\n============\n");
        printf("1. Inserir produto\n");
        printf("2. Listar produtos\n");
        printf("3. Ordenar produtos\n");
        printf("4. Buscar produtos\n");
        printf("5. Alterar produtos\n");
        printf("6. Excluir produtos\n");
        printf("7. Exportar para Excel\n");
        printf("0. Sair");
        printf("\n============\n");
        printf("Escolha uma opção: ");

        scanf("%d", &opcao);

        switch (opcao) { // Verifica imput do usuário, leva para diferentes procedimentos de acordo com cada um
            case 1:
                inserirProduto ();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                exibirListaOrdenada();
                break;
            case 4:
                buscarProduto();
                break;
            case 5:
                alterarProduto();
                break;
            case 6:
                excluirProduto();
                break;
            case 7:
                exportarParaCSV();
                break;
            case 0:
                printf("Desligando...\n");
                break;
            default:
                printf("Opção inválida! Digite novamente.\n");
        }
    } while (opcao != 0); // Enquanto imput for diferente de 0, essa operação continuará sendo repetida

    return 0;
}