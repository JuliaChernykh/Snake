
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define BUTTON_E 6
#define BUTTON_F 7
#define BUTTON_G 8

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);

#define PAUSE 8
#define MAX_WIDTH 84
#define MAX_HEIGHT 48

boolean dl = false, dr = false, du = false, dd = false; 
int x[200], y[200], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high = 0;
uint8_t bh, bl;
int fruitx, fruitxy;
int freq, tb;
int l, r, u, d, p;
unsigned long time = 280, beeptime = 50;
int score = 0,flag = 0;

long randNumber;


//Настройки
uint8_t size = 1;
uint8_t difficulty = 0;
uint8_t contrast = 60;

//Опции в меню
uint8_t chosen_option = 0;
uint8_t section = 0;

//Рекорды
String Top1 = "ZZZ";
int Top1score = 0;

String Top2 = "ZZZ";
int Top2score = 0;

String Top3 = "ZZZ";
int Top3score = 0;

//Запускаем игру
void loopgame()
{
  while(true){
    movesnake();
  }
}

//Перезапуск
void(*reset) (void) = 0;

//Вввод настроек из памяти
    void savingsettings()
    {
      size = 1;
      difficulty = 0;

      Top1 = "ZZZ";
      Top1score = 0;

      Top2 = "ZZZ";
      Top2score = 0;

      Top3 = "ZZZ";
      Top3score = 0;
      
      if (EEPROM.read(0) >= 1 && EEPROM.read(0) <= 3)
        EEPROM.get(0,size);
      if (EEPROM.read(9) >= 0 && EEPROM.read(9) <= 3)
        EEPROM.get(9,difficulty);

        int count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(237 + i*9) >= 'A' && EEPROM.read(237 + i*9) <= 'Z')
            count++;
            
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(237 + i*9, Top1[i]);

        count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(362 + i*9) >= 'A' && EEPROM.read(362 + i*9) <= 'Z')
            count++;
          
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(362 + i*9, Top2[i]);

        count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(487 + i*9) >= 'A' && EEPROM.read(487 + i*9) <= 'Z')
            count++;
          
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(487 + i*9, Top3[i]);
        
      if(EEPROM.read(111) >= 0 && EEPROM.read(111) <= 32000)
        EEPROM.get(111, Top1score);
      if(EEPROM.read(128) >= 0 && EEPROM.read(128) <= 32000)
        EEPROM.get(128, Top2score);
      if(EEPROM.read(145) >= 0 && EEPROM.read(145) <= 32000)
        EEPROM.get(145, Top3score);
    }


