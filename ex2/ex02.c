#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dimensaoX 17 // 15 + '\n' + '\0'
#define dimensaoY 10

int le_figura(char nmArqEntrada[], char figura[][dimensaoX]);
int escreve_figura(char nmArqSaida[], char figura[][dimensaoX]);
void pintar_figura(char figura[][dimensaoX], int x, int y, char c);

int main(int argc, char *argv[]){
    char figura[dimensaoY][dimensaoX];

    if(argc < 6)
        printf("Numero de argumentos invalidos \n");
    
    else if(atoi(argv[3]) >= (dimensaoX-2) || atoi(argv[3]) < 0)
        printf("Coordenada x fora dos limites figura (0 ... %d) \n", dimensaoX-3);
    
    else if(atoi(argv[4]) >= dimensaoY || atoi(argv[4]) < 0)
        printf("Coordenada y fora dos limites da figura (0 ... %d) \n", dimensaoY-1);
    
    else if(argv[5][0] != '*' && argv[5][0] != ' ')
        printf("Cor invalida, escolha entre '*' ou ' ' \n");
    
    else{
        if(le_figura(argv[1], figura) == 0){
            pintar_figura(figura, atoi(argv[3]), atoi(argv[4]), argv[5][0]);
            
            if(escreve_figura(argv[2], figura) == -1)
                printf("Não foi possivel escrever a figura pintada em %s \n",argv[2]);
        }
        else{
            printf("Nao foi possivel acessar o arquivo da figura \n");
        }
    }

    return 0;
}

//le a figura de um arquivo com o nome informado e escreve em uma matriz de dimensaoX por dimensaoY
//retorna -1 em caso de erro ao abrir o arquivo
int le_figura(char nmArqEntrada[], char figura[][dimensaoX]){
    FILE *arq = fopen(nmArqEntrada, "r");
    if(arq == NULL)
        return -1;

    for (int i = 0; i < dimensaoY; i++)
        fgets(figura[i], dimensaoX, arq);
        
    fclose(arq);
    return 0;
}

//escreve uma matriz de dimensaoX por dimensaoY em um arquivo com o nome informado
//retorna -1 em caso de erro ao abrir o arquivo
int escreve_figura(char nmArqSaida[], char figura[][dimensaoX]){
    FILE *arq = fopen(nmArqSaida, "w");
    if(arq == NULL)
        return -1;

    for (int i = 0; i < dimensaoY; i++)
        fputs(figura[i], arq);

    fclose(arq);
    return 0;
}

//pinta uma matriz com a cor informada
void pintar_figura(char figura[][dimensaoX], int x, int y, char c){
    //passo base:
    if(x >= (dimensaoX-2) || x < 0 || y >= dimensaoY || y < 0)
        return;

    if(figura[y][x] == c)
        return;
    
    //passo recursivo:
    figura[y][x] = c;

    pintar_figura(figura, x-1, y, c); //esquerda
    pintar_figura(figura, x+1, y, c); //direita
    pintar_figura(figura, x, y-1, c); //cima
    pintar_figura(figura, x, y+1, c); //baixo
}
