// Pinos
#define TRIGGER 7
#define OUT 2
#define PUSH_BUTTON 5
// Variáveis de tempo
unsigned long clockTrigger = 0;
unsigned long tempSubida = 0;
unsigned long tempDescida = 0;
unsigned long tempBotao = 0;
// Variáveis de estado / flags
bool triggerState = LOW;
bool tempFlag = false;    // verdadeira se a primeira borda de subida foi salva, garante que a 1ª leitura feita seja de uma borda de subida
bool highFlag = false;    // Verdadeira se a leitura que era HIGH ainda não abaixou para LOW
bool lowFlag = false;     // Verdadeira se a leitura que era LOW ainda não subiu para HIGH
bool buttonFlag = false;  // Verdadeiro se o botão foi apertado e ainda não foi solto
bool debounceFlag = false;// Verdadeiro se o debounce já foi feito
bool calibracao = false;  // Verdadeira se a calibração já foi realizada
// Variáveis de lógica
float t = 0; // intervalo de tempo entre as bordas de subida da saída do 555
float Ra = 100000;
float C = 0;

int medeTempo(int pinOut) { // Intervalo entre bordas de subida e de descida
  int tDiff = 0;
  if (digitalRead(pinOut) == HIGH && !highFlag) { // Se a saída estiver em nível alto e ainda for a mesma leitura,
    highFlag = true;      // Mantém a flag de ativa para não salvar valores novos nas variáveis
    lowFlag = false;      // Desativa a lowFlag
    tempFlag = true;      // Ativa a flag que garante que a 1ª leitura foi feita
    tempSubida = millis();  //Salva o valor do tempo
  }
  if (digitalRead(pinOut) == LOW && !lowFlag && tempFlag) { // Se a saída estiver em nível baixo, ainda for a mesma leitura, e NÃO for a 1ª leitura
    lowFlag = true;       // Mantém a flag de ativa para não salvar valores novos nas variáveis
    highFlag = false;     // Desativa a lowFlag
    tempDescida = millis(); // Salva o valor do tempo
    tDiff = tempDescida - tempSubida; // Calcula o tempo em que a saída ficou em nível alto
    //Serial.println(tDiff);
    return tDiff; // Retorna o valor
  }
}

bool debounce (int pinPushButton) {
  if (!digitalRead(pinPushButton) && !buttonFlag) {
    buttonFlag = true;
    debounceFlag = false;
    tempBotao = millis();
  }
  if (digitalRead(pinPushButton)) {
    buttonFlag = false;
    return LOW;
  }
  if ((millis() - tempBotao) > 50 && !digitalRead(pinPushButton) && !debounceFlag) {
    debounceFlag = true;
    return HIGH;
  }
  return LOW;
}

void calibragem (void) {
  ;
}

void setup() {
  Serial.begin(9600);
  // Inicialização dos pinos
  pinMode(TRIGGER, OUTPUT); // 
  pinMode(OUT, INPUT_PULLUP);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
}

void loop() {
  // Clock do trigger: inverte o estatdo de saída a cada 100ms -> perído = 200ms -> 5 amostras por segundo
  if((millis() - clockTrigger) > 100){    // Se a diferença do tempo de agora com a última vez que o trigger foi acionado é maior que 100ms
    triggerState = !triggerState;           // Inverte o estado do trigger
    digitalWrite(TRIGGER, triggerState);    // Aciona o trigger
    clockTrigger = millis();                // Atualiza o tempo da última mudança de estado
    //Serial.println(clockState);
  }

  float t = medeTempo(OUT); // medeTempo retorna o intervalo de tempo entre as bordas de subida e de descida em milissegundos
  t = t / 1000; // Transforma em segundos
  C = t / (1.1 * Ra) * 1000000; // Cálculo da capacitância em uF
  //Serial.print(C);
  //Serial.println(" uF");

  if (debounce(PUSH_BUTTON)) {
    Serial.println("debug");
  } else {
    ;
  }
}