int showmenu()
{
  while(true)
{
//Выбираем опцию в меню
    if(digitalRead(UP)==LOW)
    if(chosen_option == 0)
        chosen_option = 3;
     else chosen_option -= 1;

     if(digitalRead(DOWN)==LOW)
         if(chosen_option == 3)
            chosen_option = 0;
         else chosen_option += 1;
      display.clearDisplay();

      
//Рисуем меню
if(section == 0)
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.print("MAIN MENU");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
    if (chosen_option==0) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Start");
    display.setCursor(0, 25);
    if (chosen_option==1) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Records");
    display.setCursor(0, 35);
    if (chosen_option==2) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Settings");
     display.setCursor(0, 45);
//    if (chosen_option==3) 
//    { 
//      display.setTextColor(WHITE, BLACK);
//    }
//    else 
//    {
//      display.setTextColor(BLACK, WHITE);
//    }
//    display.print(">Contrast");
    display.display();
    
    if(digitalRead(RIGHT)==LOW)
    //START
    if(chosen_option == 0)
    {
       display.clearDisplay();
       loopgame();
    }
    //RECORDS
    else if(chosen_option == 1)
    {
       display.clearDisplay();
       section = 2;
       chosen_option = 0;
       delay(100);
    }
    //SETTINGS
    else if(chosen_option == 2)
    {
       display.clearDisplay();
       section = 1;
       chosen_option = 0;
       delay(100);
    }
//    //EXIT
//     else if(chosen_option == 3)
//    {
//       display.clearDisplay();
//
//       display.setCursor(0, 0);
//       display.print("Goodbye!");
//       display.display();
//       delay(1000);
//       display.clearDisplay();
//       display.display();
//       //LCD.enableSleep();
//    }    
}
    else if(section == 1)
    {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 5);
    
    
    if (chosen_option==0) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Back");
    display.setCursor(0, 15);
    
    
    if (chosen_option==1) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Size");
    display.setCursor(0, 25);
    
    if (chosen_option==2) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Difficulty");
//    display.setCursor(0, 65);
//    if(difficulty==0)
//    display.print("OFF");
//    else 
//    display.print("ON");
    display.setCursor(0, 35);
    
     if (chosen_option==3) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Contrast");
//    display.setCursor(0, 70);
//    display.print(contrast);
    display.display();

              if(digitalRead(RIGHT)==LOW||digitalRead(LEFT)==LOW)
                if(chosen_option == 0)
                {
                  
                  //Возвращаемся в изначальное меню
                  display.clearDisplay();
                  section = 0;
                  chosen_option = 0;
                  delay(300);
                }
                if(chosen_option == 1)
                {
                  
                  //Меняем размер от 1 до 3
                  if(digitalRead(RIGHT)==LOW)
                  if(size == 3)
                    size = 1;
                  else size++;
                else if(digitalRead(LEFT)==LOW)
                  if(size == 1)
                    size = 3;
                  else size--;
                  EEPROM.put(0, size);
                  display.setCursor(70, 15);
                     display.print(size);
                     display.display();
                }
                else if(chosen_option == 2)
                {
                  
                  //Включаем или выключаем препятствия в игре
                  if(digitalRead(RIGHT)==LOW)
                  {
                     difficulty = 1;
 //                    display.setCursor(70, 25);
//                     display.print("ON");
//                     display.display();
                  }
                     
                else if(digitalRead(LEFT)==LOW)
                {
                     difficulty = 0;
//                     display.setCursor(65, 25);
//                     display.print("OFF");
//                    
//                      display.display();
                }
                  EEPROM.put(9, difficulty);
                  if(difficulty==0)
                  {
                    display.setCursor(65, 25);
                    display.print("OFF");
                     display.display();
                  }
                  else 
                  {
                    display.setCursor(70, 25);
                    display.print("ON");
                     display.display();
                  }
            }
            else if(chosen_option == 3)
                {
                  
                  //Включаем или выключаем препятствия в игре
                  if(digitalRead(RIGHT)==LOW)
                  {
                     if(contrast == 70)
                    contrast = 50;
                    else contrast += 2;
                  }
                     
                else if(digitalRead(LEFT)==LOW)
                {
                     if(contrast == 50)
                    contrast = 70;
                    else contrast -= 2;
                }
                display.setContrast(contrast);
                EEPROM.put(18, contrast);
                display.setCursor(65, 35);
                display.print(contrast);
                display.display();
            }
        }
        //Рекорды
        else if (section == 2)
        {
          display.setCursor(8, 5);
          display.print(">Back");
          display.setCursor(8, 13);
          display.print("1)");
          display.setCursor(20, 13);
          display.print(Top1);
          display.setCursor(47, 13);
          display.print(Top1score);

          display.setCursor(8, 21);
          display.print("2)");
          display.setCursor(20, 21);
          display.print(Top2);
          display.setCursor(47, 21);
          display.print(Top2score);

          display.setCursor(8, 29);
          display.print("3)");
          display.setCursor(20, 29);
          display.print(Top3);
          display.setCursor(47, 29);
          display.print(Top3score);

          display.display();
          if(digitalRead(RIGHT)==LOW||digitalRead(LEFT)==LOW)
                if(chosen_option == 0)
                {
                  display.clearDisplay();
                  section = 0;
                  chosen_option = 0;
                  delay(300);
                }
        }
        delay(100);        
      }
    }

//Задаем настройки игры
void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);  
  
  pinMode(UP, INPUT_PULLUP);
  digitalWrite(UP, HIGH);
  pinMode(RIGHT, INPUT_PULLUP);
  digitalWrite(RIGHT, HIGH);
  pinMode(DOWN, INPUT_PULLUP);
  digitalWrite(DOWN, HIGH);
  pinMode(LEFT, INPUT_PULLUP);
  digitalWrite(LEFT, HIGH);

  randomSeed(analogRead(0));

  display.begin();
  display.setContrast(60);

  slength = 4;             
  fruitx = (display.width())/2;  
  fruitxy = (display.height())/2;  
  
  display.setTextColor(BLACK);
  display.setTextSize(2);

  Serial.begin(115200);

  display.clearDisplay();
  display.setCursor(10,20);
  display.print("SNAKE");
  display.setCursor(10,50);
  display.display();
  delay(4000);
  
  display.clearDisplay();  

            
  for(i = 0; i <= slength; i++)  
  {
    x[i]=25-3*i;
    y[i]=10;   
  }  
  display.display();
  dr = true; 
  
  savingsettings();
  showmenu();
}

