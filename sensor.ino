int ledPin = 6; //LED
int sensorPin = 7; //Pino Do Sensor
int leitura = 0; //armazenamento da leitura
bool estadoSensor = false; //Armazem o estado do sensor

void setup() {
    Serial.begin(9600); //Inicia A comunicacao serial
    pinMode(ledPin, OUTPUT); //define "ledpin" como saida
    pinMode(sensorPin, INPUT); // DEFINE "sensorpin" como entrada

}
void loop() {
    leitura = digitalRead(sensorPin); //Realiza a leitura do sensor de movimento
 if (leitura == HIGH) { //SE FOR HIGH= MOVIMENTO DETECTADO
    digitalWrite(ledPin, HIGH); //liga a led
    if (estadoSensor == false){
        //compara o valor do estado do LED
        Serial.print1n("Movimemto Detectado"); //Imprime a mensagem no monitor
        estadoSensor = true; //Altera o valor de estadoSensor para HIGH

    }
    delay(5000); //tempo da led 


 } else {
    //se a leitura for low nao tem movimento
    digitalWrite(ledPin, LOW); //desliga led
    if (estadoSensor == true) {//compara o valor do led
    Serial.print1n("sem movimento"); //mostra no monitor serial
    estadoSensor = false; //Altera o valor do sensor para HIGH
}
 }
}in 