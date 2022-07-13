#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ESPACAMENTO -50
#define max_path 260 //variavel com o mesmo valor do MAX_PATH (ou PATH_MAX) 
                    //-> criamos ela pq tivemos problema com o compilador que reconhecia essa variavel com nomes diferentes 

int mostrar_dir(char nm_dir[], int nvl_dir);
int calcular_bytes(char caminhoArq[], unsigned int tipo_arq);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Numero de argumentos incorretos! Informe o caminho para o diretorio.\n");
        return 0;
    }

    if(mostrar_dir(argv[1], 0) == -1)
        printf("Não foi possivel acessar os diretorios.\n");

    return 0;
}

//lista o diretorio informada e seus arquivos internos
int mostrar_dir(char nm_dir[], int nvl_dir){
    DIR *dir = opendir(nm_dir);
    if(dir == NULL)
        return -1;
    
    /*== passo base ==*/
    //mostra o diretorio caso seja o primeiro diretorio
    if(nvl_dir == 0)
        printf("%s\n", nm_dir);

    struct dirent *arq;
    int ind[max_path], cont = 0; //indicador para as posicoes das pastas no diretorio e um contador para controle 

    while ((arq = readdir(dir)) != NULL){
        if(arq->d_name[0] != '.')
             if(arq->d_type != DT_DIR){
                struct stat st;
                
                //formata o caminho para o arquivo
                char nm_arq[strlen(nm_dir) + strlen(arq->d_name) + 2]; // +2 = '\' + '\0'
                sprintf(nm_arq, "%s\\%s", nm_dir, arq->d_name);
                
                if(stat(nm_arq, &st) == -1){
                    closedir(dir);
                    return -1;
                }
                                                //nivel do diretorio * 2 = deslocamento para cada arquivo
                printf("%*s- %*s (%lld bytes)\n", (nvl_dir*2), "", ESPACAMENTO+(nvl_dir*2), arq->d_name, (long long) st.st_size);
            }
            else{
                //para cada pasta encontrada armazena a posicao
                ind[cont] = telldir(dir) -1;
                cont++;
            }
    }
    
    /*== passo recursivo ==*/
    for (int i = 0; i < cont; i++){
        seekdir(dir, ind[i]);
        arq = readdir(dir);
        
        char nv_dir[strlen(nm_dir) + strlen(arq->d_name) + 3];
        sprintf(nv_dir, "%s\\%s", nm_dir, arq->d_name);

                                    //nivel do diretorio * 2 = deslocamento para cada diretorio
        printf("%*s+ %*s (%d bytes)\n", (nvl_dir*2),"", ESPACAMENTO+(nvl_dir*2), arq->d_name, calcular_bytes(nv_dir, arq->d_type));
        mostrar_dir(nv_dir, nvl_dir+1);
    }
    
    closedir(dir);
    return 0;
}

//calcula o tamanho de um arquivo utilizando o caminho informado
//caso seja uma pasta, faz a soma dos seus arquivos internos
int calcular_bytes(char caminhoArq[], unsigned int tipo_arq){
    /*== passo base ==*/
    if (tipo_arq != DT_DIR){
        struct stat st;
        if(stat(caminhoArq, &st) == -1)
            return -1;

        return ((int) st.st_size);
    }

    int tam = 0;

    /*== passo recursivo ==*/
    DIR *dir = opendir(caminhoArq);
    if(dir == NULL)
        return -1;

    struct dirent *arq;
    
    while ((arq = readdir(dir)) != NULL){
        if(arq->d_name[0] != '.'){
            char nvCaminho[strlen(caminhoArq) + strlen(arq->d_name) + 2]; 
            sprintf(nvCaminho, "%s\\%s", caminhoArq, arq->d_name);

            tam += calcular_bytes(nvCaminho, arq->d_type);
        }
    }

    closedir(dir);
    return tam;
}