void loop(){  
  showmenu(); 
}

//Направление змейки
void direct()             
{
  if(l==LOW and dr==false)      
  {
    dl=true;du=false;dd=false;
    tempx=x[0]-3;          
    tempy=y[0];
    flag=1;                    
  }  
  else if(r==LOW and dl==false)
  {
    dr=true;du=false;dd=false;
    tempx=x[0]+3;
    tempy=y[0];
    flag=1;
  }  
  else if(u==LOW and dd==false)
  {
    du=true;dl=false;dr=false;
    tempy=y[0]-3;
    tempx=x[0];
    flag=1;
  }  
  else if(d==LOW and du==false)
  {
    dd=true;dl=false;dr=false;
    tempy=y[0]+3;
    tempx=x[0];
    flag=1;
  } 
  else if(p==LOW)
  {
  display.clearDisplay();  
  display.setTextColor(BLACK);
  for(i=5;i>0;i--)
    {
     display.setCursor(25,10); 
     display.setTextSize(1);
     display.print("PAUSED");
     display.setCursor(40,30);
     display.print(i);
     display.display();
     delay(1000);
     display.clearDisplay();
    } 
    redraw();    
  } 
}

//Риусем препятствия 
void drawobstacles()
{
  display.fillRect(1,1,2,10,BLACK);  
  display.fillRect(1,1,5,2,BLACK);  
  display.fillRect(15,30,6,2,BLACK);  
  display.fillRect(60,30,2,10,BLACK);  
  display.fillRect(75,15,2,10,BLACK);  
  display.fillRect(70,15,5,2,BLACK);  
  display.display();
}

//Рисуем змейку
void drawsnake()  
{
//  if(size == 3)
//       {
//        
//        for (uint8_t _i = 0; _i < size; ++_i)
//          for (uint8_t _j = 0; _j < size; ++_j)
//              display.drawPixel(size*i+_i,size*j+_j,WHITE);
//       }
//       else {
//        for (uint8_t _i = 0; _i < size; ++_i)
//          for (uint8_t _j = 0; _j < size; ++_j)
//              display.drawPixel(size*j+_j,size*i+_i,WHITE);
//       }
  if(difficulty==1)
    drawobstacles();
  
  display.fillRect(fruitx,fruitxy,3*size,3*size,BLACK);  
  display.drawCircle(x[0],y[0],1*size,BLACK);  
  display.drawCircle(x[slength],y[slength],1*size,WHITE);
  display.display(); 
}

//Перерисовываем змейку(для игры заново)
void redraw()
{
  drawobstacles();
  display.fillRect(fruitx,fruitxy,3,3,BLACK);
  for(i=0;i<slength;i++)
     {
     display.drawCircle(x[i],y[i],1,BLACK);
     }
     display.display();
}   

//Создаем ускорение          
void beep (int freq,long tb)  
{ 
    int x;   
    long delayAmount = (long)(500/freq);   
    long loopTime = (long)(tb/(delayAmount*2)); 
    for (x=0;x<loopTime;x++)     // One iteration produces sound for one cycle
    {    
        digitalWrite(UP,HIGH);
        delay(delayAmount);
        digitalWrite(UP,LOW); 
        delay(delayAmount);
    }        
    delay(2);   
}

