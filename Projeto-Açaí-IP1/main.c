/*****************************************************************************
  Introdução a Programação 1 - Linguagem C - UFRPE                           *
                                                                             *
  Projeto: Sistema de autoatendimento na venda de Açaí - Açaíteria           *
  Aluno: Thiago Gabriel Gomes da Silva Paiva                                 *
  Professor: Sidney Nogueira                                                 *
                                                                             *
                                                                             *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

struct Compra
{
    int codigoCompra;
    int qtdCompra;
    char saborCompra[20];
    float precoCompra;
};

struct Cliente
{
    char nome[70];
    char senha[15];
    char endereco[99];
    int idade;
    char cpf[11];
    int qtdCompras;
    float saldo;
    struct Compra compras[50];
};

struct Acai
{
    int codigo;
    int qtdProduto;
    float preco;
    char sabor[20];
    char descricao[60];
};

#define TAMANHO_CPF 12
#define MAX_LOGIN 16
#define MAX_SENHA 12

int TAMANHO_VETOR;
int TAMANHO_VETOR2;

int estoque_acai;
int quantidade_clientes;

char clienteEspecifico[70];
char verificaLoginCpf[TAMANHO_CPF];
int guardarPosicaoCliente;

struct Acai *acai;
struct Cliente *clientes;

char cliente_arquivo[] = "/Users/thiago/Projeto-Açaí/clientes.bin";
char acai_arquivo[] = "/Users/thiago/Projeto-Açaí/acai.bin";

int DEBUG_MODE = 1;

void imprimeInformacoesDebug()
{
    if(DEBUG_MODE)
    {
        printf("*********************\n");
        printf("TAMANHO_VETOR = %d\n", TAMANHO_VETOR);
        printf("TAMANHO_VETOR 2 = %d\n", TAMANHO_VETOR2);
        printf("quantidade_usuarios = %d\n", quantidade_clientes);
        printf("quantidade_acai= %d\n", estoque_acai);
        printf("***********************\n\n");

    }
}

void historicoAdm()
{
    int i, clienteX;
    float precoTotal = 0;

    printf("Informe o número do cliente que você deseja visualizar as compras: \n");
    scanf("%d", &clienteX);

    printf("\n-----  Histórico de compras dos clientes  -----\n");
    for(i=0; i< clientes[clienteX - 1].qtdCompras; i++)
    {
        printf("\n--------  Compra [%d]  --------\n", i+1);
        printf("Código: %d\n",clientes[clienteX - 1].compras[i].codigoCompra);
        printf("Quantidade comprada: %d\n",clientes[clienteX - 1].compras[i].qtdCompra);
        printf("Sabor: %s\n",clientes[clienteX - 1].compras[i].saborCompra);
        printf("Valor da compra: R$%.2f\n",clientes[clienteX - 1].compras[i].precoCompra);
        precoTotal = precoTotal + clientes[clienteX - 1].compras[i].precoCompra;

    }
    if(precoTotal > 0)
    {
        printf("\nValor total de compras: R$%.2f\n\n", precoTotal);
    }
    else
    {
        printf("\nCliente ainda não possui compras ou não foi encontrado!\n\n");
    }
    printf("----------------------------------\n");

}

void historicoCliente()
{
    int i;
    float precoTotal = 0;

    printf("\n-----  Meu Histórico de Compras  -----\n");
    for(i=0; i<clientes[guardarPosicaoCliente].qtdCompras; i++)
    {
        printf("\n--------  Compra [%d]  --------\n", i+1);
        printf("Código: %d\n",clientes[guardarPosicaoCliente].compras[i].codigoCompra);
        printf("Quantidade comprada: %d\n",clientes[guardarPosicaoCliente].compras[i].qtdCompra);
        printf("Sabor: %s\n",clientes[guardarPosicaoCliente].compras[i].saborCompra);
        printf("Valor da compra: R$%.2f\n",clientes[guardarPosicaoCliente].compras[i].precoCompra);
        printf("\n");
        precoTotal = precoTotal + clientes[guardarPosicaoCliente].compras[i].precoCompra;
    }
    printf("Valor total de compras: R$%.2f\n", precoTotal);
    printf("\n----------------------------------\n");
}

void realizarCompra()
{
    int x = 0;
    int op,i,qtdAcai;

    printf("Informe o código do produto que deseja comprar: ");
    scanf("%d", &op);

    for(i=0; i<estoque_acai; i++)
    {
        if(op == acai[i].codigo && acai[i].qtdProduto <= 0)
        {
            printf("\nO Produto selecionado está em falta no estoque, veja outras opções de compra!\n");
            x=1;
            break;
        }
        else if(op == acai[i].codigo && clientes[guardarPosicaoCliente].saldo >= acai[i].preco )
        {
            printf("\nVocê selecionou o Açaí de %s.\nInforme quantos deseja comprar? ", acai[i].sabor);
            scanf("%d", &qtdAcai);

            if(acai[i].qtdProduto - qtdAcai >= 0 && clientes[guardarPosicaoCliente].saldo - (acai[i].preco * qtdAcai) >= 0 && qtdAcai > 0)
            {
                acai[i].qtdProduto = acai[i].qtdProduto - qtdAcai;
                clientes[guardarPosicaoCliente].saldo = clientes[guardarPosicaoCliente].saldo - (acai[i].preco * qtdAcai);

                printf("\nParabéns pela compra, %s!\n", clienteEspecifico);

                clientes[guardarPosicaoCliente].compras[clientes[guardarPosicaoCliente].qtdCompras].codigoCompra = acai[i].codigo;
                clientes[guardarPosicaoCliente].compras[clientes[guardarPosicaoCliente].qtdCompras].precoCompra = acai[i].preco * qtdAcai;
                clientes[guardarPosicaoCliente].compras[clientes[guardarPosicaoCliente].qtdCompras].qtdCompra = qtdAcai;
                strcpy(clientes[guardarPosicaoCliente].compras[clientes[guardarPosicaoCliente].qtdCompras].saborCompra, acai[i].sabor);

                clientes[guardarPosicaoCliente].qtdCompras++;
            }
            else
            {
                printf("\nA compra não foi aprovada! Quantidade inválida ou saldo de compra insuficiente!\n");
            }

            x=1;
            break;
        }
        else if(op == acai[i].codigo && clientes[guardarPosicaoCliente].saldo < acai[i].preco )
        {
            printf("\nAtenção! Seu saldo de compras é insuficiente para esse produto.\nPor favor, recarregue seu saldo!\n");
            x=1;
            break;
        }
    }
    if(!x)
    {
        printf("\nCódigo de produto não encontrado!\n");
    }
}

void mensagemErro(char *nome_arquivo)
{
    printf("Erro: Não foi possível abrir o arquivo %s, pois ele não existe!\nMas para o arquivo ser criado e salvo você deve sair corretamente do sistema!\n\n", nome_arquivo);

}

void mensagemErroGravar(char *nome_arquivo)
{
    printf("\nErro: Não é possível salvar no arquivo %s\nVerifique se o arquivo ou diretório realmente existe!\n", nome_arquivo);

}

void gravarArquivo()
{
    FILE *arquivo = fopen(cliente_arquivo, "wb");

    if(arquivo == NULL)
    {
        mensagemErroGravar(cliente_arquivo);
        exit(1);
    }
    else
    {
        int c = (int) fwrite(&TAMANHO_VETOR, sizeof(int), 1, arquivo);

        if(c < 1)
        {
            mensagemErroGravar(cliente_arquivo);
            fclose(arquivo);
            return;
        }

        c = (int) fwrite(&quantidade_clientes, sizeof(int), 1, arquivo);

        if(c < 1)
        {
            mensagemErroGravar(cliente_arquivo);
            fclose(arquivo);
            return;
        }

        c = (int) fwrite(clientes, sizeof(struct Cliente), TAMANHO_VETOR, arquivo);

        if(c < TAMANHO_VETOR)
        {
            mensagemErroGravar(cliente_arquivo);
            fclose(arquivo);
            return;
        }

        fclose(arquivo);
    }

    FILE *arquivo2 = fopen(acai_arquivo, "wb");

    if(arquivo2 == NULL)
    {
        mensagemErroGravar(acai_arquivo);
        exit(1);
    }
    else
    {

        int d = (int) fwrite(&TAMANHO_VETOR2, sizeof(int), 1, arquivo2);

        if(d < 1)
        {
            mensagemErroGravar(acai_arquivo);
            fclose(arquivo2);
            return;
        }

        d= (int) fwrite(&estoque_acai, sizeof(int), 1, arquivo2);

        if(d < 1)
        {
            mensagemErroGravar(acai_arquivo);
            fclose(arquivo2);
            return;
        }

        d = (int) fwrite(acai, sizeof(struct Acai), TAMANHO_VETOR2, arquivo2);

        if(d < TAMANHO_VETOR2)
        {
            mensagemErroGravar(acai_arquivo);
            fclose(arquivo2);
            return;
        }

        fclose(arquivo2);
    }
}

void lerArquivo()
{
    FILE *arquivo = fopen(cliente_arquivo, "rb");

    if(arquivo == NULL)
    {
        mensagemErro(cliente_arquivo);

        TAMANHO_VETOR = 10;
        quantidade_clientes = 0;
        clientes = malloc(TAMANHO_VETOR*sizeof(struct Cliente));
    }
    else
    {

        int c = (int)fread(&TAMANHO_VETOR, sizeof(int), 1, arquivo);

        if(c < 1)
        {
            mensagemErro(cliente_arquivo);
            fclose(arquivo);
            return;
        }

        c = (int)fread(&quantidade_clientes, sizeof(int), 1, arquivo);

        if(c < 1)
        {
            mensagemErro(cliente_arquivo);
            fclose(arquivo);
            return;
        }


        clientes = malloc(TAMANHO_VETOR*sizeof(struct Cliente));


        c = (int)fread(clientes, sizeof(struct Cliente), TAMANHO_VETOR, arquivo);

        if(c < TAMANHO_VETOR)
        {
            mensagemErro(cliente_arquivo);
            fclose(arquivo);
            return;
        }

        fclose(arquivo);
    }

    FILE *arquivo2 = fopen(acai_arquivo, "rb");

    if(arquivo2 == NULL)
    {
        mensagemErro(acai_arquivo);

        TAMANHO_VETOR2 = 10;
        estoque_acai = 0;
        acai = malloc(TAMANHO_VETOR2*sizeof(struct Acai));
    }
    else
    {

        int d = (int)fread(&TAMANHO_VETOR2, sizeof(int), 1, arquivo2);

        if(d < 1)
        {
            mensagemErro(acai_arquivo);
            fclose(arquivo2);
            return;
        }


        d = (int)fread(&estoque_acai, sizeof(int), 1, arquivo2);

        if(d < 1)
        {
            mensagemErro(acai_arquivo);
            fclose(arquivo2);
            return;
        }


        acai = malloc(TAMANHO_VETOR2*sizeof(struct Acai));


        d = (int)fread(acai, sizeof(struct Acai), TAMANHO_VETOR2, arquivo2);

        if(d < TAMANHO_VETOR2)
        {
            mensagemErro(acai_arquivo);
            fclose(arquivo2);
            return;
        }

        fclose(arquivo2);
    }

}

void imprimeClientes()
{
    int c;
    printf("---- Lista de clientes cadastrados ----\n");
    for(c=0; c < quantidade_clientes; c++)
    {
        printf("-----------------------------------\n");
        printf("-----  Cliente (%d)  -----\n", c+1);
        printf("Nome Completo = %s\n", clientes[c].nome);
        printf("CPF  = %s\n", clientes[c].cpf);
        printf("Idade = %d\n", clientes[c].idade);
        printf("Senha = %s\n", clientes[c].senha);
        printf("Endereço = %s\n", clientes[c].endereco);
        printf("Quantidade de compras = %d\n", clientes[c].qtdCompras);
        printf("Saldo do cliente = R$%.2f\n", clientes[c].saldo);

    }
    printf("---------------------------------------\n");
}

void imprimeAcai()
{
    int a;
    printf("---- Lista de Produtos cadastrados ----\n");
    for(a=0; a < estoque_acai; a++)
    {
        printf("\n-----  Açaí (%d)  -----\n", a+1);
        printf("Código  = %d\n", acai[a].codigo);
        printf("Sabor  = %s\n", acai[a].sabor);
        printf("Quantidade  = %d\n", acai[a].qtdProduto);
        printf("Preço  = R$%.2f\n", acai[a].preco);
        printf("Descrição  = %s\n", acai[a].descricao);
    }
    printf("---------------------------------------\n");
}

void removeQuebraLinha(char *string)
{
    if(string != NULL && strlen(string) > 0)
    {
        short tamanho = strlen(string);
        if(string[tamanho-1] == '\n')
        {
            string[tamanho-1] = '\0';
        }
    }
}

void leStringTeclado(char *string_destino, int quantidade_caracteres)
{


    fgets(string_destino, quantidade_caracteres, stdin);

    removeQuebraLinha(string_destino);
}

int igualdadeCPF(char verificaLoginCpf[])
{
    struct Cliente *c;
    int i;
    for(i=0; i<quantidade_clientes; i++)
    {
        if(!strcmp(verificaLoginCpf,clientes[i].cpf))
        {

            return 0;
            break;
        }
    }
    return 1;
}

int igualdadeCodigo(int code)
{
    struct Acai *a;
    int i;
    for(i=0; i<estoque_acai; i++)
    {
        if(acai[i].codigo == code)
        {

            return 0;
            break;
        }
    }

    return 1;
}

struct Cliente lerNovoSaldo()
{
    struct Cliente u;
    do
    {
        printf("Informe o saldo que deseja carregar: \n");
        scanf("%f", &u.saldo);
    }
    while(u.saldo<0);

    return u;
}

struct Cliente cadastroCliente()
{
    struct Cliente u;

    int k=0;

    printf("\nCPF(Apenas números): ");
    leStringTeclado(u.cpf, TAMANHO_CPF);
    getchar();
    while(!k)
    {
        int x = validarCPF(u.cpf);
        int z = igualdadeCPF(u.cpf);
        if(x && z)
        {

            printf("Nome Completo: ");
            leStringTeclado(u.nome, 70);
            fflush(stdin);
            printf("Idade: ");
            scanf("%d", &u.idade);
            getchar();
            if(u.idade < 7 || u.idade > 100)
            {
                printf("Idade inválida, tente novamente!\n");
                menu_adm();
            }
            printf("Senha: ");
            leStringTeclado(u.senha, MAX_SENHA);
            printf("Endereço: ");
            leStringTeclado(u.endereco, 99);
            printf("Quantidade de compras: ");
            scanf("%d", &u.qtdCompras);
            if(u.qtdCompras < 0 || u.qtdCompras > 50)
            {
                printf("Quantidade de compras inválida, tente novamente!\n");
                menu_adm();
            }
            printf("Saldo do cliente: ");
            scanf("%f", &u.saldo);
            if(u.saldo < 0)
            {
                printf("Saldo inválido, tente novamente!\n");
                menu_adm();
            }
            printf("\n---- Cliente cadastrado com sucesso! ----\n\n");
            k=1;
        }
        else if(!z)
        {
            printf("Já existe um cliente cadastrado com este CPF!\n");
            k=1;
            menu_adm();
        }
        else
        {
            printf("**** CPF inválido! ****\n*** Para se cadastrar é necessário um CPF válido! ***\n");
            k=1;
            menu_adm();

        }

    }

    return u;

}

struct Cliente atualizarCadastroCliente()
{

    struct Cliente u;

    printf("\nNome Completo: ");
    leStringTeclado(u.nome, 70);
    printf("Atualizar idade: ");
    scanf("%d", &u.idade);
    getchar();
    if(u.idade < 7 || u.idade > 100)
    {
        printf("Idade inválida, tente novamente!\n");
        menu_adm();
    }
    printf("Nova Senha: ");
    leStringTeclado(u.senha, MAX_SENHA);
    printf("Endereço: ");
    leStringTeclado(u.endereco, 99);
    printf("Novo saldo: ");
    scanf("%f", &u.saldo);
    if(u.saldo < 0)
    {
        printf("Saldo inválido, tente novamente!\n");
        menu_adm();
    }

    printf("---- Cliente atualizado com sucesso! ----\n");

    return u;
}

struct Acai atualizarCadastroAcai()
{
    struct Acai a;


    printf("\nNova quantidade de Açaí: ");
    scanf("%d", &a.qtdProduto);
    getchar();
    if(a.qtdProduto < 0)
        {
            printf("Quantidade de cadastro do produto é inválida, tente novamente!\n");
            menu_adm();
        }
    printf("Novo sabor do Açaí: ");
    leStringTeclado(a.sabor,20);
    printf("Novo preço: ");
    scanf("%f", &a.preco);
    getchar();
    if(a.preco <= 0)
    {
        printf("Preço do produto inválido, tente novamente!\n");
        menu_adm();
    }
    printf("Nova descrição: ");
    leStringTeclado(a.descricao, 60);


    printf("\n---- Açaí Atualizado com sucesso! ----\n");

    return a;
}

struct Acai cadastroAcai()
{

    struct Acai a;

    printf("\nInforme o código do Açaí: ");
    scanf("%d", &a.codigo);
    int x = igualdadeCodigo(a.codigo);
    if(x)
    {
        getchar();
        printf("Informe o sabor do Açaí: ");
        leStringTeclado(a.sabor,20);
        printf("Informe a quantidade de Açaí: ");
        scanf("%d", &a.qtdProduto);
        if(a.qtdProduto < 0)
        {
            printf("Quantidade de cadastro do produto é inválida, tente novamente!\n");
            menu_adm();
        }
        printf("Informe o preço do Açaí: ");
        scanf("%f", &a.preco);
        getchar();
        if(a.preco <= 0)
        {
            printf("Preço do produto inválido, tente novamente!\n");
            menu_adm();
        }
        printf("Descrição do Açaí: ");
        leStringTeclado(a.descricao, 60);

        printf("\n---- Açaí cadastrado com sucesso! ----\n\n");
    }
    else
    {
        printf("\nJá existe um produto cadastrado com esse código!\nPor favor, tente com um novo código!\n");
        menu_adm();
    }


    return a;
}

void insereNovoCliente(struct Cliente u)
{
    if(quantidade_clientes == TAMANHO_VETOR)
    {
        TAMANHO_VETOR *= 1.5;

        clientes = realloc(clientes, TAMANHO_VETOR*sizeof(struct Cliente));
    }

    clientes[quantidade_clientes] = u;

    quantidade_clientes++;

}

void insereNovoAcai(struct Acai a)
{
    if(estoque_acai == TAMANHO_VETOR2)
    {
        TAMANHO_VETOR2 *= 1.5;

        acai = realloc(acai, TAMANHO_VETOR2*sizeof(struct Acai));
    }
    acai[estoque_acai] = a;

    estoque_acai++;
}

struct Cliente *recuperaCliente(char cpf[13])
{
    struct Cliente *u = NULL;
    int i;
    for (i=0; i < quantidade_clientes; i++)
    {
        if(strcmp(clientes[i].cpf, cpf) == 0)
        {
            u = &clientes[i];

            break;
        }
    }
    return u;
}

struct Acai *recuperaAcai(int cod)
{
    struct Acai *ab = NULL;
    int b;
    for (b=0; b < estoque_acai; b++)
    {
        if(acai[b].codigo == cod)
        {
            ab = &acai[b];

            break;
        }
    }
    return ab;
}

struct Cliente *loginCliente(char login[], char senhaCliente[])
{
    struct Cliente *u = NULL;
    int i;
    for (i=0; i < quantidade_clientes; i++)
    {
        if(!strcmp(clientes[i].cpf, login) && !strcmp(clientes[i].senha, senhaCliente))
        {
            u = &clientes[i];

            break;
        }
    }
    return u;
}

void reorganizaVetor(int indice_elemento_removido)
{
    int j = indice_elemento_removido;
    while(j < quantidade_clientes-1)
    {
        clientes[j] = clientes[j+1];
        j++;
    }
}

int removerAcaiVetor(int codigo)
{
    int i, encontrou = 0;
    for (i=0; i < estoque_acai; i++)
    {
        if(acai[i].codigo == codigo)
        {
            reorganizaVetor(i);
            if(estoque_acai < TAMANHO_VETOR2/4)
            {

                TAMANHO_VETOR2 = TAMANHO_VETOR2*0.5;
                acai = realloc(acai, TAMANHO_VETOR2*sizeof(struct Acai));
            }
            estoque_acai--;
            encontrou = 1;

            break;
        }
    }
    return encontrou;
}

int removerClienteVetor(char *cpf_cliente)
{
    int i, encontrou = 0;
    for (i=0; i < quantidade_clientes; i++)
    {
        if(strcmp(clientes[i].cpf, cpf_cliente) == 0)
        {
            reorganizaVetor(i);
            if(quantidade_clientes < TAMANHO_VETOR/4)
            {

                TAMANHO_VETOR = TAMANHO_VETOR*0.5;
                clientes = realloc(clientes, TAMANHO_VETOR*sizeof(struct Cliente));
            }
            quantidade_clientes--;
            encontrou = 1;

            break;
        }
    }
    return encontrou;
}

void recarregarSaldo()
{
    printf("Confirme seu CPF para recarregar seu saldo: \n");

    char cpf_clienteatt[13];
    leStringTeclado(cpf_clienteatt, 13);

    struct Cliente *u = recuperaCliente(cpf_clienteatt);

    if(u != NULL)
    {
        struct Cliente u2 = lerNovoSaldo();
        u->saldo = u->saldo + u2.saldo;
        printf("Seu saldo foi cadastrado com sucesso!\nSeu novo saldo é R$%.2f\n", u->saldo);

    }
    else
    {
        printf("Cliente não encontrado!\n");
    }
}

void atualizarAcai()
{
    int codigoAcai;

    printf("Digite o código do produto que deseja atualizar os dados:\n");
    scanf("%d", &codigoAcai);

    struct Acai *a = recuperaAcai(codigoAcai);

    if(a != NULL)
    {
        printf("\nInforme os novos dados do Açaí");

        struct Acai a2 = atualizarCadastroAcai();

        strcpy(a->sabor, a2.sabor);
        a->qtdProduto = a2.qtdProduto;
        a->preco = a2.preco;
        strcpy(a->descricao, a2.descricao);
    }
    else
    {
        printf("Produto não encontrado!\n");
    }
}

void atualizarClientes()
{
    printf("Digite o CPF do cliente a ser atualizado: ");

    char cpf_clienteatt[13];
    leStringTeclado(cpf_clienteatt, 13);

    struct Cliente *u = recuperaCliente(cpf_clienteatt);

    if(u != NULL)
    {
        printf("\nInforme os novos dados do cliente");

        struct Cliente u2 = atualizarCadastroCliente();

        strcpy(u->nome, u2.nome);
        u->idade = u2.idade;
        strcpy(u->senha, u2.senha);
        strcpy(u->endereco, u2.endereco);
        u->saldo = u2.saldo;
    }
    else
    {
        printf("Cliente não encontrado!\n");
    }
}

void removerAcai()
{
    int code;
    printf("Digite o código do produto que será removido do sistema: ");
    scanf("%d", &code);

    int remover = removerAcaiVetor(code);

    if(!remover)
    {
        printf("\nProduto não encontrado!\n");
    }
    else
    {
        printf("\nProduto removido com sucesso!\n");
    }
}

void removerClientes()
{

    printf("Digite o CPF do cliente que será removido do sistema: ");

    char cpf_cliente[13];
    leStringTeclado(cpf_cliente, 13);

    int sucesso = removerClienteVetor(cpf_cliente);

    if(!sucesso)
    {
        printf("\nCliente não encontrado!\n");
    }
    else
    {
        printf("\nCliente removido com sucesso!\n\n");
    }
}

void menu_clientes()
{
    int sairMenu = 0;
    system ("color 1f");
    while(!sairMenu)
    {
        printf("\nSelecione o que deseja fazer:\n\n");
        printf("1 - Visualizar Cardápio +Açaí\n");
        printf("2 - Visualizar Dados cadastrais\n");
        printf("3 - Comprar Açaí\n");
        printf("4 - Recarregar Saldo\n");
        printf("5 - Visualizar Histórico de Compras\n");
        printf("6 - Voltar a tela de login\n");
        printf("7 - Finalizar sistema\n");
        printf("Opção = ");

        int opcao;
        scanf("%d", &opcao);
        getchar();
        printf("\n");

        switch (opcao)
        {

        case 1:
            printf("---- Cardápio +Açaí ----\n");
            imprimeAcai();
            break;
        case 2:
            meusDados(verificaLoginCpf);
            break;
        case 3:
            realizarCompra();
            break;
        case 4:
            recarregarSaldo();
            break;
        case 5:
            historicoCliente();
            break;
        case 6:
            gravarArquivo();
            system("cls");
            main();
            sairMenu =1;
            break;
        case 7:
            sairMenu = 1;
            saindo_sistema();
            break;

        default:
            printf("Opcão inválida!\n");
        }
    }
}

void menu_adm()
{
    system ("color 1f");
    int sair = 0;

    while(!sair)
    {

        printf("\nSelecione o que deseja fazer:\n\n");
        printf("1 - Cadastrar Clientes\n");
        printf("2 - Visualizar Clientes\n");
        printf("3 - Histórico de Compra dos Clientes\n");
        printf("4 - Atualizar Clientes\n");
        printf("5 - Remover Clientes\n");
        printf("6 - Cadastrar Produtos +Açaí\n");
        printf("7 - Visualizar Produtos\n");
        printf("8 - Atualizar Produtos\n");
        printf("9 - Remover Produtos\n");
        printf("10 - Voltar a tela de login\n");
        printf("11 - Finalizar sistema\n");
        printf("Opção = ");

        int opcao;
        scanf("%d", &opcao);
        getchar();
        printf("\n");

        switch (opcao)
        {

        case 1:
            printf("---- Cadastro de clientes ----\nInforme os dados: ");
            insereNovoCliente(cadastroCliente());
            imprimeInformacoesDebug();
            break;

        case 2:
            imprimeClientes();
            break;

        case 3:
            historicoAdm();
            break;

        case 4:
            atualizarClientes();
            break;

        case 5:
            removerClientes();
            imprimeInformacoesDebug();
            break;

        case 6:
            printf("---- Cadastro do produto ----");
            insereNovoAcai(cadastroAcai());
            imprimeInformacoesDebug();
            break;

        case 7:
            imprimeAcai();
            break;

        case 8:
            atualizarAcai();
            break;

        case 9:
            removerAcai();
            break;

        case 10:
            gravarArquivo();
            system("cls");
            main();
            sair =1;
            break;

        case 11:
            imprimeInformacoesDebug();
            sair = 1;
            saindo_sistema();
            break;

        default:
            printf("Opcão inválida!\n");
        }

    }

}

void autentica_adm()
{
    system ("color 9f");
    char login[MAX_LOGIN];
    char senha[MAX_SENHA];

    char login2[MAX_LOGIN]= {"admin"};
    char senha2[MAX_SENHA]= {"123"};

    char c;
    int indice = 0;
    int i, op;

    fflush(stdin);
    printf("Login: ");
    gets(login);
    fflush(stdin);
    printf("%s", "Senha: ");
    do
    {
        c = getch();

        if(isprint(c))
        {
            senha[indice++] = c;
            printf("%c",'*');
        }
        else if(c == 10 && indice)
        {
            senha[indice--] = '\0';
            printf("%s", "\b \b");
        }

    }
    while(c!=13);
    printf("\n");
    if(!strcmp(login,login2) && !strcmp(senha, senha2))
    {
        printf("\nAutenticando administrador...\n");
        Sleep(2500);
        system("cls");
        printf("\n----- Parabéns! Você foi logado no Sistema +Açaí no modo Administrador! -----\n");
        menu_adm();
    }
    else
    {
        printf("\n***** Login e/ou Senha inválida, tente novamente! *****\n\n");
        autentica_adm();
    }
}

void autenticadorCliente()
{
    autentica_cliente();
}

void autentica_cliente()
{
    system ("color 9f");
    verificaLoginCpf[TAMANHO_CPF];
    char senhaC[MAX_SENHA];

    printf("Digite seu login: ");
    leStringTeclado(verificaLoginCpf, TAMANHO_CPF);
    getchar();
    printf("Digite sua senha: ");
    leStringTeclado(senhaC, MAX_SENHA);
    fflush(stdin);

    struct Cliente *u = loginCliente(verificaLoginCpf, senhaC);

    if(u != NULL)
    {
        printf("\nAutenticando cliente +Açaí...\n");
        Sleep(2500);
        system("cls");
        printf("\n----- Parabéns! Você foi logado no sistema +Açaí -----\n");
        bemVindo(verificaLoginCpf, senhaC);
        menu_clientes();
    }
    else
    {
        printf("***** Login e/ou senha inválida, tente novamente! *****\n\n");
        autenticadorCliente();
    }

}

void bemVindo(char login[], char senhaCliente[])
{
    int i;
    for (i=0; i < quantidade_clientes; i++)
    {
        if(!strcmp(clientes[i].cpf, login) && !strcmp(clientes[i].senha, senhaCliente))
        {
            printf("\n    Olá, %s! Bem vindo ao +Açaí\n", clientes[i].nome);
            strcpy(clienteEspecifico, clientes[i].nome);
            guardarPosicaoCliente = i;
            break;
        }
    }

}

void meusDados(char login[])
{
    int i;
    for (i=0; i < quantidade_clientes; i++)
    {
        if(!strcmp(clientes[i].cpf, login))
        {
            printf("-----  Meus dados  -----\n\n");
            printf("Nome: %s\n",clientes[i].nome);
            printf("Idade: %d\n", clientes[i].idade);
            printf("CPF: %s\n",clientes[i].cpf);
            printf("Senha: %s\n",clientes[i].senha);
            printf("Endereço: %s\n",clientes[i].endereco);
            printf("Quantidade de compras: %d\n", clientes[i].qtdCompras);
            printf("Meu saldo: R$%.2f\n", clientes[i].saldo);
            printf("---------------------------\n");
            break;
        }
    }

}

void acesso_sistema()
{
    int sistema, reentrar;

    printf("Informe a opção de acesso ao Sistema +Açaí!\n1- Administrador\n2- Cliente\n3- Sair\nOpção: ");
    scanf("%d", &sistema);
    getchar();
    if(sistema == 1)
    {

        printf("\nTELA DE AUTENTICAÇÃO ADMINISTRADOR +AÇAÍ\n");
        printf("----------------------------------------\n\n");
        autentica_adm();
    }
    else if(sistema == 2)
    {
        printf("\nTELA DE AUTENTICAÇÃO CLIENTE +AÇAÍ\n");
        printf("------------------------------------\n\n");
        autentica_cliente();
    }
    else if(sistema == 3)
    {
        saindo_sistema();
    }
    else
    {
        printf("*** Opção inválida! Para tentar novamente digite 1! ***\n\n");
        scanf("%d", &reentrar);
        if(reentrar ==1)
        {
            acesso_sistema();
        }
        else
        {
            printf("Como você não irá tentar novamente o sistema será finalizado...");
            saindo_sistema();
        }
    }
}

void saindo_sistema()
{
    gravarArquivo();
    system("cls");
    printf("\n----- Até Breve! Você saiu do Sistema +Açaí! -----\n");
    exit(0);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    system ("color 1f");
    printf("---------------------------------------------------\n");
    printf("|      BEM VINDO AO SISTEMA DE COMPRAS +AÇAÍ      |\n");
    printf("|_________________________________________________|\n\n");


    imprimeInformacoesDebug();
    lerArquivo();
    imprimeInformacoesDebug();

    acesso_sistema();

    return 0;
}

int validarCPF(char cpf[])
{
    int i, j, digito1 = 0, digito2 = 0;
    if(strlen(cpf) != 11)
        return 0;
    else if((strcmp(cpf,"00000000000") == 0) || (strcmp(cpf,"11111111111") == 0) || (strcmp(cpf,"22222222222") == 0) ||
            (strcmp(cpf,"33333333333") == 0) || (strcmp(cpf,"44444444444") == 0) || (strcmp(cpf,"55555555555") == 0) ||
            (strcmp(cpf,"66666666666") == 0) || (strcmp(cpf,"77777777777") == 0) || (strcmp(cpf,"88888888888") == 0) ||
            (strcmp(cpf,"99999999999") == 0))
        return 0;
    else
    {

        for(i = 0, j = 10; i < strlen(cpf)-2; i++, j--)
            digito1 += (cpf[i]-48) * j;
        digito1 %= 11;
        if(digito1 < 2)
            digito1 = 0;
        else
            digito1 = 11 - digito1;
        if((cpf[9]-48) != digito1)
            return 0;
        else

        {
            for(i = 0, j = 11; i < strlen(cpf)-1; i++, j--)
                digito2 += (cpf[i]-48) * j;
            digito2 %= 11;
            if(digito2 < 2)
                digito2 = 0;
            else
                digito2 = 11 - digito2;
            if((cpf[10]-48) != digito2)
                return 0;
        }
    }
    return 1;
}



