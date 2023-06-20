// Pinos
#define TRIGGER 7
#define OUT 2
// Variáveis de tempo
unsigned long clockTrigger = 0;
unsigned long temp1 = 0;
unsigned long temp2 = 0;
// Variáveis de estado
bool triggerState = LOW; 
bool tempFlag = false; // verdadeira se a primeira borda de subida foi salva
bool highFlag = false; // Verdadeira se a leitura que era HIGH ainda não abaixou para LOW

int medeTempo(int pinOut) {
  if (digitalRead(pinOut) == HIGH && highFlag == false) { // Se a saída estiver em nível alto e ainda for a 1ª leitura,
    highFlag = true;    // Mantém a flag de 1ª leitura ativa para não salvar valores novos nas variáveis
    if (!tempFlag) {    // Se a 1ª borda de subida ainda não foi salva
      temp1 = millis();   // salva 
      tempFlag = true;    // Libera a flag para salvar a 2ª borda de subida
    } else {            // Se a 2ª borda de subida ainda não foi salva
      temp2 = millis();   // Salva
      tempFlag = false;   // bloqueia a flag para salvar a 2ª borda de subida
      Serial.println(temp2 - temp1);
      return temp2 - temp1;
    }
  } 
  if (digitalRead(pinOut) == LOW) {
    highFlag = false;
  }
}

void setup() {
  Serial.begin(9600);
  // Inicialização dos pinos
  pinMode(TRIGGER, OUTPUT); // 
  pinMode(OUT, INPUT_PULLUP);
}

void loop() {
  if((millis() - clockTrigger) > 100){
    triggerState = !triggerState;
    digitalWrite(TRIGGER, triggerState);
    clockTrigger = millis();
    //Serial.println(clockState);
  } 
    float t = medeTempo(OUT) / 1000; // medeTempo retorna o intervalo de tempo entre as bordas de subida em milissegundo
    //Serial.println(t);
    //float C = 1 / (1.1 * )
}
