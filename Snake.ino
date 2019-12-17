
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

#define PAUSE 9
#define MAX_WIDTH 84
#define MAX_HEIGHT 48

boolean dl = false, dr = false, du = false, dd = false; 
int x[200], y[200], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high = 0;
uint8_t bh, bl;
int xegg, yegg;
int freq, tb;
int l, r, u, d, p;
unsigned long time = 280, beeptime = 50;
int score = 0,flag = 0;


//Настройки: размер, контраст, сложность
    uint8_t size = 2;
    uint8_t contrast = 70;
    uint8_t difficulty = 0;
    //Выбранные в данный момент параметры в меню
    uint8_t _chosenOption = 0;
    uint8_t _section = 0;
    //Переменная, которая показывает, играем мы в данный момент или нет
    uint8_t _isPlaying = 0;
    //Таблица рекордов
    String Top1 = "ZZZ";
    uint8_t Top1level = 0;
    int Top1score = 0;

    String Top2 = "ZZZ";
    uint8_t Top2level = 0;
    int Top2score = 0;

    String Top3 = "ZZZ";
    uint8_t Top3level = 0;
    int Top3score = 0;

void direct();
void checkegg();
void drawsnake();  
int CalcRecords(String _name, int score);
void beep (int freq,long tb);
void redraw();

//Метод, вызывающий меню
    int showmenu()
    {
      while(true)
      {
        //Меняем выбранную опцию кнопками вверх и вниз
        if(digitalRead(UP)==LOW)
          if(_chosenOption == 0)
            _chosenOption = 3;
          else _chosenOption -= 1;

        if(digitalRead(DOWN)==LOW)
          if(_chosenOption == 3)
            _chosenOption = 0;
          else _chosenOption += 1;
        display.clearDisplay();

        //Вывод начальной панели меню
        if(_section == 0)
        {
              display.setTextSize(1);
              display.clearDisplay();
              display.setTextColor(BLACK, WHITE);
              display.setCursor(15, 0);
              display.print("MAIN MENU");
              display.drawFastHLine(0,10,83,BLACK);
              display.setCursor(0, 15);
              display.print(">Start");
              display.setCursor(0, 25);
              display.print(">Records");
              display.setCursor(0, 35);
              display.print(">Settings");
              display.display();
              if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_G)==LOW)
                //Действия для кнопки START
                if(_chosenOption == 0)
                {
                  //Начинаем игру
                  display.clearDisplay();
                 // return 1;
//                  while(true)
//                  {
//                    movesnake();
//                  }
                }
                //Для кнопки RECORDS
                else if(_chosenOption == 1)
                {
                  //Переходим к таблице рекордов
                  display.clearDisplay();
                  _section = 2;
                  _chosenOption = 0;
                  delay(100);
                }
                //Для кнопки SETTINGS
                else if(_chosenOption == 2)
                {
                  //Переходим к меню настроек
                  display.clearDisplay();
                  _section = 1;
                  _chosenOption = 0;
                  delay(100);
                }
    
        }
        //Вывод меню настроек
        else if(_section == 1)
        {
              display.setTextSize(1);
              display.clearDisplay();
              display.setTextColor(BLACK, WHITE);
              display.setCursor(0, 15);
              display.print(">Back");
              display.setCursor(0, 25);
              display.print(">Size");
              display.setCursor(0, 35);
              display.print(">Difficulty");
              display.display();

              if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_G)==LOW)
                if(_chosenOption == 0)
                {
                  //Возвращаемся в изначальное меню
                  display.clearDisplay();
                  _section = 0;
                  _chosenOption = 0;
                  delay(300);
                }
                if(_chosenOption == 1)
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
                }
                else if(_chosenOption == 2)
                {
                  //Меняем сложность(скорость) от 0 до 3
                  if(digitalRead(RIGHT)==LOW)
                  if(difficulty == 3)
                    difficulty = 0;
                  else difficulty++;
                else if(digitalRead(LEFT)==LOW)
                  if(difficulty == 0)
                    difficulty = 3;
                  else difficulty--;
                  EEPROM.put(9, difficulty);
                }
        }
        //Вывод таблицы рекордов
        else if (_section == 2)
        {
          display.setCursor(8, 5);
          display.print(">Back");
          display.setCursor(8, 13);
          display.print("1)");
          display.setCursor(20, 13);
          display.print("Top1");
          display.setCursor(47, 13);
          display.print(Top1score);

          display.setCursor(8, 21);
          display.print("2)");
          display.setCursor(20, 21);
          display.print("Top2");
          display.setCursor(47, 21);
          display.print(Top2score);

          display.setCursor(8, 29);
          display.print("3)");
          display.setCursor(20, 29);
          display.print("Top3");
          display.setCursor(47, 29);
          display.print(Top3score);

          display.display();
          if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_F)==LOW||digitalRead(BUTTON_G)==LOW)
                if(_chosenOption == 0)
                {
                  display.clearDisplay();
                  _section = 0;
                  _chosenOption = 0;
                  delay(300);
                }
        }
        delay(100);        
      }
    }


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

  display.begin();
  display.setContrast(60);

  slength = 4;             
  xegg = (display.width())/2;  
  yegg = (display.height())/2;  
  
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
}

void loop(){
  showmenu();
 // movesnake(); 
}

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
    checkegg();   
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
void checkgame()      
{
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
      display.print(high);     
      display.display();

       while (true)
        if (digitalRead(UP)==LOW||digitalRead(DOWN)==LOW||digitalRead(LEFT)==LOW||digitalRead(RIGHT)==LOW)
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

            display.setCursor(0,12);
            display.print("Press RIGHT");
            display.setCursor(0,20);
            display.print("To Try Again");
            display.setCursor(0,28);
            display.print("Press LEFT");
            display.setCursor(0,36);
            display.print("To Menu");

            
            display.display();
            delay(200);
          }
      if(digitalRead(RIGHT)==LOW)
      {
        
      delay(1000);
      CalcRecords(_name, score);
      beep(20,5000);            
      display.clearDisplay();      
      slength = 4;        
      score = 0;
      time = 280;      
      redraw(); 
      }     
      if(digitalRead(LEFT)==LOW)
      {
        
      display.clearDisplay();     
      CalcRecords(_name, score);
      display.display();
//      resetFunc();
      }             
    }
  }
}
void checkegg()   
{
  if(x[0]==xegg or x[0]==(xegg+1) or x[0]==(xegg+2) or x[0]==(xegg-1))  
  {
    if(y[0]==yegg or y[0]==(yegg+1) or y[0]==(yegg+2) or y[0]==(yegg-1))
    {
      score+=1;                
      slength+=1;
      if(time>=90)
      {time-=20;}      
      display.fillRect(xegg,yegg,3,3,WHITE);          
      display.display();            
      beep(35,beeptime);          
      xegg=random(1,80);            
      yegg=random(1,40);
    }
  }
}  
void redraw()
{
  display.fillRect(xegg,yegg,3,3,BLACK);
  for(i=0;i<slength;i++)
     {
     display.drawCircle(x[i],y[i],1,BLACK);
     }
     display.display();
}                   
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
void drawsnake()  
{
  display.fillRect(xegg,yegg,3,3,BLACK);  
  display.drawCircle(x[0],y[0],1,BLACK);  
  display.drawCircle(x[slength],y[slength],1,WHITE);
  display.display(); 
}
  
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










    //Метод пересчета позиций в таблице рекордов
    void CalcRecords(String _name, int score)
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

      //Запоминаем топ3 в памяти
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




   
