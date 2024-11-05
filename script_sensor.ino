// Pinos de conexão
const int sensorPortaPin = 2;          // Pino do sensor que simula a porta
const int botaoSilenciarAlarmePin = 4; // Pino do botão para silenciar o alarme temporariamente
const int buzzerPin = 8;                // Pino do buzzer
const int ledPin = 7;                   // Pino do LED

// Variáveis de estado
bool alarmeAtivo = false;              
bool alarmeSilenciado = false;          // Estado para indicar se o alarme está silenciado
unsigned long tempoAberto = 0;         
const unsigned long tempoLimite = 5000; // Tempo limite em milissegundos

// Variáveis para debouncing
unsigned long lastDebounceTime = 0; // O último tempo em que o estado foi alterado
unsigned long debounceDelay = 50;    // Tempo de debounce
int lastLeitura = HIGH;               // Última leitura do botão

void setup() {
  pinMode(sensorPortaPin, INPUT); 
  pinMode(botaoSilenciarAlarmePin, INPUT_PULLUP); // Use INPUT_PULLUP para ativar a resistência interna
  pinMode(buzzerPin, OUTPUT); 
  pinMode(ledPin, OUTPUT);            // Configura o pino do LED como saída
  Serial.begin(9600); // Inicia o monitor serial
}

void loop() {
  int estadoPorta = digitalRead(sensorPortaPin);
  int leituraAtual = digitalRead(botaoSilenciarAlarmePin);
  
  // Verifica se a leitura do botão mudou desde a última verificação
  if (leituraAtual != lastLeitura) {
    lastDebounceTime = millis(); // Reinicia o timer de debounce
  }
  
  // Verifica se a leitura está estável
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Se a leitura está estável e é diferente do estado anterior
    if (leituraAtual == LOW) { // Botão pressionado
      alarmeSilenciado = true; // Silencia o alarme
      digitalWrite(buzzerPin, LOW); // Desliga o buzzer
      digitalWrite(ledPin, HIGH);     // Desliga o LED imediatamente
      Serial.println("Alarme silenciado");
    }
  }
  
  lastLeitura = leituraAtual; // Atualiza a leitura anterior

  if (estadoPorta == HIGH) { // Porta aberta
    if (tempoAberto == 0) {
      tempoAberto = millis(); // Inicia a contagem do tempo
    }
    unsigned long tempoDecorrido = (millis() - tempoAberto) / 1000;
    Serial.println("Porta aberta há " + String(tempoDecorrido) + " segundos");

    // Ativa o alarme se o tempo aberto exceder o limite
    if (millis() - tempoAberto > tempoLimite && !alarmeAtivo && !alarmeSilenciado) {
      alarmeAtivo = true; 
      digitalWrite(buzzerPin, HIGH); // Ativa o alarme
      Serial.println("Alarme ativado!");
    }
  } else { // Porta fechada (LOW)
    // Reinicia todas as variáveis de estado ao fechar a porta
    tempoAberto = 0; 
    alarmeAtivo = false; 
    alarmeSilenciado = false; // Resetar o silenciamento
    digitalWrite(buzzerPin, LOW); // Desativa o alarme
    digitalWrite(ledPin, LOW);     // Liga o LED
    Serial.println("Sistema resetado. Pronto para reiniciar.");
  }

  delay(10); // Pequeno atraso para estabilizar a leitura
}
