// Pinos
#define TRIGGER 7
#define OUT 2
#define PUSH_BUTTON 5
// Variáveis de tempo
unsigned long clockTrigger = 0;
unsigned long tempSubida = 0;
unsigned long tempDescida = 0;
unsigned long tempBotao = 0;
unsigned long tempCalib = 0;
// Variáveis de estado / flags
bool triggerState = LOW;
bool tempFlag = false;    // verdadeira se a primeira borda de subida foi salva, garante que a 1ª leitura feita seja de uma borda de subida
bool highFlag = false;    // Verdadeira se a leitura que era HIGH ainda não abaixou para LOW
bool lowFlag = false;     // Verdadeira se a leitura que era LOW ainda não subiu para HIGH
bool buttonFlag = false;  // Verdadeiro se o botão foi apertado e ainda não foi solto
bool debounceFlag = false;// Verdadeiro se o debounce já foi feito
bool calibFlag = false;   // Verdadeira se a calibração já foi realizada
bool minFlag = false;     // Verdadeira se o valor mínimo de capacitância da calibração foi salvo
bool maxFlag = false;     // Verdadeira se o valor máximo de capacitância da calibração foi salvo
// Variáveis de lógica
float t = 0;        // intervalo de tempo entre as bordas de subida da saída do 555
float Ra = 100;  // Valor da resistência ligada entre a saída e o trigger do 555
float C = 0, Cmin = 0, Cmax = 0;
float Nmax = 10, N = 0;

void triggerClock(void) { // Chamar em toda estrutura de repetição
  // Clock do trigger: inverte o estatdo de saída a cada 100ms -> perído = 200ms -> 5 amostras por segundo
  if((millis() - clockTrigger) > 100){    // Se a diferença do tempo de agora com a última vez que o trigger foi acionado é maior que 100ms
    triggerState = !triggerState;           // Inverte o estado do trigger
    digitalWrite(TRIGGER, triggerState);    // Aciona o trigger
    clockTrigger = millis();                // Atualiza o tempo da última mudança de estado
    //Serial.println(clockState);
  }
}

int medeTempo(int pinOut) { // Intervalo entre bordas de subida e de descida -> Chamar em toda estrutura de repetição
  int tDiff = 0;
  if (digitalRead(pinOut) == HIGH && !highFlag) { // Se a saída estiver em nível alto e ainda for a mesma leitura,
    tempSubida = millis();  //Salva o valor do tempo
    highFlag = true;      // Mantém a flag de ativa para não salvar valores novos nas variáveis
    lowFlag = false;      // Desativa a lowFlag
    tempFlag = true;      // Ativa a flag que garante que a 1ª leitura foi feita
  }
  if (digitalRead(pinOut) == LOW && !lowFlag && tempFlag) { // Se a saída estiver em nível baixo, ainda for a mesma leitura, e NÃO for a 1ª leitura
    tempDescida = millis(); // Salva o valor do tempo
    lowFlag = true;       // Mantém a flag de ativa para não salvar valores novos nas variáveis
    highFlag = false;     // Desativa a lowFlag
    tDiff = tempDescida - tempSubida; // Calcula o tempo em que a saída ficou em nível alto
    //Serial.println(tDiff);
    return tDiff; // Retorna o valor (ms)
  }
}

void calculaCap(void) {
  float t = medeTempo(OUT);
  t = t / 1000; // Transforma em segundos
  //Serial.print(t);
  //Serial.println(" s");
  C = t / (1.1 * Ra) * 1000000; // Cálculo da capacitância em uF
  //Serial.println(C);
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

void setup() {
  Serial.begin(9600);
  // Inicialização dos pinos
  pinMode(TRIGGER, OUTPUT); // 
  pinMode(OUT, INPUT_PULLUP);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
}

void loop() {
  triggerClock();
  calculaCap();

  if (!calibFlag) {
    if (debounce(PUSH_BUTTON) && !minFlag) {
      Cmin = C;
      minFlag = true;
      Serial.println(Cmin);
    }
    if (debounce(PUSH_BUTTON) && !maxFlag && minFlag) {
      Cmax = C;
      maxFlag = true; 
      Serial.println(Cmax);     
    }
    if (minFlag && maxFlag) {
      calibFlag = true;
    }
  } else {
    N = Nmax * ((C - Cmin) / (Cmax - Cmin));
    Serial.print(N);
    Serial.println(" cm");
  }
}
