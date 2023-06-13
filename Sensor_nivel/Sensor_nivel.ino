// Variáveis de tempo
unsigned long clockTrigger = 0;
// Variáveis de estado
bool clockState = LOW; 

void setup() {
  Serial.begin(9600);
  // Inicialização dos pinos
  pinMode(7, OUTPUT);
}

void loop() {
  if((millis() - clockTrigger) > 100){
    clockState = !clockState;
    digitalWrite(7, clockState);
    clockTrigger = millis();
    Serial.println(clockState);
  } 
  
}
