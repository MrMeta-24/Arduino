#define clk 2
#define latch 1
#define dados 0
#define qtdCI 4

#define led_jogador1 5
#define led_jogador2 6

#define pot1 A0
#define pot2 A1
#define btn1 11
#define btn2 10

byte p1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte p2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

const byte gabarito[8][9] = {
  {1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 1},
  {0, 0, 1, 0, 1, 0, 1, 0, 0}
};

int press1;
int press2;
int gabarito_pos_acerto = 0;
int jogador_vez;

void setup()
{
    pinMode(clk, OUTPUT);
  	pinMode(latch, OUTPUT);
  	pinMode(dados, OUTPUT);
  
  	pinMode(btn1, INPUT);
  	pinMode(btn2, INPUT);
  
  	pinMode(led_jogador1, OUTPUT);
  	pinMode(led_jogador2, OUTPUT);
  	
  	randomSeed(analogRead(2));
  	int rand = random(5);

  	jogador_vez = rand  < 3 ? jogador_vez = 0: jogador_vez = 1;
}

void loop()
{
  press1 = digitalRead(btn1);
  press2 = digitalRead(btn2);
  
  if (jogador_vez == 0){
    digitalWrite(led_jogador1, HIGH);
    int pos1 = 8 - map(analogRead(pot1), 0, 1023, 0, 8);
    escolhePos(pos1);
        
    if (press1 == 1 && (p1[pos1] == 0 && p2[pos1] == 0)){
      p1[pos1] = 1;
      jogador_vez = trocaVez();
      digitalWrite(led_jogador1, LOW);
    }
  }
  if (jogador_vez == 1){
    digitalWrite(led_jogador2, HIGH);
    int pos2 = 8 - map(analogRead(pot2), 0, 1023, 0, 8);
    escolhePos(pos2);
        
    if (press2 == 1 && (p1[pos2] == 0 && p2[pos2] == 0)){
      p2[pos2] = 1;
      jogador_vez = trocaVez();
      digitalWrite(led_jogador2, LOW);
    }
  }
  
  for (int i = 0; i < 9; i++){
    if (p1[i] == 1){
      	ciWrite(i*2, HIGH);
    }
    if (p2[i] == 1){
      ciWrite((i*2)+1, HIGH);
    }
  }
  checkResult();
}

void ciWrite(byte pino, bool estado){
  static byte ciBuffer[qtdCI];
  
  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  
  digitalWrite(latch, LOW);
  digitalWrite(dados, LOW);
  digitalWrite(clk, LOW);
  
  for (int i = qtdCI-1; i >=0; i--){
    for (int j = 7; j >= 0; j--){
      digitalWrite(clk, LOW);
      digitalWrite(dados, bitRead(ciBuffer[i], j));
      digitalWrite(clk, HIGH);
      digitalWrite(dados, LOW);
    }
  }
  digitalWrite(latch, HIGH);
}

void escolhePos(byte pos){
  int n_pos = pos + 18;
  if (p1[pos] == 0 && p2[pos] == 0){
    ciWrite(n_pos, HIGH);
    delay(20);
    ciWrite(n_pos, LOW);
    delay(50);
  }
}

int trocaVez(){
  int vez;
  jogador_vez == 1 ?  vez = 0 : vez = 1;
  return vez;
}

void checkResult(){
  int cont1 = 0;
  int cont2 = 0;
  int velha = 0;
  
  for (int i = 0; i < 8; i++){
     for (int j = 0; j < 9; j++){
        if (p1[j] == 1 && gabarito[i][j] == 1){
          cont1++;
          gabarito_pos_acerto = i;
        }
        if (p2[j] == 1 && gabarito[i][j] == 1){
          cont2++;
          gabarito_pos_acerto = i;
        }
      }
      if (cont1 == 3 || cont2 == 3) break; 
      else{
        cont1=0;
        cont2=0;
      }
   }
    
  if (cont1 == 3){
    animacaoVencedor(0);
    delay(1000);
    reiniciaJogo(0);
  }else if (cont2 == 3){
    animacaoVencedor(1);
    delay(1000);
    reiniciaJogo(1);
  }else{
    for (int i = 0; i < 9; i++){
      if (p1[i] != 0 || p2[i] != 0)
        velha++; 
    }

    if (velha == 9){
      animacao03();
      delay(1000);
      reiniciaJogo(3);
    }
  }
}

void reiniciaJogo(int jogador){
  
  jogador == 3 ? jogador_vez = random(0, 1) : jogador_vez = jogador_vez;
  
  for (int i = 0; i < 9; i++){
    p1[i] = 0;
    p2[i] = 0;
  }
  
  digitalWrite(led_jogador1, LOW);
  digitalWrite(led_jogador2, LOW);
}

void animacaoVencedor(int jogador){
  if (jogador == 0){
    animacao01();
  }else if (jogador == 1){
    animacao02();
  }
}
          
void animacao01(){
  int i;
  for (i = 0; i < 27; i++){
    ciWrite(i, LOW);
  }
  
  for (i = 0; i < 6; i++){
    for (int j = 0; j < 9; j++){
      if (i % 2 == 0 && gabarito[gabarito_pos_acerto][j] == 1)
        ciWrite(j*2, HIGH);
      else if (i % 2 == 1 && gabarito[gabarito_pos_acerto][j] == 1)
        ciWrite(j*2, LOW);
      delay(10);
    }
    delay(50);
  }
}

void animacao02(){
  int i;
  for (i = 0; i < 27; i++){
    ciWrite(i, LOW);
  }
  
  for (i = 0; i < 6; i++){
    for (int j = 0; j < 9; j++){
      if (i % 2 == 0 && gabarito[gabarito_pos_acerto][j] == 1)
        ciWrite((j*2)+1, HIGH);
      else if (i % 2 == 1 && gabarito[gabarito_pos_acerto][j] == 1)
        ciWrite((j*2)+1, LOW);
       delay(10);
    }
    delay(50);
  }
}

void animacao03(){
  int i;
  for (i = 0; i < 27; i++){
    ciWrite(i, LOW);
  }
  
  for (i = 0; i < 4; i++){
    for (int j = 0; j < 9; j++){
      if (i % 2 == 0){
        ciWrite(j*2, HIGH);
        ciWrite(j+18, HIGH);
      }
      else{
        ciWrite(j*2, LOW);
        ciWrite(j+18, LOW);
       }
      delay(10);
    }
    delay(50);
  }
}