//Проверяем, съела ли змейка фрукт
void checkfruit()   
{
  if(size==1)
  {
  if(x[0]==fruitx or x[0]==(fruitx+1) or x[0]==(fruitx+2) or x[0]==(fruitx-1))  
  {
    if(y[0]==fruitxy or y[0]==(fruitxy+1) or y[0]==(fruitxy+2) or y[0]==(fruitxy-1))
    {
      score+=1;                
      slength+=1;
      if(time>=90)
      {time-=20;}      
      display.fillRect(fruitx,fruitxy,3*size,3*size,WHITE);          
      display.display();            
      beep(35,beeptime);          
      fruitx=random(1,80);            
      fruitxy=random(1,40);
    }
  }
  }   
  if(size==2)
  {
  if(x[0]==fruitx or x[0]==(fruitx+1) or x[0]==(fruitx+2) or x[0]==(fruitx+3) or x[0]==(fruitx+4) or x[0]==(fruitx-1))  
  {
    if(y[0]==fruitxy or y[0]==(fruitxy+1) or y[0]==(fruitxy+2) or y[0]==(fruitxy+3) or y[0]==(fruitxy+4) or y[0]==(fruitxy-1))
    {
      score+=1;                
      slength+=1;
      if(time>=90)
      {time-=20;}      
      display.fillRect(fruitx,fruitxy,3*size,3*size,WHITE);          
      display.display();            
      beep(35,beeptime);          
      fruitx=random(1,80);            
      fruitxy=random(1,40);
    }
  }
  }  
  if(size==3)
  {
  if(x[0]==fruitx or x[0]==(fruitx+1) or x[0]==(fruitx+2) or x[0]==(fruitx+3) or x[0]==(fruitx+4) or x[0]==(fruitx+5) or x[0]==(fruitx+6) or x[0]==(fruitx+7) or x[0]==(fruitx-1))  
  {
    if(y[0]==fruitxy or y[0]==(fruitxy+1) or y[0]==(fruitxy+2) or y[0]==(fruitxy+3) or y[0]==(fruitxy+4) or y[0]==(fruitxy+5) or y[0]==(fruitxy+6) or y[0]==(fruitxy+7) or y[0]==(fruitxy-1))
    {
      score+=1;                
      slength+=1;
      if(time>=90)
      {time-=20;}      
      display.fillRect(fruitx,fruitxy,3*size,3*size,WHITE);          
      display.display();            
      beep(35,beeptime);          
      fruitx=random(1,80);            
      fruitxy=random(1,40);
    }
  }
  }    
}     

//Перемещаем змейку
void movesnake()
{
  l = digitalRead(LEFT);   
  d = digitalRead(DOWN);
  r = digitalRead(RIGHT);
  u = digitalRead(UP);
  p = digitalRead(PAUSE);  
  
  if(flag == 0)
  {
  direct();            
  }
  if(millis() % time == 0)   
  {             
     if(flag == 0)                                
     {
       if(dr == true){tempx=x[0] + 3; tempy = y[0];}   
       if(dl == true){tempx=x[0] - 3; tempy = y[0];}    
       if(du == true){tempy=y[0] - 3; tempx = x[0];}
       if(dd == true){tempy=y[0] + 3; tempx = x[0];}
     }   
    flag=0;  
    checkgame();                       
    checkfruit();   
    if(tempx <= 0){tempx = 84+tempx;}    
    if(tempx >= 84){tempx = tempx-84;}
    if(tempy <= 0){tempy = 48+tempy;}
    if(tempy >= 48){tempy = tempy-48;}
    for(i=0;i<=slength;i++)      
    {
     xx=x[i];
     yy=y[i]; 
     x[i]=tempx;
     y[i]=tempy;
     tempx=xx;
     tempy=yy;
    }  
    drawsnake();        
  }
}

