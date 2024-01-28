# Comunicação Mestre-MultiDispositivo com LoRa

Este repositório contém o código e a documentação para um sistema de comunicação LoRa mestre-multi-dispositivo, utilizando o módulo LoRa serial EBYTE E220-900T22D. O sistema é projetado para ser compatível com qualquer microcontrolador, com exemplos fornecidos para Arduino.

## Visão Geral

O sistema consiste em um dispositivo mestre que envia perguntas ou comandos para múltiplos dispositivos secundários. Cada dispositivo secundário responde ao mestre com base nas instruções recebidas. Este sistema é ideal para aplicações que requerem comunicação sem fio de longo alcance, como monitoramento de sensores ou controle de dispositivos em áreas extensas.

## Componentes

- **Dispositivo Mestre:** Responsável por iniciar a comunicação e coordenar os dispositivos secundários.
- **Dispositivos Secundários:** Respondem ao mestre com informações ou confirmações.

## Hardware

- Módulo LoRa EBYTE E220-900T22D.
- Microcontroladores compatíveis com Arduino (ou similares), ESP32, STM32, etc para dispositivos mestre e secundários.

## Configuração

### Dispositivo Mestre

1. Carregue o sketch `Comunicacao_LoRa_MASTER.ino` no microcontrolador do dispositivo mestre.
2. Conecte o módulo LoRa EBYTE E220-900T22D aos pinos apropriados definidos no código.

### Dispositivos Secundários

1. Para cada dispositivo secundário, carregue o sketch `DispositivoSecundario_LoRa.ino`.
2. Altere a definição `DISPOSITIVO_ATUAL` no topo do sketch para corresponder ao nome do dispositivo específico (por exemplo, `device_A`, `device_B`).
3. Conecte o módulo LoRa EBYTE E220-900T22D aos pinos especificados no código.

## Funcionamento

- O dispositivo mestre envia uma mensagem para um dispositivo secundário específico.
- O dispositivo secundário correspondente responde ao mestre.
- O mestre gerencia as respostas e organiza o fluxo de comunicação.

## Aplicações

- Monitoramento remoto de sensores.
- Controle de sistemas em ambientes extensos.
- Aplicações de IoT que exigem comunicação de longo alcance.

## Suporte e Contribuições

Sinta-se à vontade para contribuir com este projeto, seja com melhorias no código, sugestões de novas funcionalidades ou correções de bugs. Para suporte, abra uma issue neste repositório.
