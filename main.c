#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


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
  QueueHandle_t xQueue;
  int i = 0; //variável bizarra para teste e debug
  TaskHandle_t xTaskMaqEstados;
  TaskHandle_t xTaskObterEvento;
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

int vtaskObterEvento()
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
int codigoEvento;
void taskObterEvento(void *pvParameters) {
  
  BaseType_t xStatus;
  for( ;; ) {
    codigoEvento = NENHUM_EVENTO;
    // Adicionar lógica para receber eventos
    if (i%2==0) {// LÓGICA DE TESTE
      codigoEvento = INICIAR;
      xStatus = xQueueSendToBack( xQueue, &codigoEvento, 0 );
      if( xStatus != pdPASS )
        //Serial.println("Erro ao enviar evento para fila");   
        printf("Erro ao enviar evento para fila 1 \n");
      vTaskDelay(200);
      continue;
    }
    if (i%3==0) {// LÓGICA DE TESTE
      codigoEvento = PLOTAR;
      xStatus = xQueueSendToBack( xQueue, &codigoEvento, 0 );
      if( xStatus != pdPASS )
        //Serial.println("Erro ao enviar evento para fila");
        printf("Erro ao enviar evento para fila 2 \n");
        vTaskDelay(200);
      continue;
    }
    if (i%4==0) {// LÓGICA DE TESTE
      codigoEvento = RETORNAR;
      xStatus = xQueueSendToBack( xQueue, &codigoEvento, 0 );
      if( xStatus != pdPASS )
        //Serial.println("Erro ao enviar evento para fila");
        printf("Erro ao enviar evento para fila 3 \n");
        vTaskDelay(200);
      continue;
    }
    if (i%5==0) {// LÓGICA DE TESTE
      codigoEvento = ENCERRAR;
      xStatus = xQueueSendToBack( xQueue, &codigoEvento, 0 );
      if( xStatus != pdPASS )
        //Serial.println("Erro ao enviar evento para fila");
        printf("Erro ao enviar evento para fila 4 \n");
        vTaskDelay(200);
      continue;
    }
  }
}


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


/*********************************************************************************/


void taskMaqEstados(void *pvParameters) {
  int codigoEvento;
  const TickType_t xDelay2000ms = pdMS_TO_TICKS( 2000 );
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for( ;; ) {
    if( xQueueReceive( xQueue, &codigoEvento, portMAX_DELAY ) == pdPASS ) {
      if (codigoEvento != NENHUM_EVENTO)
      {
        codigoAcao = obterAcao(estado, codigoEvento);
        estado = obterProximoEstado(estado, codigoEvento);
        executarAcao(codigoAcao);
        /*Serial.print("Estado: ");
        Serial.print(estado);
        Serial.print(" Evento: ");
        Serial.print(codigoEvento);
        Serial.print(" Acao: ");
        Serial.println(codigoAcao);*/
        printf("%d", codigoEvento);
        printf("Estado: %d Evento: %d Acao:%d\n", estado, codigoEvento, codigoAcao);
      }
    }
    else {
      printf("Erro ao receber evento da fila\n");
    }
    printf("Task 3 (periodic) is running\n");
    vTaskDelayUntil(&xLastWakeTime, xDelay2000ms);
    i++;
  }
}

/************************************************************************
 Main
 Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
int estado;
int eventoInterno;

int estado = ESPERA;
int eventoInterno = NENHUM_EVENTO;
/*
int main( void ) {

  int codigoEvento;
  int codigoAcao;
  

  iniciaSistema();
  printf ("PenPlot pronto\n");
  // --- APPLICATION TASKS CAN BE CREATED HERE --- 
    xTaskCreate( taskMaqEstados, "Task 1", 1000, NULL, 1, NULL);
 
    // Start the created tasks running.
    vTaskStartScheduler();
 
    // Execution will only reach here if there was insufficient heap to
    start the scheduler. 
    for( ;; );
    return 0;
} // main
*/
int main() {
  iniciaSistema();
  printf ("Alarme iniciado\n");

  // configure tasks
  xQueue = xQueueCreate(5, sizeof(int));
  if(xQueue != NULL)
  {
    xTaskCreate(taskMaqEstados,"taskMaqEstados", 1500, NULL, 2, &xTaskMaqEstados);
    xTaskCreate(taskObterEvento,"taskObterEvento", 1000, NULL, 1, &xTaskObterEvento);
    vTaskStartScheduler();
  }
  else
  {
    /* The queue could not be created. */
  }
} // main