//Пересчет позиций в таблице рекордов
    void calculatingscore(String _name, int score)
    {
      
      if (score >= Top1score)
      {
        if (score == Top1score)
        {
          if (_name.compareTo(Top1) < 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
  
            for (int i = 0; i<3;++i)
              Top2[i] = Top1[i];
            Top2score = Top1score;
            
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = score;
          }
          else if(_name.compareTo(Top1) > 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = score;
          }
          else if (_name.compareTo(Top1) == 0)
          {
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = score;
          }
        }
        else
        {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
  
            for (int i = 0; i<3;++i)
              Top2[i] = Top1[i];
            Top2score = Top1score;
            
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = score;
        }
      }
      else if (score >= Top2score)
      {
        if (score == Top2score)
        {
          if(_name.compareTo(Top2) < 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = score;
          }
          else if(_name.compareTo(Top2) > 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = _name[i];
            Top3score = score;
          }
          else if(_name.compareTo(Top2) == 0)
          {
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = score;
          }
        }
        else
        {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = score;
        }
      }
      else if (score >= Top3score)
      {
            for (int i = 0; i<3;++i)
              Top3[i] = _name[i];
            Top3score = score;
      }

      //Запоминаем рекорды в памяти
      for (int i = 0; i < 3; ++i)
        EEPROM.write(237 + i*9, Top1[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(362 + i*9, Top2[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(487 + i*9, Top3[i]);

      EEPROM.put(111, Top1score);
      EEPROM.put(128, Top2score);
      EEPROM.put(145, Top3score);
    }

//Проверяем, не закончена ли играч
void checkgame()      
{
  if(difficulty==1)
  {
   for(int i = 1; i <= 10; i++)
  {
    if(x[0]==60 && y[0]==29+i || x[0]==61 && y[0]==29+i
       || x[0]==1 && y[0]==i || x[0]==2 && y[0]==i
       || x[0]==75 && y[0]==14+i || x[0]==76 && y[0]==14+i
       || i<6 && x[0]==i && y[0]==1 || i<6 && x[0]==i && y[0]==2
       || i<6 && x[0]==14+i && y[0]==30 || i<6 && x[0]==14+i && y[0]==31
       || i<6 && x[0]==69+i && y[0]==15 || i<6 && x[0]==69+i && y[0]==16)
  
     {      
      bh = EEPROM.read(1);
      bl = EEPROM.read(0);     
      if(score > high)
      {
        high = score;
        bh = (high >> 8);
        bl =high & 0xff;       
        EEPROM.write(1,bh);
        EEPROM.write(0,bl);
      }
      display.clearDisplay();
      display.setTextColor(BLACK);       
      display.setTextSize(1);
      display.setCursor(20,12);
      display.print("Game Over");
      display.setCursor(15,30);
      display.print("Score: ");
      display.print(score);
      display.setCursor(15,40);
      display.print("High: ");
      display.print(Top1score);     
      display.display();

      delay(200);
      display.clearDisplay();  

       while (true)
        if (digitalRead(LEFT)==LOW||digitalRead(RIGHT)==LOW)
        {
          String _name = "AAA";
          uint8_t _chosenChar = 0;
          while (true)
          {
            //Ввод имени игрока
            if(digitalRead(LEFT)==LOW)
              if(_chosenChar == 0)
                _chosenChar = 2;
                else _chosenChar -= 1;

            if(digitalRead(RIGHT)==LOW)
              if(_chosenChar == 2)
                _chosenChar = 0;
                else _chosenChar += 1;
                
            if(digitalRead(DOWN)==LOW)
              if(_name[_chosenChar] == 'A')
                _name[_chosenChar] = 'Z';
                else _name[_chosenChar] -= 1;

            if(digitalRead(UP)==LOW)
              if(_name[_chosenChar] == 'Z')
                _name[_chosenChar] = 'A';
                else _name[_chosenChar] += 1;
 
            display.clearDisplay(); 
            display.setCursor(0,0);
            display.print("Enter Name:");
            display.setCursor(64,0);
            display.print(_name);
           
            display.display();
            delay(200);
     
      if(digitalRead(BUTTON_E)==LOW)
      { 
      display.clearDisplay();     
      calculatingscore(_name, score);
      display.display();
      delay(1000);
      reset();
      }             
    }
  }
}
  }
  }

  for(i=1;i<slength;i++)            
  {
    if(x[i] == x[0] && y[i] == y[0])
    {      
      bh = EEPROM.read(1);
      bl = EEPROM.read(0);     
      if(score > high)
      {
        high = score;
        bh = (high >> 8);
        bl =high & 0xff;       
        EEPROM.write(1,bh);
        EEPROM.write(0,bl);
      }
      display.clearDisplay();
      display.setTextColor(BLACK);       
      display.setTextSize(1);
      display.setCursor(20,12);
      display.print("Game Over");
      display.setCursor(15,30);
      display.print("Score: ");
      display.print(score);
      display.setCursor(15,40);
      display.print("High: ");
      display.print(Top1score);     
      display.display();

      delay(200);
      display.clearDisplay();  

       while (true)
        if (digitalRead(LEFT)==LOW||digitalRead(RIGHT)==LOW)
        {
          String _name = "AAA";
          uint8_t _chosenChar = 0;
          while (true)
          {
            //Ввод имени игрока
            if(digitalRead(LEFT)==LOW)
              if(_chosenChar == 0)
                _chosenChar = 2;
                else _chosenChar -= 1;

            if(digitalRead(RIGHT)==LOW)
              if(_chosenChar == 2)
                _chosenChar = 0;
                else _chosenChar += 1;
                
            if(digitalRead(DOWN)==LOW)
              if(_name[_chosenChar] == 'A')
                _name[_chosenChar] = 'Z';
                else _name[_chosenChar] -= 1;

            if(digitalRead(UP)==LOW)
              if(_name[_chosenChar] == 'Z')
                _name[_chosenChar] = 'A';
                else _name[_chosenChar] += 1;
 
            display.clearDisplay(); 
            display.setCursor(0,0);
            display.print("Enter Name:");
            display.setCursor(64,0);
            display.print(_name);
           
            display.display();
            delay(200);
     
      if(digitalRead(BUTTON_E)==LOW)
      { 
      display.clearDisplay();     
      calculatingscore(_name, score);
      display.display();
      delay(1000);
      reset();
      }             
    }
  }
}
}
}














    



   
