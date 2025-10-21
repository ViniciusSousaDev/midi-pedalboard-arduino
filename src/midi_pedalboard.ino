/*
  🎛️ Pedaleira MIDI - Projeto de Vinícius Soares de Sousa
  ---------------------------------------------------------
  Este código transforma um Arduino (Nano ou Uno) em uma pedaleira MIDI simples.
  Cada botão envia um comando MIDI via USB, e o LED indica se o efeito está ligado.

  Bibliotecas necessárias:
  - MIDIUSB (instale via Gerenciador de Bibliotecas do Arduino IDE)
*/

#include <MIDIUSB.h>

// Quantidade de botões da pedaleira
const int quantidadeBotoes = 4;

// Pinos dos botões
const int pinosBotoes[quantidadeBotoes] = {2, 3, 4, 5};

// Pinos dos LEDs indicadores
const int pinosLeds[quantidadeBotoes] = {6, 7, 8, 9};

// Estados atuais e anteriores dos botões
bool estadoBotao[quantidadeBotoes] = {0, 0, 0, 0};
bool ultimoEstadoBotao[quantidadeBotoes] = {0, 0, 0, 0};

// Estados dos LEDs (ligado/desligado)
bool estadoLed[quantidadeBotoes] = {0, 0, 0, 0};

void setup() {
  // Define pinos como entrada ou saída
  for (int i = 0; i < quantidadeBotoes; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);  // Botão com resistor interno
    pinMode(pinosLeds[i], OUTPUT);          // LED como saída
    digitalWrite(pinosLeds[i], LOW);        // Todos LEDs começam apagados
  }
}

void loop() {
  // Verifica o estado de cada botão
  for (int i = 0; i < quantidadeBotoes; i++) {
    estadoBotao[i] = !digitalRead(pinosBotoes[i]); // Invertido por causa do INPUT_PULLUP

    // Detecta o momento em que o botão foi pressionado
    if (estadoBotao[i] && !ultimoEstadoBotao[i]) {
      estadoLed[i] = !estadoLed[i]; // Alterna o LED
      digitalWrite(pinosLeds[i], estadoLed[i]);

      // Envia o comando MIDI
      enviarControleMIDI(i + 20, estadoLed[i] ? 127 : 0);
    }

    // Atualiza o estado anterior do botão
    ultimoEstadoBotao[i] = estadoBotao[i];
  }

  delay(50); // Pequeno atraso para evitar leitura dupla do mesmo clique
}

// Função para enviar comando MIDI de controle (Control Change)
void enviarControleMIDI(byte controle, byte valor) {
  midiEventPacket_t pacote = {0x0B, 0xB0, controle, valor};
  MidiUSB.sendMIDI(pacote);
  MidiUSB.flush();
}
