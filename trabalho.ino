const int sensorPin = 2;  // Pino de entrada do sensor de fluxo (pulso)
const int bombaPin = 9;   // Pino de controle da bomba
const float vazaoControlada = 10; // Vazão desejada em L/min

volatile int pulseCount = 0;
unsigned long lastTime = 0;
float vazaoEntrada = 0.0;
float integral = 0;
float erro = 0;
int pwmOutput = 140;

void pulseCounter() {
    pulseCount++;
}

void setup() {
    pinMode(sensorPin, INPUT_PULLUP);
    pinMode(bombaPin, OUTPUT);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, RISING);
}

void loop() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000) { 
        detachInterrupt(digitalPinToInterrupt(sensorPin));
        
        
        vazaoEntrada = (pulseCount / 8.2);
        pulseCount = 0;
        erro = vazaoControlada - vazaoEntrada;
        integral = integral + erro;
        lastTime = currentTime;
        
        pwmOutput = pwmOutput + 2.5*erro + 1.5*integral;
        if(pwmOutput>255){
          pwmOutput = 255;
        }
        if(pwmOutput<140){
          pwmOutput = 140;
        }
        
        
        analogWrite(bombaPin, pwmOutput);
        
        Serial.print("Vazao Entrada: ");
        Serial.print(vazaoEntrada);
        Serial.print(" L/min, PWM Output: ");
        Serial.print(pwmOutput);
        Serial.println();
        
        attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, RISING);
    }
}