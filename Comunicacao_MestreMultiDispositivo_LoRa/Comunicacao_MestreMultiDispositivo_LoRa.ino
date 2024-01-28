/**
 * @file Comunicacao_MestreMultiDispositivo_LoRa.ino
 * @brief Comunicação Mestre-MultiDispositivo com LoRa
 *
 * Este sketch implementa uma comunicação serial mestre-multi-dispositivo usando LoRa,
 * destinado para uso com Arduino UNO. O mestre envia perguntas sequenciais para múltiplos
 * dispositivos secundários e aguarda as respostas. É possível adicionar mais dispositivos 
 * secundários alterando o array `devices` e ajustando a lógica de incremento do `deviceIndex`.
 *
 * Para adicionar mais dispositivos secundários:
 * 1. Inclua os novos nomes dos dispositivos no array `devices`, por exemplo: 
 *    `String devices[] = {"device_A", "device_B", "device_C", "device_D", "device_E"};`
 * 2. Certifique-se de que o incremento do `deviceIndex` reflita o novo tamanho do array:
 *    `deviceIndex = (deviceIndex + 1) % (sizeof(devices) / sizeof(devices[0]));`
 *
 * @author Maxwel
 * @date 24/01/2024
 * @framework Arduino
 * @board Arduino UNO
 * @note Para garantir a eficiência da comunicação, considere o intervalo de tempo necessário
 *       para percorrer todos os dispositivos e ajuste os valores de `intervaloPergunta` e 
 *       `timeoutResposta` conforme necessário.
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

// Definição dos pinos para a comunicação serial no Arduino UNO
#define UART1_RXD_PIN     A5  ///< Pino RX da comunicação serial
#define UART1_TXD_PIN     2   ///< Pino TX da comunicação serial

SoftwareSerial EBYTE_LoRa(UART1_RXD_PIN, UART1_TXD_PIN); ///< Objeto SoftwareSerial para comunicação LoRa

// Estados possíveis para a máquina de estados
enum Estado {
  ESPERA,              ///< Estado de espera para o próximo ciclo de pergunta
  PERGUNTANDO,         ///< Estado de enviar a pergunta ao dispositivo secundário
  AGUARDANDO_RESPOSTA  ///< Estado de aguardar a resposta do dispositivo secundário
};

Estado estadoAtual = ESPERA; ///< Estado atual da máquina de estados

// Variáveis de temporização
unsigned long prevMillis = 0;                   ///< Armazena a última vez que uma pergunta foi feita
const unsigned long intervaloPergunta = 100;    ///< Intervalo entre perguntas (em milissegundos)
const unsigned long timeoutResposta = 500;      ///< Timeout para aguardar a resposta (em milissegundos)
unsigned long startPerguntaTime;                ///< Tempo de início da pergunta atual

// Configuração dos dispositivos secundários
String devices[] = {"device_A", "device_B", "device_C", "device_D"}; ///< Nomes dos dispositivos secundários
uint8_t deviceIndex = 0; ///< Índice para rastrear o dispositivo secundário atual na lista

/**
 * Configuração inicial do sketch.
 */
void setup() {
  Serial.begin(9600);       ///< Inicia a comunicação serial com a porta USB
  EBYTE_LoRa.begin(9600);   ///< Inicia a comunicação LoRa com a configuração especificada
}

/**
 * Loop principal do sketch.
 */
void loop() {
  // Verifica se é hora de enviar uma nova pergunta
  if (millis() - prevMillis >= intervaloPergunta && estadoAtual == ESPERA) {
    prevMillis = millis();
    EBYTE_LoRa.println(devices[deviceIndex]); // Envia pergunta ao dispositivo secundário atual
    estadoAtual = AGUARDANDO_RESPOSTA;
    startPerguntaTime = millis(); // Reinicia o contador para o timeout
  }

  // Máquina de estados para gerenciar o ciclo de perguntas e respostas
  switch (estadoAtual) {
    case ESPERA:
      // Aguarda no estado de espera até que seja a hora de enviar a próxima pergunta
      break;

    case AGUARDANDO_RESPOSTA:
      // Verifica se há uma resposta disponível
      if (EBYTE_LoRa.available() > 0) {
        String resposta = EBYTE_LoRa.readStringUntil('\n'); // Lê a resposta
        Serial.println("Resposta recebida do " + devices[deviceIndex] + ": " + resposta);
        // Avança para o próximo dispositivo e retorna ao estado de espera
        deviceIndex = (deviceIndex + 1) % (sizeof(devices) / sizeof(devices[0]));
        estadoAtual = ESPERA;
      }
      // Verifica se ocorreu um timeout na espera pela resposta
      if (millis() - startPerguntaTime > timeoutResposta) {
        Serial.println("[Timeout]: Nenhuma resposta recebida de " + devices[deviceIndex] + ".");
        // Avança para o próximo dispositivo e retorna ao estado de espera
        deviceIndex = (deviceIndex + 1) % (sizeof(devices) / sizeof(devices[0]));
        estadoAtual = ESPERA;
      }
      break;
  }
}
