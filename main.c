#include <stdio.h>
#include <stdlib.h>

/*
    ALARME RESIDENCIAL
*/

#include "definicoes_sistema.h"
#include "comunicacao.h"
#include "ctrl.h"
#include "interface.h"

/***********************************************************************
 Estaticos
 ***********************************************************************/
  int codigoEvento;
  int codigoAcao;
  int estado;
  int sensores;
  int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
  int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];

/************************************************************************
 executarAcao
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NENHUM_EVENTO
*************************************************************************/
void executarAcao(int codigoAcao)
{

    if (codigoAcao == NENHUMA_ACAO){
        printf("nenhuma acao");
        return;
    }
    switch(codigoAcao)
    {
    case A01:
        printf("esperando desenho \n");
        break;
    case A02:
        printf("plotando \n");
        break;
    case A03:
        printf("retornando \n");
        break;
    case A04:
        printf("definicoes canceladas \n");
        break;
    case A05:
        printf("plot cancelado \n");
        break;
    case A06:
        printf("retornado \n");
        break;
    } // switch

} // executarAcao

/************************************************************************
 iniciaMaquina de Estados
 Carrega a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaMaquinaEstados()
{
  int i;
  int j;

  for (i=0; i < NUM_ESTADOS; i++) {
    for (j=0; j < NUM_EVENTOS; j++) {
       acao_matrizTransicaoEstados[i][j] = NENHUMA_ACAO;
       proximo_estado_matrizTransicaoEstados[i][j] = i;
    }
  }
  proximo_estado_matrizTransicaoEstados[ESPERA][INICIAR] = DEFINICOES;
  acao_matrizTransicaoEstados[ESPERA][INICIAR] = A01;

  proximo_estado_matrizTransicaoEstados[DEFINICOES][PLOTAR] = PLOTANDO;
  acao_matrizTransicaoEstados[DEFINICOES][PLOTAR] = A02;

  proximo_estado_matrizTransicaoEstados[DEFINICOES][ENCERRAR] = ESPERA;
  acao_matrizTransicaoEstados[DEFINICOES][ENCERRAR] = A04;

  proximo_estado_matrizTransicaoEstados[PLOTANDO][RETORNAR] = RETORNANDO;
  acao_matrizTransicaoEstados[PLOTANDO][RETORNAR] = A03;

  proximo_estado_matrizTransicaoEstados[PLOTANDO][ENCERRAR] = ESPERA;
  acao_matrizTransicaoEstados[PLOTANDO][ENCERRAR] = A05;

  proximo_estado_matrizTransicaoEstados[RETORNANDO][ENCERRAR] = ESPERA;
  acao_matrizTransicaoEstados[RETORNANDO][ENCERRAR] = A06;


} // initStateMachine

/************************************************************************
 iniciaSistema
 Inicia o sistema ...
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaSistema()
{
   iniciaMaquinaEstados();
} // initSystem


/************************************************************************
 obterEvento
 Obtem um evento, que pode ser da IHM ou do alarme
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/

// int decodificarAcionar()
// {
//     if (teclas[2] == 'a')
//     {
//         if (sha_validar(teclas))
//         {
//             return true;
//         }
//     }
//     return false;
// }//decodificarAcionar

// int decodificarDesacionar()
// {
//     if (teclas[2] == 'd')
//     {
//         if (sha_validar(teclas))
//         {
//             return true;
//         }
//     }
//     return false;
// }//decodificarDesacionar

// int decodificarDisparar()
// {
//     if (teclas[0] == 'l')
//     {
//         return true;
//     }
//     return false;
// }//decodificarDisparar

// int decodificarTimeout()
// {
//     return tmr_timeout();
// }

int obterEvento()
{
  char teclas;

  printf("comando: ");
  scanf("%c", &teclas);
  getchar();
  switch(teclas){
    case '1':
        return INICIAR;
        break;
    case '2':
        return PLOTAR;
        break;
    case '3':
        return RETORNAR;
        break;
    case '4':
        return ENCERRAR;
        break;
    default:
        return NENHUM_EVENTO;
  }
} // obterEvento

/************************************************************************
 obterAcao
 Obtem uma acao da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo da acao
*************************************************************************/
int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


/************************************************************************
 obterProximoEstado
 Obtem o proximo estado da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo do estado
*************************************************************************/
int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


/************************************************************************
 Main
 Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
int main() {

  int codigoEvento;
  int codigoAcao;
  int estado;
  int eventoInterno;

  estado = ESPERA;
  eventoInterno = NENHUM_EVENTO;

  iniciaSistema();
  printf ("PenPlot pronto\n");
  while (true) {
    if (eventoInterno == NENHUM_EVENTO) {
        codigoEvento = obterEvento();
    } else {
        codigoEvento = eventoInterno;
    }
    if (codigoEvento != NENHUM_EVENTO)
    {
       codigoAcao = obterAcao(estado, codigoEvento);
       estado = obterProximoEstado(estado, codigoEvento);
       executarAcao(codigoAcao);
       eventoInterno = NENHUM_EVENTO;
       printf("Estado: %d Evento: %d Acao:%d\n", estado, codigoEvento, codigoAcao);
    }
  } // while true
} // main
