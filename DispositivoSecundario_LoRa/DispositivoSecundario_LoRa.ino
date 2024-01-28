/**
 * @file DispositivoSecundario_LoRa.ino
 * @brief Dispositivo Secundário para Comunicação LoRa
 *
 * Este sketch implementa o comportamento de um dispositivo secundário em uma comunicação 
 * LoRa mestre-multi-dispositivo. Ele aguarda perguntas do dispositivo mestre e responde
 * de acordo. O nome do dispositivo é definido na diretiva DISPOSITIVO_ATUAL.
 *
 * Para configurar este sketch para um dispositivo específico, altere o valor de 
 * DISPOSITIVO_ATUAL para corresponder ao nome do dispositivo. Descomente a linha 
 * correspondente ao dispositivo que deseja configurar e comente as demais.
 *
 * @author      Maxwel
 * @date        24/01/2024
 * @framework   Arduino
 * @board       Heltec WiFi Kit 32 v3
 */

#include <Arduino.h>
#include <HardwareSerial.h>

// Define o nome do dispositivo atual
#define DISPOSITIVO_ATUAL "device_A"
//#define DISPOSITIVO_ATUAL "device_B"
//#define DISPOSITIVO_ATUAL "device_C"
//#define DISPOSITIVO_ATUAL "device_D"

// Definindo os pinos para a comunicação serial
#define UART1_RXD_PIN     45  ///< Pino RX para comunicação serial
#define UART1_TXD_PIN     42  ///< Pino TX para comunicação serial

HardwareSerial EBYTE_LoRa(1); // Instância do HardwareSerial para comunicação LoRa

// Estados possíveis para a máquina de estados
enum Estado {
  ESPERA,        ///< Aguardando por dados de entrada
  PROCESSANDO,   ///< Processando os dados recebidos
  RESPONDENDO    ///< Enviando uma resposta
};

Estado estadoAtual = ESPERA; ///< Estado inicial da máquina de estados
unsigned long lastCheck = 0; ///< Momento da última verificação de dados
const long checkInterval = 10; ///< Intervalo para verificar a entrada serial (em milissegundos)

String pergunta; ///< Armazena a pergunta recebida

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial com o computador
  EBYTE_LoRa.begin(9600, SERIAL_8N1, UART1_RXD_PIN, UART1_TXD_PIN); // Inicia a comunicação LoRa
}

void loop() {
  // Verifica periodicamente a entrada de dados
  if (millis() - lastCheck >= checkInterval) {
    lastCheck = millis(); // Atualiza o momento da última verificação

    switch (estadoAtual) {
      case ESPERA:
        // Verifica se há dados disponíveis para leitura
        if (EBYTE_LoRa.available() > 0) {
          estadoAtual = PROCESSANDO; // Muda para o estado de processamento
        }
        break;

      case PROCESSANDO:
        // Lê a pergunta recebida até encontrar uma quebra de linha
        pergunta = EBYTE_LoRa.readStringUntil('\n');
        pergunta.trim(); // Remove espaços e caracteres de nova linha

        // Compara a pergunta recebida com o nome do dispositivo atual
        if (pergunta == DISPOSITIVO_ATUAL) {
          estadoAtual = RESPONDENDO; // Se corresponder, prepara para responder
        } else {
          estadoAtual = ESPERA; // Se não, volta para o estado de espera
        }
        break;

      case RESPONDENDO:
        // Envia uma resposta confirmando o recebimento da pergunta
        EBYTE_LoRa.println(pergunta + " - OK");
        estadoAtual = ESPERA; // Retorna ao estado de espera após a resposta
        break;
    }
  }

  // Aqui você pode adicionar outras funções não bloqueantes, se necessário
}
