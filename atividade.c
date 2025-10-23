#include <stdio.h>
#include <string.h>
 
#define TAM 256
 
void lerStr(char str[], int tam);
void inserirFinalArquivo(char nomeArq[]);
void listarArquivo(char nomeArq[]);
void lerPosicaoEspecifica(char nomeArq[]);
void buscarNumero(char nomeArq[]);
void substituirNumero(char nomeArq[]);
void excluirNumero(char nomeArq[]);
 
int main() {
  char nomeArq[TAM];
  int opcao;
  printf("Digite o nome do arquivo: ");
  lerStr(nomeArq, TAM);
  while(strlen(nomeArq) == 0) {
    printf("Nome vazio é inválido!\n");
    printf("Digite o nome do arquivo: ");
    lerStr(nomeArq, TAM);
  }
  do {
    printf("\n\nMENU\n");
    printf("1 - Gravar um número no final do arquivo\n");
    printf("2 - Listar o conteúdo do arquivo\n");
    printf("3 - Imprimir valor específico\n");
    printf("4 - Buscar se um número está salvo no arquivo\n");
    printf("5 - Substituir um número por outro\n");
    printf("6 - Excluir um número do arquivo\n");
    printf("Digite a sua opção (0 p/ finalizar): ");
    scanf("%d", &opcao);
    switch (opcao) {
      case 1:
        inserirFinalArquivo(nomeArq);
        break;
      case 2:
        printf("---------------- Listagem ----------------\n");
        listarArquivo(nomeArq);
        printf("------------------------------------------\n");
        break;
      case 3:
        lerPosicaoEspecifica(nomeArq);
        break;
      case 4:
        buscarNumero(nomeArq);
        break;
      case 5:
        substituirNumero(nomeArq);
        break;
      case 6:
        excluirNumero(nomeArq);
        break;
      default:
        if (opcao != 0) {
          printf("Opção inválida!\n");
        }
    }
  } while (opcao != 0);
  return 0;
}
 
void lerStr(char str[], int tam) {
  fgets(str, tam, stdin);
  int tamDigitado = strlen(str);
  if (str[tamDigitado - 1] == '\n') {
    str[tamDigitado - 1] = '\0';
  }
}
 
void inserirFinalArquivo(char nomeArq[]) {
  int status;
  double num;
  FILE *arq = fopen(nomeArq, "ab");
  if (arq == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  printf("Digite um número real: ");
  scanf("%lf", &num);
  status = fwrite(&num, sizeof(double), 1, arq);
  if (status == 1) {
    printf("Valor armazenado com sucesso!\n");
  } else {
    printf("Erro ao gravar o valor no arquivo!\n");
  }
  fclose(arq);
}
 
void listarArquivo(char nomeArq[]) {
  int status;
  double num;
  FILE *arq = fopen(nomeArq, "rb");
  if (arq == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  status = fread(&num, sizeof(double), 1, arq);
  while (!feof(arq)) {
    if (status != 1) {
      printf("Erro na leitura do registro");
    } else {
      printf("%.3f\n", num);
    }
    status = fread(&num, sizeof(double), 1, arq);
  }
  fclose(arq);
}
 
void lerPosicaoEspecifica(char nomeArq[]) {
  int status, pos;
  double num;
  FILE *arq = fopen(nomeArq, "rb");
  if (arq == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  printf("Você quer mover para que posição: ");
  scanf("%d", &pos);
  status = fseek(arq, (pos - 1) * sizeof(double), SEEK_SET);
  if (status != 0 || feof(arq)) {
    printf("Essa é uma posição inválida!");
    fclose(arq);
    return;
  }
  status = fread(&num, sizeof(double), 1, arq);
  if (status != 1) {
    printf("Erro na leitura do registro");
  } else {
    printf("%.3f\n", num);
  }
  fclose(arq);
}
 
void buscarNumero(char nomeArq[]) {
  int status, pos = 1, encontrado = 0;
  double num, valor;
  FILE *arq = fopen(nomeArq, "rb");
  if (arq == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  printf("Digite o número que deseja buscar: ");
  scanf("%lf", &valor);
  status = fread(&num, sizeof(double), 1, arq);
  while (!feof(arq)) {
    if (status != 1) {
      printf("Erro na leitura do registro!\n");
    } else if (num == valor) {
      printf("Valor encontrado na posição %d!\n", pos);
      encontrado = 1;
    }
    pos++;
    status = fread(&num, sizeof(double), 1, arq);
  }
  if (!encontrado) {
    printf("Valor não encontrado no arquivo.\n");
  }
  fclose(arq);
}
 
void substituirNumero(char nomeArq[]) {
  int status, alterado = 0;
  double num, antigo, novo;
  FILE *arq = fopen(nomeArq, "r+b");
  if (arq == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  printf("Digite o número que deseja substituir: ");
  scanf("%lf", &antigo);
  printf("Digite o novo número: ");
  scanf("%lf", &novo);
  status = fread(&num, sizeof(double), 1, arq);
  while (!feof(arq)) {
    if (status != 1) {
      printf("Erro na leitura do registro!\n");
    } else if (num == antigo) {
      fseek(arq, -sizeof(double), SEEK_CUR);
      fwrite(&novo, sizeof(double), 1, arq);
      alterado = 1;
    }
    status = fread(&num, sizeof(double), 1, arq);
  }
  if (alterado) {
    printf("Número substituído com sucesso!\n");
  } else {
    printf("Valor não encontrado no arquivo.\n");
  }
  fclose(arq);
}
 
void excluirNumero(char nomeArq[]) {
  int status, gravado;
  double num, excluir;
  FILE *arq1, *arq2;
  char nomeTemp[TAM] = "temporario.bin";
  arq1 = fopen(nomeArq, "rb");
  if (arq1 == NULL) {
    printf("Não pode abrir o arquivo!\n");
    return;
  }
  arq2 = fopen(nomeTemp, "wb");
  if (arq2 == NULL) {
    printf("Não pode criar arquivo temporário!\n");
    fclose(arq1);
    return;
  }
  printf("Digite o número que deseja excluir: ");
  scanf("%lf", &excluir);
  status = fread(&num, sizeof(double), 1, arq1);
  while (!feof(arq1)) {
    if (status != 1) {
      printf("Erro na leitura do registro!\n");
    } else if (num != excluir) {
      gravado = fwrite(&num, sizeof(double), 1, arq2);
      if (gravado != 1) {
        printf("Erro ao gravar no arquivo temporário!\n");
      }
    }
    status = fread(&num, sizeof(double), 1, arq1);
  }
  fclose(arq1);
  fclose(arq2);
  remove(nomeArq);
  rename(nomeTemp, nomeArq);
  printf("Número apagado com sucesso (caso existisse)!\n");
}